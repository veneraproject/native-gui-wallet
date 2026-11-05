                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#include <algorithm>
#include <cstdio>
#include <boost/filesystem.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/format.hpp>

#include "include_base_utils.h"
#include "cryptonote_basic/cryptonote_basic_impl.h"
#include "tx_pool.h"
#include "blockchain.h"
#include "blockchain_db/blockchain_db.h"
#include "cryptonote_basic/cryptonote_boost_serialization.h"
#include "cryptonote_config.h"
#include "cryptonote_basic/reward_burn.h"
#include "cryptonote_basic/miner.h"
#include "hardforks/hardforks.h"
#include "misc_language.h"
#include "profile_tools.h"
#include "file_io_utils.h"
#include "int-util.h"
#include "common/threadpool.h"
#include "common/boost_serialization_helper.h"
#include "warnings.h"
#include "crypto/hash.h"
#include "cryptonote_core.h"
#include "cryptonote_basic/block_approval.h"
#include "ringct/rctSigs.h"
#include "common/perf_timer.h"
#include "common/notify.h"
#include "common/varint.h"
#include "common/pruning.h"
#include "common/data_cache.h"
#include "time_helper.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "blockchain"

#define FIND_BLOCKCHAIN_SUPPLEMENT_MAX_SIZE (100*1024*1024)  

using namespace crypto;

 

 
 
 
 
 

using namespace cryptonote;
using epee::string_tools::pod_to_hex;
extern "C" void slow_hash_allocate_state();
extern "C" void slow_hash_free_state();

DISABLE_VS_WARNINGS(4267)

#define MERROR_VER(x) MCERROR("verify", x)

 
#define BLOCK_REWARD_OVERESTIMATE (10 * 1000000000000)

 
Blockchain::Blockchain(tx_memory_pool& tx_pool) :
  m_db(), m_tx_pool(tx_pool), m_hardfork(NULL), m_timestamps_and_difficulties_height(0), m_reset_timestamps_and_difficulties_height(true), m_current_block_cumul_weight_limit(0), m_current_block_cumul_weight_median(0),
  m_enforce_dns_checkpoints(false), m_max_prepare_blocks_threads(4), m_db_sync_on_blocks(true), m_db_sync_threshold(1), m_db_sync_mode(db_async), m_db_default_sync(false), m_fast_sync(true), m_show_time_stats(false), m_sync_counter(0), m_bytes_to_sync(0), m_cancel(false),
  m_long_term_block_weights_window(CRYPTONOTE_LONG_TERM_BLOCK_WEIGHT_WINDOW_SIZE),
  m_long_term_effective_median_block_weight(0),
  m_long_term_block_weights_cache_tip_hash(crypto::null_hash),
  m_long_term_block_weights_cache_rolling_median(CRYPTONOTE_LONG_TERM_BLOCK_WEIGHT_WINDOW_SIZE),
  m_difficulty_for_next_block_top_hash(crypto::null_hash),
  m_difficulty_for_next_block(1),
  m_btc_valid(false),
  m_batch_success(true),
  m_prepare_height(0),
  m_rct_ver_cache()
{
  LOG_PRINT_L3("Blockchain::" << __func__);
}
 
Blockchain::~Blockchain()
{
  try { deinit(); }
  catch (const std::exception &e) {   }
}
 
bool Blockchain::have_tx(const crypto::hash &id) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
   
   
   
  return m_db->tx_exists(id);
}
 
bool Blockchain::have_tx_keyimg_as_spent(const crypto::key_image &key_im) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
   
   
   
  return  m_db->has_key_image(key_im);
}
 
 
 
 
template <class visitor_t>
bool Blockchain::scan_outputkeys_for_indexes(size_t tx_version, const txin_to_key& tx_in_to_key, visitor_t &vis, const crypto::hash &tx_prefix_hash, uint64_t* pmax_related_block_height) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);

   
   

   
  if(!tx_in_to_key.key_offsets.size())
    return false;

   
   
   
   
  std::vector<uint64_t> absolute_offsets = relative_output_offsets_to_absolute(tx_in_to_key.key_offsets);
  std::vector<output_data_t> outputs;

  bool found = false;
  auto it = m_scan_table.find(tx_prefix_hash);
  if (it != m_scan_table.end())
  {
    auto its = it->second.find(tx_in_to_key.k_image);
    if (its != it->second.end())
    {
      outputs = its->second;
      found = true;
    }
  }

  if (!found)
  {
    try
    {
      m_db->get_output_key(epee::span<const uint64_t>(&tx_in_to_key.amount, 1), absolute_offsets, outputs, true);
      if (absolute_offsets.size() != outputs.size())
      {
        MERROR_VER("Output does not exist! amount = " << tx_in_to_key.amount);
        return false;
      }
    }
    catch (...)
    {
      MERROR_VER("Output does not exist! amount = " << tx_in_to_key.amount);
      return false;
    }
  }
  else
  {
     
    if (outputs.size() < absolute_offsets.size() && outputs.size() > 0)
    {
      MDEBUG("Additional outputs needed: " << absolute_offsets.size() - outputs.size());
      std::vector < uint64_t > add_offsets;
      std::vector<output_data_t> add_outputs;
      add_outputs.reserve(absolute_offsets.size() - outputs.size());
      for (size_t i = outputs.size(); i < absolute_offsets.size(); i++)
        add_offsets.push_back(absolute_offsets[i]);
      try
      {
        m_db->get_output_key(epee::span<const uint64_t>(&tx_in_to_key.amount, 1), add_offsets, add_outputs, true);
        if (add_offsets.size() != add_outputs.size())
        {
          MERROR_VER("Output does not exist! amount = " << tx_in_to_key.amount);
          return false;
        }
      }
      catch (...)
      {
        MERROR_VER("Output does not exist! amount = " << tx_in_to_key.amount);
        return false;
      }
      outputs.insert(outputs.end(), add_outputs.begin(), add_outputs.end());
    }
  }

  size_t count = 0;
  for (const uint64_t& i : absolute_offsets)
  {
    try
    {
      output_data_t output_index;
      try
      {
         
        if (count < outputs.size())
          output_index = outputs.at(count);
        else
          output_index = m_db->get_output_key(tx_in_to_key.amount, i);

         
        if (!vis.handle_output(output_index.unlock_time, output_index.pubkey, output_index.commitment))
        {
          MERROR_VER("Failed to handle_output for output no = " << count << ", with absolute offset " << i);
          return false;
        }
      }
      catch (...)
      {
        MERROR_VER("Output does not exist! amount = " << tx_in_to_key.amount << ", absolute_offset = " << i);
        return false;
      }

       
      if(++count == absolute_offsets.size() && pmax_related_block_height)
      {
         
        auto h = output_index.height;
        if(*pmax_related_block_height < h)
        {
          *pmax_related_block_height = h;
        }
      }

    }
    catch (const OUTPUT_DNE& e)
    {
      MERROR_VER("Output does not exist: " << e.what());
      return false;
    }
    catch (const TX_DNE& e)
    {
      MERROR_VER("Transaction does not exist: " << e.what());
      return false;
    }

  }

  return true;
}
 
uint64_t Blockchain::get_current_blockchain_height() const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
   
   
   
  return m_db->height();
}
 
 
 
bool Blockchain::init(BlockchainDB* db, const network_type nettype, bool offline, const cryptonote::test_options *test_options, difficulty_type fixed_difficulty, const GetCheckpointsCallback& get_checkpoints )
{
  LOG_PRINT_L3("Blockchain::" << __func__);

  CHECK_AND_ASSERT_MES(nettype != FAKECHAIN || test_options, false, "fake chain network type used without options");

  CRITICAL_REGION_LOCAL(m_tx_pool);
  CRITICAL_REGION_LOCAL1(m_blockchain_lock);

  if (db == nullptr)
  {
    LOG_ERROR("Attempted to init Blockchain with null DB");
    return false;
  }
  if (!db->is_open())
  {
    LOG_ERROR("Attempted to init Blockchain with unopened DB");
    delete db;
    return false;
  }

  m_db = db;

  m_nettype = test_options != NULL ? FAKECHAIN : nettype;
  m_offline = offline;
  m_fixed_difficulty = fixed_difficulty;
  if (m_hardfork == nullptr)
  {
    if (m_nettype ==  FAKECHAIN || m_nettype == STAGENET)
      m_hardfork = new HardFork(*db, 1, 0);
    else if (m_nettype == TESTNET)
      m_hardfork = new HardFork(*db, 1, testnet_hard_fork_version_1_till);
    else
      m_hardfork = new HardFork(*db, 1, mainnet_hard_fork_version_1_till);
  }
  if (m_nettype == FAKECHAIN)
  {
    for (size_t n = 0; test_options->hard_forks[n].first; ++n)
      m_hardfork->add_fork(test_options->hard_forks[n].first, test_options->hard_forks[n].second, 0, n + 1);
  }
  else if (m_nettype == TESTNET)
  {
    for (size_t n = 0; n < num_testnet_hard_forks; ++n)
      m_hardfork->add_fork(testnet_hard_forks[n].version, testnet_hard_forks[n].height, testnet_hard_forks[n].threshold, testnet_hard_forks[n].time);
  }
  else if (m_nettype == STAGENET)
  {
    for (size_t n = 0; n < num_stagenet_hard_forks; ++n)
      m_hardfork->add_fork(stagenet_hard_forks[n].version, stagenet_hard_forks[n].height, stagenet_hard_forks[n].threshold, stagenet_hard_forks[n].time);
  }
  else
  {
    for (size_t n = 0; n < num_mainnet_hard_forks; ++n)
      m_hardfork->add_fork(mainnet_hard_forks[n].version, mainnet_hard_forks[n].height, mainnet_hard_forks[n].threshold, mainnet_hard_forks[n].time);
  }
  m_hardfork->init();

  m_db->set_hard_fork(m_hardfork);

   
   
   
   
  if(!m_db->height())
  {
    MINFO("Blockchain not loaded, generating genesis block.");
    block bl;
    block_verification_context bvc = {};
    generate_genesis_block(bl, get_config(m_nettype).GENESIS_TX, get_config(m_nettype).GENESIS_NONCE);
    db_wtxn_guard wtxn_guard(m_db);
    add_new_block(bl, bvc);
    CHECK_AND_ASSERT_MES(!bvc.m_verifivation_failed, false, "Failed to add genesis block to blockchain");
  }
   
   
  else
  {
  }


  db_rtxn_guard rtxn_guard(m_db);

   
  uint64_t top_block_timestamp = m_db->get_top_block_timestamp();
  uint64_t timestamp_diff = time(NULL) - top_block_timestamp;

   
  if(!top_block_timestamp)
    timestamp_diff = time(NULL) - 1397818133;

   

  m_async_work_idle = std::unique_ptr < boost::asio::io_service::work > (new boost::asio::io_service::work(m_async_service));
   
  m_async_pool.create_thread(boost::bind(&boost::asio::io_service::run, &m_async_service));

#if defined(PER_BLOCK_CHECKPOINT)
  if (m_nettype != FAKECHAIN)
    load_compiled_in_block_hashes(get_checkpoints);
#endif

  MINFO("Blockchain initialized. last block: " << m_db->height() - 1 << ", " << epee::misc_utils::get_time_interval_string(timestamp_diff) << " time ago, current difficulty: " << get_difficulty_for_next_block());

  rtxn_guard.stop();

  uint64_t num_popped_blocks = 0;
  while (!m_db->is_read_only())
  {
    uint64_t top_height;
    const crypto::hash top_id = m_db->top_block_hash(&top_height);
    const block top_block = m_db->get_top_block();
    const uint8_t ideal_hf_version = get_ideal_hard_fork_version(top_height);
    if (ideal_hf_version <= 1 || ideal_hf_version == top_block.major_version)
    {
      if (num_popped_blocks > 0)
        MGINFO("Initial popping done, top block: " << top_id << ", top height: " << top_height << ", block version: " << (uint64_t)top_block.major_version);
      break;
    }
    else
    {
      if (num_popped_blocks == 0)
        MGINFO("Current top block " << top_id << " at height " << top_height << " has version " << (uint64_t)top_block.major_version << " which disagrees with the ideal version " << (uint64_t)ideal_hf_version);
      if (num_popped_blocks % 100 == 0)
        MGINFO("Popping blocks... " << top_height);
      ++num_popped_blocks;
      block popped_block;
      std::vector<transaction> popped_txs;
      try
      {
        m_db->pop_block(popped_block, popped_txs);
      }
       
       
      catch (const std::exception& e)
      {
        MERROR("Error popping block from blockchain: " << e.what());
        throw;
      }
      catch (...)
      {
        MERROR("Error popping block from blockchain, throwing!");
        throw;
      }
    }
  }
  if (num_popped_blocks > 0)
  {
    m_timestamps_and_difficulties_height = 0;
    m_reset_timestamps_and_difficulties_height = true;
    m_hardfork->reorganize_from_chain_height(get_current_blockchain_height());
    uint64_t top_block_height;
    crypto::hash top_block_hash = get_tail_id(top_block_height);
    m_tx_pool.on_blockchain_dec(top_block_height, top_block_hash);
  }

  if (test_options && test_options->long_term_block_weight_window)
  {
    m_long_term_block_weights_window = test_options->long_term_block_weight_window;
    m_long_term_block_weights_cache_rolling_median = epee::misc_utils::rolling_median_t<uint64_t>(m_long_term_block_weights_window);
  }

  bool difficulty_ok;
  uint64_t difficulty_recalc_height;
  std::tie(difficulty_ok, difficulty_recalc_height) = check_difficulty_checkpoints();
  if (!difficulty_ok)
  {
    MERROR("Difficulty drift detected!");
    recalculate_difficulties(difficulty_recalc_height);
  }

  {
    db_txn_guard txn_guard(m_db, m_db->is_read_only());
    if (!update_next_cumulative_weight_limit())
      return false;
  }

  const crypto::hash seedhash = get_block_id_by_height(crypto::rx_seedheight(m_db->height()));
  if (seedhash != crypto::null_hash)
    rx_set_main_seedhash(seedhash.data, tools::get_max_concurrency());

  return true;
}
 
bool Blockchain::init(BlockchainDB* db, HardFork*& hf, const network_type nettype, bool offline)
{
  if (hf != nullptr)
    m_hardfork = hf;
  bool res = init(db, nettype, offline, NULL);
  if (hf == nullptr)
    hf = m_hardfork;
  return res;
}
 
bool Blockchain::store_blockchain()
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
  CRITICAL_REGION_LOCAL(m_db->m_synchronization_lock);

  TIME_MEASURE_START(save);
   
   
  try
  {
    m_db->sync();
  }
  catch (const std::exception& e)
  {
    MERROR(std::string("Error syncing blockchain db: ") + e.what() + "-- shutting down now to prevent issues!");
    throw;
  }
  catch (...)
  {
    MERROR("There was an issue storing the blockchain, shutting down now to prevent issues!");
    throw;
  }

  TIME_MEASURE_FINISH(save);
  if(m_show_time_stats)
    MINFO("Blockchain stored OK, took: " << save << " ms");
  return true;
}
 
bool Blockchain::deinit()
{
  LOG_PRINT_L3("Blockchain::" << __func__);

  MTRACE("Stopping blockchain read/write activity");

  
  m_async_work_idle.reset();
  m_async_pool.join_all();
  m_async_service.stop();

   
   
   
  try
  {
    if (m_db)
    {
      m_db->close();
      MTRACE("Local blockchain read/write activity stopped successfully");
    }
  }
  catch (const std::exception& e)
  {
    LOG_ERROR(std::string("Error closing blockchain db: ") + e.what());
  }
  catch (...)
  {
    LOG_ERROR("There was an issue closing/storing the blockchain, shutting down now to prevent issues!");
  }

  delete m_hardfork;
  m_hardfork = NULL;
  delete m_db;
  m_db = NULL;
  return true;
}
 
 
 
void Blockchain::pop_blocks(uint64_t nblocks)
{
  uint64_t i = 0;
  CRITICAL_REGION_LOCAL(m_tx_pool);
  CRITICAL_REGION_LOCAL1(m_blockchain_lock);

  bool stop_batch = m_db->batch_start();

  try
  {
    const uint64_t blockchain_height = m_db->height();
    if (blockchain_height > 0)
      nblocks = std::min(nblocks, blockchain_height - 1);
    while (i < nblocks)
    {
      pop_block_from_blockchain();
      ++i;
    }
  }
  catch (const std::exception& e)
  {
    LOG_ERROR("Error when popping blocks after processing " << i << " blocks: " << e.what());
    if (stop_batch)
      m_db->batch_abort();
    return;
  }

  CHECK_AND_ASSERT_THROW_MES(update_next_cumulative_weight_limit(), "Error updating next cumulative weight limit");

  if (stop_batch)
    m_db->batch_stop();

  const crypto::hash seedhash = get_block_id_by_height(crypto::rx_seedheight(m_db->height()));
  rx_set_main_seedhash(seedhash.data, tools::get_max_concurrency());
}
 
 
 
 
block Blockchain::pop_block_from_blockchain()
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  m_timestamps_and_difficulties_height = 0;
  m_reset_timestamps_and_difficulties_height = true;

  block popped_block;
  std::vector<transaction> popped_txs;

  CHECK_AND_ASSERT_THROW_MES(m_db->height() > 1, "Cannot pop the genesis block");

  const uint8_t previous_hf_version = get_current_hard_fork_version();
  try
  {
    m_db->pop_block(popped_block, popped_txs);
  }
   
   
  catch (const std::exception& e)
  {
    LOG_ERROR("Error popping block from blockchain: " << e.what());
    throw;
  }
  catch (...)
  {
    LOG_ERROR("Error popping block from blockchain, throwing!");
    throw;
  }

   
  m_hardfork->on_block_popped(1);

   
  size_t pruned = 0;
  for (transaction& tx : popped_txs)
  {
    if (tx.pruned)
    {
      ++pruned;
      continue;
    }
    if (!is_coinbase(tx))
    {
      cryptonote::tx_verification_context tvc = AUTO_VAL_INIT(tvc);

       
       
       
      uint8_t version = get_ideal_hard_fork_version(m_db->height());

       
       
       
       
       
      bool r = m_tx_pool.add_tx(tx, tvc, relay_method::block, true, version);
      if (!r)
      {
        LOG_ERROR("Error returning transaction to tx_pool");
      }
    }
  }
  if (pruned)
    MWARNING(pruned << " pruned txes could not be added back to the txpool");

  m_blocks_longhash_table.clear();
  m_scan_table.clear();
  m_blocks_txs_check.clear();

  uint64_t top_block_height;
  crypto::hash top_block_hash = get_tail_id(top_block_height);
  m_tx_pool.on_blockchain_dec(top_block_height, top_block_hash);
  invalidate_block_template_cache();

  const uint8_t new_hf_version = get_current_hard_fork_version();
  if (new_hf_version != previous_hf_version)
  {
    MINFO("Validating txpool for v" << (unsigned)new_hf_version);
    m_tx_pool.validate(new_hf_version);
  }

  return popped_block;
}
 
bool Blockchain::reset_and_set_genesis_block(const block& b)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  m_timestamps_and_difficulties_height = 0;
  m_reset_timestamps_and_difficulties_height = true;
  invalidate_block_template_cache();
  m_db->reset();
  m_db->drop_alt_blocks();
  m_hardfork->init();

  db_wtxn_guard wtxn_guard(m_db);
  block_verification_context bvc = {};
  add_new_block(b, bvc);
  if (!update_next_cumulative_weight_limit())
    return false;
  return bvc.m_added_to_main_chain && !bvc.m_verifivation_failed;
}
 
crypto::hash Blockchain::get_tail_id(uint64_t& height) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  return m_db->top_block_hash(&height);
}
 
crypto::hash Blockchain::get_tail_id() const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
   
   
   
  return m_db->top_block_hash();
}
 
 
 
 
 
 
 
 
 
 
 
 
 
bool Blockchain::get_short_chain_history(std::list<crypto::hash>& ids) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  uint64_t i = 0;
  uint64_t current_multiplier = 1;
  uint64_t sz = m_db->height();

  if(!sz)
    return true;

  db_rtxn_guard rtxn_guard(m_db);
  bool genesis_included = false;
  uint64_t current_back_offset = 1;
  while(current_back_offset < sz)
  {
    ids.push_back(m_db->get_block_hash_from_height(sz - current_back_offset));

    if(sz-current_back_offset == 0)
    {
      genesis_included = true;
    }
    if(i < 10)
    {
      ++current_back_offset;
    }
    else
    {
      current_multiplier *= 2;
      current_back_offset += current_multiplier;
    }
    ++i;
  }

  if (!genesis_included)
  {
    ids.push_back(m_db->get_block_hash_from_height(0));
  }

  return true;
}
 
crypto::hash Blockchain::get_block_id_by_height(uint64_t height) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
   
   
   
  try
  {
    return m_db->get_block_hash_from_height(height);
  }
  catch (const BLOCK_DNE& e)
  {
  }
  catch (const std::exception& e)
  {
    MERROR(std::string("Something went wrong fetching block hash by height: ") + e.what());
    throw;
  }
  catch (...)
  {
    MERROR(std::string("Something went wrong fetching block hash by height"));
    throw;
  }
  return null_hash;
}
 
crypto::hash Blockchain::get_pending_block_id_by_height(uint64_t height) const
{
  if (m_prepare_height && height >= m_prepare_height && height - m_prepare_height < m_prepare_nblocks)
    return (*m_prepare_blocks)[height - m_prepare_height].hash;
  return get_block_id_by_height(height);
}
 
bool Blockchain::get_block_by_hash(const crypto::hash &h, block &blk, bool *orphan) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

   
  try
  {
    blk = m_db->get_block(h);
    if (orphan)
      *orphan = false;
    return true;
  }
   
  catch (const BLOCK_DNE& e)
  {
    alt_block_data_t data;
    cryptonote::blobdata blob;
    if (m_db->get_alt_block(h, &data, &blob))
    {
      if (!cryptonote::parse_and_validate_block_from_blob(blob, blk))
      {
        MERROR("Found block " << h << " in alt chain, but failed to parse it");
        throw std::runtime_error("Found block in alt chain, but failed to parse it");
      }
      if (orphan)
        *orphan = true;
      return true;
    }
  }
  catch (const std::exception& e)
  {
    MERROR(std::string("Something went wrong fetching block by hash: ") + e.what());
    throw;
  }
  catch (...)
  {
    MERROR(std::string("Something went wrong fetching block hash by hash"));
    throw;
  }

  return false;
}
 
 
 
 
 
difficulty_type Blockchain::get_difficulty_for_next_block()
{
  LOG_PRINT_L3("Blockchain::" << __func__);

  std::stringstream ss;
  bool print = false;

  int done = 0;
  ss << "get_difficulty_for_next_block: height " << m_db->height() << std::endl;
  if (m_fixed_difficulty)
  {
    return m_db->height() ? m_fixed_difficulty : 1;
  }

start:
  difficulty_type D = 0;

  crypto::hash top_hash = get_tail_id();
  {
    CRITICAL_REGION_LOCAL(m_difficulty_lock);
     
     
     
     
    ss << "Locked, tail id " << top_hash << ", cached is " << m_difficulty_for_next_block_top_hash << std::endl;
    if (top_hash == m_difficulty_for_next_block_top_hash)
    {
      ss << "Same, using cached diff " << m_difficulty_for_next_block << std::endl;
      D = m_difficulty_for_next_block;
    }
  }

  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  std::vector<uint64_t> timestamps;
  std::vector<difficulty_type> difficulties;
  uint64_t height;
  auto new_top_hash = get_tail_id(height);  
  ++height;
  if (!(new_top_hash == top_hash)) D=0;
  ss << "Re-locked, height " << height << ", tail id " << new_top_hash << (new_top_hash == top_hash ? "" : " (different)") << std::endl;
  top_hash = new_top_hash;

   
   
   
   
   
  bool check = false;
  if (m_reset_timestamps_and_difficulties_height)
    m_timestamps_and_difficulties_height = 0;
  if (m_timestamps_and_difficulties_height != 0 && ((height - m_timestamps_and_difficulties_height) == 1) && m_timestamps.size() >= DIFFICULTY_BLOCKS_COUNT_V2)
  {
    uint64_t index = height - 1;
    m_timestamps.push_back(m_db->get_block_timestamp(index));
    m_difficulties.push_back(m_db->get_block_cumulative_difficulty(index));

    while (m_timestamps.size() > DIFFICULTY_BLOCKS_COUNT_V2)
      m_timestamps.erase(m_timestamps.begin());
    while (m_difficulties.size() > DIFFICULTY_BLOCKS_COUNT_V2)
      m_difficulties.erase(m_difficulties.begin());

    m_timestamps_and_difficulties_height = height;
    timestamps = m_timestamps;
    difficulties = m_difficulties;
    check = true;
  }
   
  std::vector<uint64_t> timestamps_from_cache = timestamps;
  std::vector<difficulty_type> difficulties_from_cache = difficulties;

  {
    uint64_t offset = height - std::min <uint64_t> (height, static_cast<uint64_t>(DIFFICULTY_BLOCKS_COUNT_V2));
    if (offset == 0)
      ++offset;

    timestamps.clear();
    difficulties.clear();
    if (height > offset)
    {
      timestamps.reserve(height - offset);
      difficulties.reserve(height - offset);
    }
    ss << "Looking up " << (height - offset) << " from " << offset << std::endl;
    for (; offset < height; offset++)
    {
      timestamps.push_back(m_db->get_block_timestamp(offset));
      difficulties.push_back(m_db->get_block_cumulative_difficulty(offset));
    }

    if (check) if (timestamps != timestamps_from_cache || difficulties !=difficulties_from_cache)
    {
      ss << "Inconsistency XXX:" << std::endl;
      ss << "top hash: "<<top_hash << std::endl;
      ss << "timestamps: " << timestamps_from_cache.size() << " from cache, but " << timestamps.size() << " without" << std::endl;
      ss << "difficulties: " << difficulties_from_cache.size() << " from cache, but " << difficulties.size() << " without" << std::endl;
      ss << "timestamps_from_cache:" << std::endl; for (const auto &v :timestamps_from_cache) ss << "  " << v << std::endl;
      ss << "timestamps:" << std::endl; for (const auto &v :timestamps) ss << "  " << v << std::endl;
      ss << "difficulties_from_cache:" << std::endl; for (const auto &v :difficulties_from_cache) ss << "  " << v << std::endl;
      ss << "difficulties:" << std::endl; for (const auto &v :difficulties) ss << "  " << v << std::endl;

      uint64_t dbh = m_db->height();
      uint64_t sh = dbh < 10000 ? 0 : dbh - 10000;
      ss << "History from -10k at :" << dbh << ", from " << sh << std::endl;
      for (uint64_t h = sh; h < dbh; ++h)
      {
        uint64_t ts = m_db->get_block_timestamp(h);
        difficulty_type d = m_db->get_block_cumulative_difficulty(h);
        ss << "  " << h << " " << ts << " " << d << std::endl;
      }
      print = true;
    }
    m_timestamps_and_difficulties_height = height;
    m_timestamps = timestamps;
    m_difficulties = difficulties;
  }

  size_t target = get_difficulty_target();
  difficulty_type diff = next_difficulty_v2(timestamps, difficulties, target);

  CRITICAL_REGION_LOCAL1(m_difficulty_lock);
  m_difficulty_for_next_block_top_hash = top_hash;
  m_difficulty_for_next_block = diff;
  if (D && D != diff)
  {
    ss << "XXX Mismatch at " << height << "/" << top_hash << "/" << get_tail_id() << ": cached " << D << ", real " << diff << std::endl;
    print = true;
  }

  ++done;
  if (done == 1 && D && D != diff)
  {
    print = true;
    ss << "Might be a race. Let's see what happens if we try again..." << std::endl;
    epee::misc_utils::sleep_no_w(100);
    goto start;
  }
  ss << "Diff for " << top_hash << ": " << diff << std::endl;
  if (print)
  {
    MGINFO("START DUMP");
    MGINFO(ss.str());
    MGINFO("END DUMP");
    MGINFO("Please send Venera support the contents of this log, from a couple dozen lines before START DUMP to END DUMP");
  }
  return diff;
}
 
std::pair<bool, uint64_t> Blockchain::check_difficulty_checkpoints() const
{
  uint64_t res = 0;
  for (const std::pair<const uint64_t, difficulty_type>& i : m_checkpoints.get_difficulty_points())
  {
    if (i.first >= m_db->height())
      break;
    if (m_db->get_block_cumulative_difficulty(i.first) != i.second)
      return {false, res};
    res = i.first;
  }
  return {true, res};
}
 
size_t Blockchain::recalculate_difficulties(boost::optional<uint64_t> start_height_opt)
{
  if (m_fixed_difficulty)
  {
    return 0;
  }
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  const uint64_t start_height = start_height_opt ? *start_height_opt : check_difficulty_checkpoints().second;
  const uint64_t top_height = m_db->height() - 1;
  MGINFO("Recalculating difficulties from height " << start_height << " to height " << top_height);

  std::vector<uint64_t> timestamps;
  std::vector<difficulty_type> difficulties;
  timestamps.reserve(DIFFICULTY_BLOCKS_COUNT_V2 + 1);
  difficulties.reserve(DIFFICULTY_BLOCKS_COUNT_V2 + 1);
  if (start_height > 1)
  {
    for (uint64_t i = 0; i < DIFFICULTY_BLOCKS_COUNT_V2; ++i)
    {
      uint64_t height = start_height - 1 - i;
      if (height == 0)
        break;
      timestamps.insert(timestamps.begin(), m_db->get_block_timestamp(height));
      difficulties.insert(difficulties.begin(), m_db->get_block_cumulative_difficulty(height));
    }
  }
  difficulty_type last_cum_diff = start_height <= 1 ? start_height : difficulties.back();
  uint64_t drift_start_height = 0;
  std::vector<difficulty_type> new_cumulative_difficulties;
  for (uint64_t height = start_height; height <= top_height; ++height)
  {
    size_t target = DIFFICULTY_TARGET_V2;
    difficulty_type recalculated_diff = next_difficulty_v2(timestamps, difficulties, target);

    boost::multiprecision::uint256_t recalculated_cum_diff_256 = boost::multiprecision::uint256_t(recalculated_diff) + last_cum_diff;
    CHECK_AND_ASSERT_THROW_MES(recalculated_cum_diff_256 <= std::numeric_limits<difficulty_type>::max(), "Difficulty overflow!");
    difficulty_type recalculated_cum_diff = recalculated_cum_diff_256.convert_to<difficulty_type>();

    if (drift_start_height == 0)
    {
      difficulty_type existing_cum_diff = m_db->get_block_cumulative_difficulty(height);
      if (recalculated_cum_diff != existing_cum_diff)
      {
        drift_start_height = height;
        new_cumulative_difficulties.reserve(top_height + 1 - height);
        LOG_ERROR("Difficulty drift found at height:" << height << ", hash:" << m_db->get_block_hash_from_height(height) << ", existing:" << existing_cum_diff << ", recalculated:" << recalculated_cum_diff);
      }
    }
    if (drift_start_height > 0)
    {
      new_cumulative_difficulties.push_back(recalculated_cum_diff);
      if (height % 100000 == 0)
        LOG_ERROR(boost::format("%llu / %llu (%.1f%%)") % height % top_height % (100 * (height - drift_start_height) / float(top_height - drift_start_height)));
    }

    if (height > 0)
    {
      timestamps.push_back(m_db->get_block_timestamp(height));
      difficulties.push_back(recalculated_cum_diff);
    }
    if (timestamps.size() > DIFFICULTY_BLOCKS_COUNT_V2)
    {
      CHECK_AND_ASSERT_THROW_MES(timestamps.size() == DIFFICULTY_BLOCKS_COUNT_V2 + 1, "Wrong timestamps size: " << timestamps.size());
      timestamps.erase(timestamps.begin());
      difficulties.erase(difficulties.begin());
    }
    last_cum_diff = recalculated_cum_diff;
  }

  if (drift_start_height > 0)
  {
    LOG_ERROR("Writing to the DB...");
    try
    {
      m_db->correct_block_cumulative_difficulties(drift_start_height, new_cumulative_difficulties);
    }
    catch (const std::exception& e)
    {
      LOG_ERROR("Error correcting cumulative difficulties from height " << drift_start_height << ", what = " << e.what());
    }
    LOG_ERROR("Corrected difficulties for " << new_cumulative_difficulties.size() << " blocks");
     
    m_difficulty_for_next_block_top_hash = crypto::null_hash;
    m_timestamps_and_difficulties_height = 0;
  }

  return new_cumulative_difficulties.size();
}
 
std::vector<time_t> Blockchain::get_last_block_timestamps(unsigned int blocks) const
{
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  uint64_t height = m_db->height();
  if (blocks > height)
    blocks = height;
  std::vector<time_t> timestamps(blocks);
  while (blocks--)
    timestamps[blocks] = m_db->get_block_timestamp(height - blocks - 1);
  return timestamps;
}
 
 
 
 
bool Blockchain::rollback_blockchain_switching(std::list<block>& original_chain, uint64_t rollback_height)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

   
  if (rollback_height > m_db->height())
  {
    return true;
  }

  m_timestamps_and_difficulties_height = 0;
  m_reset_timestamps_and_difficulties_height = true;

   
  while (m_db->height() != rollback_height)
  {
    pop_block_from_blockchain();
  }
  CHECK_AND_ASSERT_THROW_MES(update_next_cumulative_weight_limit(), "Error updating next cumulative weight limit");

   
  m_hardfork->reorganize_from_chain_height(rollback_height);

   
  for (auto& bl : original_chain)
  {
    block_verification_context bvc = {};
    bool r = handle_block_to_main_chain(bl, bvc, false);
    CHECK_AND_ASSERT_MES(r && bvc.m_added_to_main_chain, false, "PANIC! failed to add (again) block while chain switching during the rollback!");
  }

  m_hardfork->reorganize_from_chain_height(rollback_height);

  MINFO("Rollback to height " << rollback_height << " was successful.");
  if (!original_chain.empty())
  {
    MINFO("Restoration to previous blockchain successful as well.");
  }
  return true;
}
 
 
 
bool Blockchain::switch_to_alternative_blockchain(std::list<block_extended_info>& alt_chain, bool discard_disconnected_chain)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  m_timestamps_and_difficulties_height = 0;
  m_reset_timestamps_and_difficulties_height = true;

   
  CHECK_AND_ASSERT_MES(alt_chain.size(), false, "switch_to_alternative_blockchain: empty chain passed");
  CHECK_AND_ASSERT_MES(m_db->height() <= BLOCK_APPROVAL_HEIGHT || alt_chain.front().height >= m_db->height(),
      false, "Cannot reorganize finalized approved history");

   
  if (!m_db->block_exists(alt_chain.front().bl.prev_id))
  {
    LOG_ERROR("Attempting to move to an alternate chain, but it doesn't appear to connect to the main chain!");
    return false;
  }

   
   
  std::list<block> disconnected_chain;
  while (m_db->top_block_hash() != alt_chain.front().bl.prev_id)
  {
    block b = pop_block_from_blockchain();
    disconnected_chain.push_front(b);
  }
  CHECK_AND_ASSERT_THROW_MES(update_next_cumulative_weight_limit(), "Error updating next cumulative weight limit");

  auto split_height = m_db->height();

   
  for(auto alt_ch_iter = alt_chain.begin(); alt_ch_iter != alt_chain.end(); alt_ch_iter++)
  {
    const auto &bei = *alt_ch_iter;
    block_verification_context bvc = {};

     
    bool r = handle_block_to_main_chain(bei.bl, bvc, false);

     
     
    if(!r || !bvc.m_added_to_main_chain)
    {
      MERROR("Failed to switch to alternative blockchain");

       
       
       
      rollback_blockchain_switching(disconnected_chain, split_height);

       
       
       
      const crypto::hash blkid = cryptonote::get_block_hash(bei.bl);
      add_block_as_invalid(bei, blkid);
      MERROR("The block was inserted as invalid while connecting new alternative chain, block_id: " << blkid);
      m_db->remove_alt_block(blkid);
      alt_ch_iter++;

      for(auto alt_ch_to_orph_iter = alt_ch_iter; alt_ch_to_orph_iter != alt_chain.end(); )
      {
        const auto &bei = *alt_ch_to_orph_iter++;
        const crypto::hash blkid = cryptonote::get_block_hash(bei.bl);
        add_block_as_invalid(bei, blkid);
        m_db->remove_alt_block(blkid);
      }
      return false;
    }
  }

   
  const size_t discarded_blocks = disconnected_chain.size();
  if(!discard_disconnected_chain)
  {
     
    for (auto& old_ch_ent : disconnected_chain)
    {
      block_verification_context bvc = {};
      bool r = handle_alternative_block(old_ch_ent, get_block_hash(old_ch_ent), bvc);
      if(!r)
      {
        MERROR("Failed to push ex-main chain blocks to alternative chain ");
         
         
      }
    }
  }

   
  for (const auto &bei: alt_chain)
  {
    m_db->remove_alt_block(cryptonote::get_block_hash(bei.bl));
  }

  m_hardfork->reorganize_from_chain_height(split_height);

  std::shared_ptr<tools::Notify> reorg_notify = m_reorg_notify;
  if (reorg_notify)
    reorg_notify->notify("%s", std::to_string(split_height).c_str(), "%h", std::to_string(m_db->height()).c_str(),
        "%n", std::to_string(m_db->height() - split_height).c_str(), "%d", std::to_string(discarded_blocks).c_str(), NULL);

  const uint64_t new_height = m_db->height();
  const crypto::hash seedhash = get_block_id_by_height(crypto::rx_seedheight(new_height));

  crypto::hash prev_id;
  if (!get_block_hash(alt_chain.back().bl, prev_id))
    MERROR("Failed to get block hash of an alternative chain's tip");
  else
    send_miner_notifications(new_height, seedhash, prev_id, m_db->get_block_already_generated_coins(new_height - 1));

  for (const auto& notifier : m_block_notifiers)
  {
    std::size_t notify_height = split_height;
    for (const auto& bei: alt_chain)
    {
      notifier(notify_height, {std::addressof(bei.bl), 1});
      ++notify_height;
    }
  }

  rx_set_main_seedhash(seedhash.data, tools::get_max_concurrency());

  MGINFO_GREEN("REORGANIZE SUCCESS! on height: " << split_height << ", new blockchain size: " << m_db->height());
  return true;
}
 
 
 
difficulty_type Blockchain::get_next_difficulty_for_alternative_chain(const std::list<block_extended_info>& alt_chain, block_extended_info& bei) const
{
  if (m_fixed_difficulty)
  {
    return m_db->height() ? m_fixed_difficulty : 1;
  }

  LOG_PRINT_L3("Blockchain::" << __func__);
  std::vector<uint64_t> timestamps;
  std::vector<difficulty_type> cumulative_difficulties;

   
   
  if(alt_chain.size()< DIFFICULTY_BLOCKS_COUNT_V2)
  {
    CRITICAL_REGION_LOCAL(m_blockchain_lock);

     
    size_t main_chain_stop_offset = alt_chain.size() ? alt_chain.front().height : bei.height;
    size_t main_chain_count = DIFFICULTY_BLOCKS_COUNT_V2 - std::min(static_cast<size_t>(DIFFICULTY_BLOCKS_COUNT_V2), alt_chain.size());
    main_chain_count = std::min(main_chain_count, main_chain_stop_offset);
    size_t main_chain_start_offset = main_chain_stop_offset - main_chain_count;

    if(!main_chain_start_offset)
      ++main_chain_start_offset;  

     
    for(; main_chain_start_offset < main_chain_stop_offset; ++main_chain_start_offset)
    {
      timestamps.push_back(m_db->get_block_timestamp(main_chain_start_offset));
      cumulative_difficulties.push_back(m_db->get_block_cumulative_difficulty(main_chain_start_offset));
    }

     
    CHECK_AND_ASSERT_MES((alt_chain.size() + timestamps.size()) <= DIFFICULTY_BLOCKS_COUNT_V2, false, "Internal error, alt_chain.size()[" << alt_chain.size() << "] + vtimestampsec.size()[" << timestamps.size() << "] NOT <= DIFFICULTY_WINDOW[]" << DIFFICULTY_BLOCKS_COUNT_V2);

    for (const auto &bei : alt_chain)
    {
      timestamps.push_back(bei.bl.timestamp);
      cumulative_difficulties.push_back(bei.cumulative_difficulty);
    }
  }
   
   
  else
  {
    timestamps.resize(static_cast<size_t>(DIFFICULTY_BLOCKS_COUNT_V2));
    cumulative_difficulties.resize(static_cast<size_t>(DIFFICULTY_BLOCKS_COUNT_V2));
    size_t count = 0;
    size_t max_i = timestamps.size()-1;
     
    for (const auto &bei: boost::adaptors::reverse(alt_chain))
    {
      timestamps[max_i - count] = bei.bl.timestamp;
      cumulative_difficulties[max_i - count] = bei.cumulative_difficulty;
      count++;
      if(count >= DIFFICULTY_BLOCKS_COUNT_V2)
        break;
    }
  }

   
  size_t target = DIFFICULTY_TARGET_V2;

   
  return next_difficulty_v2(timestamps, cumulative_difficulties, target);
}
 
 
 
 
 
 
 
bool Blockchain::prevalidate_miner_transaction(const block& b, uint64_t height, uint8_t hf_version)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CHECK_AND_ASSERT_MES(b.miner_tx.vin.size() == 1, false, "coinbase transaction in the block has no inputs");
  CHECK_AND_ASSERT_MES(b.miner_tx.vin[0].type() == typeid(txin_gen), false, "coinbase transaction in the block has the wrong type");
  CHECK_AND_ASSERT_MES(b.miner_tx.version > 1, false, "Invalid coinbase transaction version");

   
  if (hf_version >= HF_VERSION_REJECT_SIGS_IN_COINBASE && b.miner_tx.version >= 2)
  {
    CHECK_AND_ASSERT_MES(b.miner_tx.rct_signatures.type == rct::RCTTypeNull, false, "RingCT signatures not allowed in coinbase transactions");
  }

  if(boost::get<txin_gen>(b.miner_tx.vin[0]).height != height)
  {
    MWARNING("The miner transaction in block has invalid height: " << boost::get<txin_gen>(b.miner_tx.vin[0]).height << ", expected: " << height);
    return false;
  }
  MDEBUG("Miner tx hash: " << get_transaction_hash(b.miner_tx));
  CHECK_AND_ASSERT_MES(b.miner_tx.unlock_time == height + CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW, false, "coinbase transaction transaction has the wrong unlock time=" << b.miner_tx.unlock_time << ", expected " << height + CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW);

  if (height >= VENERA_REWARD_BURN_HEIGHT)
  {
    CHECK_AND_ASSERT_MES(b.miner_tx.version == 3 &&
        b.miner_tx.type == transaction_type::BURN && b.miner_tx.vout.size() == 1 &&
        b.miner_tx.rct_signatures.type == rct::RCTTypeNull,
        false, "Invalid mandatory coinbase burn format");
  }
  else
    CHECK_AND_ASSERT_MES(b.miner_tx.version == 2 &&
        b.miner_tx.type == transaction_type::UNSET && b.miner_tx.amount_burnt == 0,
        false, "Premature coinbase burn");

   
  if(!check_outs_overflow(b.miner_tx))
  {
    MERROR("miner transaction has money overflow in block " << get_block_hash(b));
    return false;
  }

  CHECK_AND_ASSERT_MES(check_output_types(b.miner_tx, hf_version), false, "miner transaction has invalid output type(s) in block " << get_block_hash(b));

  return true;
}
 
 
bool Blockchain::validate_miner_transaction(const block& b, size_t cumulative_block_weight, uint64_t fee, uint64_t& base_reward, uint64_t already_generated_coins, bool &partial_block_reward, uint8_t version)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
  uint64_t money_in_use = 0;
  for (auto& o: b.miner_tx.vout)
    money_in_use += o.amount;
  partial_block_reward = false;

  CHECK_AND_ASSERT_MES(check_miner_output_amount_format(b.miner_tx,
      boost::get<txin_gen>(b.miner_tx.vin[0]).height, version),
      false, "Invalid historical decomposed coinbase amount");

  uint64_t median_weight;
  if (version >= 1)
  {
    median_weight = m_current_block_cumul_weight_median;
  }
  else
  {
    std::vector<uint64_t> last_blocks_weights;
    get_last_n_blocks_weights(last_blocks_weights, CRYPTONOTE_REWARD_BLOCKS_WINDOW);
    median_weight = epee::misc_utils::median(last_blocks_weights);
  }
  if (!get_block_reward(median_weight, cumulative_block_weight, already_generated_coins, base_reward, version,
      boost::get<txin_gen>(b.miner_tx.vin[0]).height))
  {
    MERROR_VER("block weight " << cumulative_block_weight << " is bigger than allowed for this blockchain");
    return false;
  }
  CHECK_AND_ASSERT_MES(check_miner_reward_burn(b.miner_tx,
      boost::get<txin_gen>(b.miner_tx.vin[0]).height, base_reward, fee),
      false, "Incorrect coinbase payout or mandatory burn");
  return true;
}
 
 
void Blockchain::get_last_n_blocks_weights(std::vector<uint64_t>& weights, size_t count) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  auto h = m_db->height();

   
  if(h == 0)
    return;

   
  size_t start_offset = h - std::min<size_t>(h, count);
  weights = m_db->get_block_weights(start_offset, count);
}
 
uint64_t Blockchain::get_long_term_block_weight_median(uint64_t start_height, size_t count) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  PERF_TIMER(get_long_term_block_weights);

  CHECK_AND_ASSERT_THROW_MES(count > 0, "count == 0");

  bool cached = false;
  uint64_t blockchain_height = m_db->height();
  uint64_t tip_height = start_height + count - 1;
  crypto::hash tip_hash = crypto::null_hash;
  if (tip_height < blockchain_height && count == (size_t)m_long_term_block_weights_cache_rolling_median.size())
  {
    tip_hash = m_db->get_block_hash_from_height(tip_height);
    cached = tip_hash == m_long_term_block_weights_cache_tip_hash;
  }

  if (cached)
  {
    MTRACE("requesting " << count << " from " << start_height << ", cached");
    return m_long_term_block_weights_cache_rolling_median.median();
  }

   
   
  if (tip_height > 0 && count == (size_t)m_long_term_block_weights_cache_rolling_median.size() && tip_height < blockchain_height)
  {
    crypto::hash old_tip_hash = m_db->get_block_hash_from_height(tip_height - 1);
    if (old_tip_hash == m_long_term_block_weights_cache_tip_hash)
    {
      MTRACE("requesting " << count << " from " << start_height << ", incremental");
      m_long_term_block_weights_cache_tip_hash = tip_hash;
      m_long_term_block_weights_cache_rolling_median.insert(m_db->get_block_long_term_weight(tip_height));
      return m_long_term_block_weights_cache_rolling_median.median();
    }
  }

  MTRACE("requesting " << count << " from " << start_height << ", uncached");
  std::vector<uint64_t> weights = m_db->get_long_term_block_weights(start_height, count);
  m_long_term_block_weights_cache_tip_hash = tip_hash;
  m_long_term_block_weights_cache_rolling_median.clear();
  for (uint64_t w: weights)
    m_long_term_block_weights_cache_rolling_median.insert(w);
  return m_long_term_block_weights_cache_rolling_median.median();
}
 
uint64_t Blockchain::get_current_cumulative_block_weight_limit() const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  return m_current_block_cumul_weight_limit;
}
 
uint64_t Blockchain::get_current_cumulative_block_weight_median() const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  return m_current_block_cumul_weight_median;
}
 
 
 
 
 
 
 
 
 
 
 
 
bool Blockchain::create_block_template(block& b, const crypto::hash *from_block, const account_public_address& miner_address, difficulty_type& diffic, uint64_t& height, uint64_t& expected_reward, const blobdata& ex_nonce, uint64_t &seed_height, crypto::hash &seed_hash, bool include_sensitive)
{
  return false;  
  }
 
bool Blockchain::create_block_template(block& b, const account_public_address& miner_address, difficulty_type& diffic, uint64_t& height, uint64_t& expected_reward, const blobdata& ex_nonce, uint64_t &seed_height, crypto::hash &seed_hash)
{
  return false;  
  }
 
bool Blockchain::get_miner_data(uint8_t& major_version, uint64_t& height, crypto::hash& prev_id, crypto::hash& seed_hash, difficulty_type& difficulty, uint64_t& median_weight, uint64_t& already_generated_coins, std::vector<tx_block_template_backlog_entry>& tx_backlog)
{
  return false;  
  }
 
 
 
bool Blockchain::complete_timestamps_vector(uint64_t start_top_height, std::vector<uint64_t>& timestamps) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);

  if(timestamps.size() >= BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW)
    return true;

  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  size_t need_elements = BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW - timestamps.size();
  CHECK_AND_ASSERT_MES(start_top_height < m_db->height(), false, "internal error: passed start_height not < " << " m_db->height() -- " << start_top_height << " >= " << m_db->height());
  size_t stop_offset = start_top_height > need_elements ? start_top_height - need_elements : 0;
  timestamps.reserve(timestamps.size() + start_top_height - stop_offset);
  while (start_top_height != stop_offset)
  {
    timestamps.push_back(m_db->get_block_timestamp(start_top_height));
    --start_top_height;
  }
  return true;
}
 
bool Blockchain::build_alt_chain(const crypto::hash &prev_id, std::list<block_extended_info>& alt_chain, std::vector<uint64_t> &timestamps, block_verification_context& bvc) const
{
     
    cryptonote::alt_block_data_t data;
    cryptonote::blobdata blob;
    bool found = m_db->get_alt_block(prev_id, &data, &blob);
    timestamps.clear();
    while(found)
    {
      block_extended_info bei;
      CHECK_AND_ASSERT_MES(cryptonote::parse_and_validate_block_from_blob(blob, bei.bl), false, "Failed to parse alt block");
      bei.height = data.height;
      bei.block_cumulative_weight = data.cumulative_weight;
      bei.cumulative_difficulty = data.cumulative_difficulty_high;
      bei.cumulative_difficulty = (bei.cumulative_difficulty << 64) + data.cumulative_difficulty_low;
      bei.already_generated_coins = data.already_generated_coins;
      timestamps.push_back(bei.bl.timestamp);
      alt_chain.push_front(std::move(bei));
      found = m_db->get_alt_block(bei.bl.prev_id, &data, &blob);
    }

     
     
    if(!alt_chain.empty())
    {
       
      CHECK_AND_ASSERT_MES(m_db->height() > alt_chain.front().height, false, "main blockchain wrong height");

       
       
      if (!m_db->block_exists(alt_chain.front().bl.prev_id))
      {
        MERROR("alternate chain does not appear to connect to main chain...");
        return false;
      }

       
      auto h = m_db->get_block_hash_from_height(alt_chain.front().height - 1);
      CHECK_AND_ASSERT_MES(h == alt_chain.front().bl.prev_id, false, "alternative chain has wrong connection to main chain");
      complete_timestamps_vector(m_db->get_block_height(alt_chain.front().bl.prev_id), timestamps);
    }
     
    else
    {
       
       
      bool parent_in_main = m_db->block_exists(prev_id);
      CHECK_AND_ASSERT_MES(parent_in_main, false, "internal error: broken imperative condition: parent_in_main");

      complete_timestamps_vector(m_db->get_block_height(prev_id), timestamps);
    }

    return true;
}
 
 
 
 
 
 
 
bool Blockchain::handle_alternative_block(const block& b, const crypto::hash& id, block_verification_context& bvc)
{
  if (!check_block_approval(b, m_nettype)) { bvc.m_verifivation_failed = true; return false; }
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  m_timestamps_and_difficulties_height = 0;
  m_reset_timestamps_and_difficulties_height = true;
  uint64_t block_height = get_block_height(b);
  if(0 == block_height)
  {
    MERROR_VER("Block with id: " << epee::string_tools::pod_to_hex(id) << " (as alternative), but miner tx says height is 0.");
    bvc.m_verifivation_failed = true;
    return false;
  }
   
   
   
   
  if (!m_checkpoints.is_alternative_block_allowed(get_current_blockchain_height(), block_height))
  {
    MERROR_VER("Block with id: " << id << std::endl << " can't be accepted for alternative chain, block height: " << block_height << std::endl << " blockchain height: " << get_current_blockchain_height());
    bvc.m_verifivation_failed = true;
    return false;
  }

   
  const uint8_t hf_version = m_hardfork->get_ideal_version(block_height);
  if (!m_hardfork->check_for_height(b, block_height))
  {
    LOG_PRINT_L1("Block with id: " << id << std::endl << "has old version for height " << block_height);
    bvc.m_verifivation_failed = true;
    return false;
  }

   
   
  alt_block_data_t prev_data;
  bool parent_in_alt = m_db->get_alt_block(b.prev_id, &prev_data, NULL);
  bool parent_in_main = m_db->block_exists(b.prev_id);
  if (parent_in_alt || parent_in_main)
  {
     
    std::list<block_extended_info> alt_chain;
    std::vector<uint64_t> timestamps;
    if (!build_alt_chain(b.prev_id, alt_chain, timestamps, bvc))
      return false;

     
    block_extended_info bei = {};
    bei.bl = b;
    const uint64_t prev_height = alt_chain.size() ? prev_data.height : m_db->get_block_height(b.prev_id);
    bei.height = prev_height + 1;
    const uint64_t split_height = alt_chain.empty() ? bei.height : alt_chain.front().height;
     
     
    if (m_db->height() > BLOCK_APPROVAL_HEIGHT && split_height < m_db->height()) {
      bvc.m_verifivation_failed = true;
      MERROR("Alternative branch would replace finalized approved history");
      return false;
    }
     
     
     
    bei.already_generated_coins = 0;

     
     
    if(!check_block_timestamp(timestamps, b))
    {
      MERROR_VER("Block with id: " << id << std::endl << " for alternative chain, has invalid timestamp: " << b.timestamp);
      bvc.m_verifivation_failed = true;
      return false;
    }

    bool is_a_checkpoint;
    if(!m_checkpoints.check_block(bei.height, id, is_a_checkpoint))
    {
      LOG_ERROR("CHECKPOINT VALIDATION FAILED");
      bvc.m_verifivation_failed = true;
      return false;
    }

     
    difficulty_type current_diff = get_next_difficulty_for_alternative_chain(alt_chain, bei);
    CHECK_AND_ASSERT_MES(current_diff, false, "!!!!!!! DIFFICULTY OVERHEAD !!!!!!!");
    crypto::hash proof_of_work;
    memset(proof_of_work.data, 0xff, sizeof(proof_of_work.data));
      crypto::hash seedhash = null_hash;
      uint64_t seedheight = rx_seedheight(bei.height);
       
      if (alt_chain.size() && alt_chain.front().height <= seedheight)
      {
        for (auto it=alt_chain.begin(); it != alt_chain.end(); it++)
        {
          if (it->height == seedheight+1)
          {
            seedhash = it->bl.prev_id;
            break;
          }
        }
      } else
      {
        seedhash = get_block_id_by_height(seedheight);
      }
      get_altblock_longhash(bei.bl, proof_of_work, seedhash);
      get_block_longhash(this, bei.bl, proof_of_work, bei.height, 0);
    if(!check_hash(proof_of_work, current_diff))
    {
      MERROR_VER("Block with id: " << id << std::endl << " for alternative chain, does not have enough proof of work: " << proof_of_work << std::endl << " expected difficulty: " << current_diff);
      bvc.m_verifivation_failed = true;
      bvc.m_bad_pow = true;
      return false;
    }

    if(!prevalidate_miner_transaction(b, bei.height, hf_version))
    {
      MERROR_VER("Block with id: " << epee::string_tools::pod_to_hex(id) << " (as alternative) has incorrect miner transaction.");
      bvc.m_verifivation_failed = true;
      return false;
    }

     
     
     
    difficulty_type main_chain_cumulative_difficulty = m_db->get_block_cumulative_difficulty(m_db->height() - 1);
    if (alt_chain.size())
    {
      bei.cumulative_difficulty = prev_data.cumulative_difficulty_high;
      bei.cumulative_difficulty = (bei.cumulative_difficulty << 64) + prev_data.cumulative_difficulty_low;
    }
    else
    {
       
      bei.cumulative_difficulty = m_db->get_block_cumulative_difficulty(m_db->get_block_height(b.prev_id));
    }
    bei.cumulative_difficulty += current_diff;

    bei.block_cumulative_weight = cryptonote::get_transaction_weight(b.miner_tx);
    for (const crypto::hash &txid: b.tx_hashes)
    {
      cryptonote::tx_memory_pool::tx_details td;
      cryptonote::blobdata blob;
      if (m_tx_pool.have_tx(txid, relay_category::legacy))
      {
        if (m_tx_pool.get_transaction_info(txid, td, true ))
        {
          bei.block_cumulative_weight += td.weight;
        }
        else
        {
          MERROR_VER("Transaction is in the txpool, but metadata not found");
          bvc.m_verifivation_failed = true;
          return false;
        }
      }
      else if (m_db->get_pruned_tx_blob(txid, blob))
      {
        cryptonote::transaction tx;
        if (!cryptonote::parse_and_validate_tx_base_from_blob(blob, tx))
        {
          MERROR_VER("Block with id: " << epee::string_tools::pod_to_hex(id) << " (as alternative) refers to unparsable transaction hash " << txid << ".");
          bvc.m_verifivation_failed = true;
          return false;
        }
        bei.block_cumulative_weight += cryptonote::get_pruned_transaction_weight(tx);

      }
      else
      {
         
        bei.block_cumulative_weight = 0;
        break;
      }
    }

     
     
    CHECK_AND_ASSERT_MES(!m_db->get_alt_block(id, NULL, NULL), false, "insertion of new alternative block returned as it already exists");
    cryptonote::alt_block_data_t data;
    data.height = bei.height;
    data.cumulative_weight = bei.block_cumulative_weight;
    data.cumulative_difficulty_low = (bei.cumulative_difficulty & 0xffffffffffffffff).convert_to<uint64_t>();
    data.cumulative_difficulty_high = ((bei.cumulative_difficulty >> 64) & 0xffffffffffffffff).convert_to<uint64_t>();
    data.already_generated_coins = bei.already_generated_coins;
    m_db->add_alt_block(id, data, cryptonote::block_to_blob(bei.bl));
    alt_chain.push_back(bei);

     
    if(is_a_checkpoint)
    {
       
      MGINFO_GREEN("###### REORGANIZE on height: " << alt_chain.front().height << " of " << m_db->height() - 1 << ", checkpoint is found in alternative chain on height " << bei.height);

      bool r = switch_to_alternative_blockchain(alt_chain, true);

      if(r) bvc.m_added_to_main_chain = true;
      else bvc.m_verifivation_failed = true;

      return r;
    }
    else if(main_chain_cumulative_difficulty < bei.cumulative_difficulty)  
    {
       
      MGINFO_GREEN("###### REORGANIZE on height: " << alt_chain.front().height << " of " << m_db->height() - 1 << " with cum_difficulty " << m_db->get_block_cumulative_difficulty(m_db->height() - 1) << std::endl << " alternative blockchain size: " << alt_chain.size() << " with cum_difficulty " << bei.cumulative_difficulty);

      bool r = switch_to_alternative_blockchain(alt_chain, false);
      if (r)
        bvc.m_added_to_main_chain = true;
      else
        bvc.m_verifivation_failed = true;
      return r;
    }
    else
    {
      MGINFO_BLUE("----- BLOCK ADDED AS ALTERNATIVE ON HEIGHT " << bei.height << std::endl << "id:\t" << id << std::endl << "PoW:\t" << proof_of_work << std::endl << "difficulty:\t" << current_diff);
      return true;
    }
  }
  else
  {
     
    bvc.m_marked_as_orphaned = true;
    MERROR_VER("Block recognized as orphaned and rejected, id = " << id << ", height " << block_height
        << ", parent in alt " << parent_in_alt << ", parent in main " << parent_in_main
        << " (parent " << b.prev_id << ", current top " << get_tail_id() << ", chain height " << get_current_blockchain_height() << ")");
  }

  return true;
}
 
bool Blockchain::get_blocks(uint64_t start_offset, size_t count, std::vector<std::pair<cryptonote::blobdata,block>>& blocks, std::vector<cryptonote::blobdata>& txs) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  if(start_offset >= m_db->height())
    return false;

  if (!get_blocks(start_offset, count, blocks))
  {
    return false;
  }

  for(const auto& blk : blocks)
  {
    std::vector<crypto::hash> missed_ids;
    get_transactions_blobs(blk.second.tx_hashes, txs, missed_ids);
    CHECK_AND_ASSERT_MES(!missed_ids.size(), false, "has missed transactions in own block in main blockchain");
  }

  return true;
}
 
bool Blockchain::get_blocks(uint64_t start_offset, size_t count, std::vector<std::pair<cryptonote::blobdata,block>>& blocks) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  const uint64_t height = m_db->height();
  if(start_offset >= height)
    return false;

  blocks.reserve(blocks.size() + height - start_offset);
  for(size_t i = start_offset; i < start_offset + count && i < height;i++)
  {
    blocks.push_back(std::make_pair(m_db->get_block_blob_from_height(i), block()));
    if (!parse_and_validate_block_from_blob(blocks.back().first, blocks.back().second))
    {
      LOG_ERROR("Invalid block");
      return false;
    }
  }
  return true;
}
 
 
 
 
 
 
 
 
bool Blockchain::handle_get_objects(NOTIFY_REQUEST_GET_OBJECTS::request& arg, NOTIFY_RESPONSE_GET_OBJECTS::request& rsp)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  db_rtxn_guard rtxn_guard (m_db);
  rsp.current_blockchain_height = get_current_blockchain_height();
  std::vector<std::pair<cryptonote::blobdata,block>> blocks;
  get_blocks(arg.blocks, blocks, rsp.missed_ids);

  for (size_t i = 0; i < blocks.size(); ++i)
  {
    auto& bl = blocks[i];
    std::vector<crypto::hash> missed_tx_ids;

    rsp.blocks.push_back(block_complete_entry());
    block_complete_entry& e = rsp.blocks.back();

     
     
    e.pruned = arg.prune;
    get_transactions_blobs(bl.second.tx_hashes, e.txs, missed_tx_ids, arg.prune);
    if (missed_tx_ids.size() != 0)
    {
       
      if (tools::has_unpruned_block(get_block_height(bl.second), get_current_blockchain_height(), get_blockchain_pruning_seed()))
      {
        LOG_ERROR("Error retrieving blocks, missed " << missed_tx_ids.size()
            << " transactions for block with hash: " << get_block_hash(bl.second)
            << std::endl
        );
      }

       
       
       
      rsp.missed_ids.insert(rsp.missed_ids.end(), missed_tx_ids.begin(), missed_tx_ids.end());
      return false;
    }

     
    e.block = std::move(bl.first);
    e.block_weight = 0;
    if (arg.prune && m_db->block_exists(arg.blocks[i]))
      e.block_weight = m_db->get_block_weight(m_db->get_block_height(arg.blocks[i]));
  }

  return true;
}
 
bool Blockchain::get_alternative_blocks(std::vector<block>& blocks) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  blocks.reserve(m_db->get_alt_block_count());
  m_db->for_all_alt_blocks([&blocks](const crypto::hash &blkid, const cryptonote::alt_block_data_t &data, const cryptonote::blobdata_ref *blob) {
    if (!blob)
    {
      MERROR("No blob, but blobs were requested");
      return false;
    }
    cryptonote::block bl;
    if (cryptonote::parse_and_validate_block_from_blob(*blob, bl))
      blocks.push_back(std::move(bl));
    else
      MERROR("Failed to parse block from blob");
    return true;
  }, true);
  return true;
}
 
size_t Blockchain::get_alternative_blocks_count() const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  return m_db->get_alt_block_count();
}
 
 
 
uint64_t Blockchain::get_num_mature_outputs(uint64_t amount) const
{
  uint64_t num_outs = m_db->get_num_outputs(amount);
   
   
  const uint64_t blockchain_height = m_db->height();
  while (num_outs > 0)
  {
    const tx_out_index toi = m_db->get_output_tx_and_index(amount, num_outs - 1);
    const uint64_t height = m_db->get_tx_block_height(toi.first);
    if (height + CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE <= blockchain_height)
      break;
    --num_outs;
  }

  return num_outs;
}

crypto::public_key Blockchain::get_output_key(uint64_t amount, uint64_t global_index) const
{
  output_data_t data = m_db->get_output_key(amount, global_index);
  return data.pubkey;
}

 
bool Blockchain::get_outs(const COMMAND_RPC_GET_OUTPUTS_BIN::request& req, COMMAND_RPC_GET_OUTPUTS_BIN::response& res) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  res.outs.clear();
  res.outs.reserve(req.outputs.size());

  std::vector<cryptonote::output_data_t> data;
  try
  {
    std::vector<uint64_t> amounts, offsets;
    amounts.reserve(req.outputs.size());
    offsets.reserve(req.outputs.size());
    for (const auto &i: req.outputs)
    {
      amounts.push_back(i.amount);
      offsets.push_back(i.index);
    }
    m_db->get_output_key(epee::span<const uint64_t>(amounts.data(), amounts.size()), offsets, data);
    if (data.size() != req.outputs.size())
    {
      MERROR("Unexpected output data size: expected " << req.outputs.size() << ", got " << data.size());
      return false;
    }
    const uint8_t hf_version = m_hardfork->get_current_version();
    for (const auto &t: data)
      res.outs.push_back({t.pubkey, t.commitment, is_tx_spendtime_unlocked(t.unlock_time, hf_version), t.height, crypto::null_hash});

    if (req.get_txid)
    {
      for (size_t i = 0; i < req.outputs.size(); ++i)
      {
        tx_out_index toi = m_db->get_output_tx_and_index(req.outputs[i].amount, req.outputs[i].index);
        res.outs[i].txid = toi.first;
      }
    }
  }
  catch (const std::exception &e)
  {
    return false;
  }
  return true;
}
 
void Blockchain::get_output_key_mask_unlocked(const uint64_t& amount, const uint64_t& index, crypto::public_key& key, rct::key& mask, bool& unlocked) const
{
  const auto o_data = m_db->get_output_key(amount, index);
  key = o_data.pubkey;
  mask = o_data.commitment;
  tx_out_index toi = m_db->get_output_tx_and_index(amount, index);
  const uint8_t hf_version = m_hardfork->get_current_version();
  unlocked = is_tx_spendtime_unlocked(m_db->get_tx_unlock_time(toi.first), hf_version);
}
 
bool Blockchain::get_output_distribution(uint64_t amount, uint64_t from_height, uint64_t to_height, uint64_t &start_height, std::vector<uint64_t> &distribution, uint64_t &base) const
{
   
    start_height = 0;
  base = 0;

  if (to_height > 0 && to_height < from_height)
    return false;

  if (from_height > start_height)
    start_height = from_height;

  distribution.clear();
  uint64_t db_height = m_db->height();
  if (db_height == 0)
    return false;
  if (start_height >= db_height || to_height >= db_height)
    return false;
  if (amount == 0)
  {
    std::vector<uint64_t> heights;
    heights.reserve(to_height + 1 - start_height);
    const uint64_t real_start_height = start_height > 0 ? start_height-1 : start_height;
    for (uint64_t h = real_start_height; h <= to_height; ++h)
      heights.push_back(h);
    distribution = m_db->get_block_cumulative_rct_outputs(heights);
    if (start_height > 0)
    {
      base = distribution[0];
      distribution.erase(distribution.begin());
    }
    return true;
  }
  else
  {
    return m_db->get_output_distribution(amount, start_height, to_height, distribution, base);
  }
}
 
 
 
 
bool Blockchain::find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, uint64_t& starter_offset) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

   
   
  if(qblock_ids.empty())
  {
    MCERROR("net.p2p", "Client sent wrong NOTIFY_REQUEST_CHAIN: m_block_ids.size()=" << qblock_ids.size() << ", dropping connection");
    return false;
  }

  db_rtxn_guard rtxn_guard(m_db);
   
   
  auto gen_hash = m_db->get_block_hash_from_height(0);
  if(qblock_ids.back() != gen_hash)
  {
    MCERROR("net.p2p", "Client sent wrong NOTIFY_REQUEST_CHAIN: genesis block mismatch: " << std::endl << "id: " << qblock_ids.back() << ", " << std::endl << "expected: " << gen_hash << "," << std::endl << " dropping connection");
    return false;
  }

   
   
  auto bl_it = qblock_ids.begin();
  uint64_t split_height = 0;
  for(; bl_it != qblock_ids.end(); bl_it++)
  {
    try
    {
      if (m_db->block_exists(*bl_it, &split_height))
        break;
    }
    catch (const std::exception& e)
    {
      MWARNING("Non-critical error trying to find block by hash in BlockchainDB, hash: " << *bl_it);
      return false;
    }
  }

   
   
  if(bl_it == qblock_ids.end())
  {
    MERROR("Internal error handling connection, can't find split point");
    return false;
  }

   
  starter_offset = split_height;
  return true;
}
 
difficulty_type Blockchain::block_difficulty(uint64_t i) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
   
   
   
  try
  {
    return m_db->get_block_difficulty(i);
  }
  catch (const BLOCK_DNE& e)
  {
    MERROR("Attempted to get block difficulty for height above blockchain height");
  }
  return 0;
}
 
template<typename T> void reserve_container(std::vector<T> &v, size_t N) { v.reserve(N); }
template<typename T> void reserve_container(std::list<T> &v, size_t N) { }
 
 
 
template<class t_ids_container, class t_blocks_container, class t_missed_container>
bool Blockchain::get_blocks(const t_ids_container& block_ids, t_blocks_container& blocks, t_missed_container& missed_bs) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  reserve_container(blocks, block_ids.size());
  for (const auto& block_hash : block_ids)
  {
    try
    {
      uint64_t height = 0;
      if (m_db->block_exists(block_hash, &height))
      {
        blocks.push_back(std::make_pair(m_db->get_block_blob_from_height(height), block()));
        if (!parse_and_validate_block_from_blob(blocks.back().first, blocks.back().second))
        {
          LOG_ERROR("Invalid block: " << block_hash);
          blocks.pop_back();
          missed_bs.push_back(block_hash);
        }
      }
      else
        missed_bs.push_back(block_hash);
    }
    catch (const std::exception& e)
    {
      return false;
    }
  }
  return true;
}
 
static bool fill(BlockchainDB *db, const crypto::hash &tx_hash, cryptonote::blobdata &tx, bool pruned)
{
  if (pruned)
  {
    if (!db->get_pruned_tx_blob(tx_hash, tx))
    {
      MDEBUG("Pruned transaction blob not found for " << tx_hash);
      return false;
    }
  }
  else
  {
    if (!db->get_tx_blob(tx_hash, tx))
    {
      MDEBUG("Transaction blob not found for " << tx_hash);
      return false;
    }
  }
  return true;
}
 
static bool fill(BlockchainDB *db, const crypto::hash &tx_hash, tx_blob_entry &tx, bool pruned)
{
  if (!fill(db, tx_hash, tx.blob, pruned))
    return false;
  if (pruned)
  {
    if (is_v1_tx(tx.blob))
    {
       
      cryptonote::blobdata prunable_blob;
      if (!db->get_prunable_tx_blob(tx_hash, prunable_blob))
      {
        MDEBUG("Prunable transaction blob not found for " << tx_hash);
        return false;
      }
      tx.blob.append(prunable_blob);
      tx.prunable_hash = crypto::null_hash;
    }
    else
    {
      if (!db->get_prunable_tx_hash(tx_hash, tx.prunable_hash))
      {
        MDEBUG("Prunable transaction data hash not found for " << tx_hash);
        return false;
      }
    }
  }
  return true;
}
 
 
 
bool Blockchain::get_transactions_blobs(const std::vector<crypto::hash>& txs_ids, std::vector<cryptonote::blobdata>& txs, std::vector<crypto::hash>& missed_txs, bool pruned) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  txs.reserve(txs_ids.size());
  for (const auto& tx_hash : txs_ids)
  {
    try
    {
      cryptonote::blobdata tx;
      if (fill(m_db, tx_hash, tx, pruned))
        txs.push_back(std::move(tx));
      else
        missed_txs.push_back(tx_hash);
    }
    catch (const std::exception& e)
    {
      return false;
    }
  }
  return true;
}
 
bool Blockchain::get_transactions_blobs(const std::vector<crypto::hash>& txs_ids, std::vector<tx_blob_entry>& txs, std::vector<crypto::hash>& missed_txs, bool pruned) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  txs.reserve(txs_ids.size());
  for (const auto& tx_hash : txs_ids)
  {
    try
    {
      tx_blob_entry tx;
      if (fill(m_db, tx_hash, tx, pruned))
        txs.push_back(std::move(tx));
      else
        missed_txs.push_back(tx_hash);
    }
    catch (const std::exception& e)
    {
      return false;
    }
  }
  return true;
}
 
size_t get_transaction_version(const cryptonote::blobdata &bd)
{
  size_t version;
  const char* begin = static_cast<const char*>(bd.data());
  const char* end = begin + bd.size();
  int read = tools::read_varint(begin, end, version);
  if (read <= 0)
    throw std::runtime_error("Internal error getting transaction version");
  return version;
}
 
template<class t_ids_container, class t_tx_container, class t_missed_container>
bool Blockchain::get_split_transactions_blobs(const t_ids_container& txs_ids, t_tx_container& txs, t_missed_container& missed_txs) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  reserve_container(txs, txs_ids.size());
  for (const auto& tx_hash : txs_ids)
  {
    try
    {
      cryptonote::blobdata tx;
      if (m_db->get_pruned_tx_blob(tx_hash, tx))
      {
        txs.push_back(std::make_tuple(tx_hash, std::move(tx), crypto::null_hash, cryptonote::blobdata()));
        if (!is_v1_tx(std::get<1>(txs.back())) && !m_db->get_prunable_tx_hash(tx_hash, std::get<2>(txs.back())))
        {
          MERROR("Prunable data hash not found for " << tx_hash);
          return false;
        }
        if (!m_db->get_prunable_tx_blob(tx_hash, std::get<3>(txs.back())))
          std::get<3>(txs.back()).clear();
      }
      else
        missed_txs.push_back(tx_hash);
    }
    catch (const std::exception& e)
    {
      return false;
    }
  }
  return true;
}
 
template<class t_ids_container, class t_tx_container, class t_missed_container>
bool Blockchain::get_transactions(const t_ids_container& txs_ids, t_tx_container& txs, t_missed_container& missed_txs, bool pruned) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  reserve_container(txs, txs_ids.size());
  for (const auto& tx_hash : txs_ids)
  {
    try
    {
      cryptonote::blobdata tx;
      bool res = pruned ? m_db->get_pruned_tx_blob(tx_hash, tx) : m_db->get_tx_blob(tx_hash, tx);
      if (res)
      {
        txs.push_back(transaction());
        res = pruned ? parse_and_validate_tx_base_from_blob(tx, txs.back()) : parse_and_validate_tx_from_blob(tx, txs.back());
        if (!res)
        {
          LOG_ERROR("Invalid transaction");
          return false;
        }
      }
      else
        missed_txs.push_back(tx_hash);
    }
    catch (const std::exception& e)
    {
      return false;
    }
  }
  return true;
}
 
 
 
 
bool Blockchain::find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, std::vector<crypto::hash>& hashes, std::vector<uint64_t>* weights, uint64_t& start_height, uint64_t& current_height, bool clip_pruned) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

   
  if(!find_blockchain_supplement(qblock_ids, start_height))
  {
    return false;
  }

  db_rtxn_guard rtxn_guard(m_db);
  current_height = get_current_blockchain_height();
  uint64_t stop_height = current_height;
  if (clip_pruned)
  {
    const uint32_t pruning_seed = get_blockchain_pruning_seed();
    if (start_height < tools::get_next_unpruned_block_height(start_height, current_height, pruning_seed))
    {
      MDEBUG("We only have a pruned version of the common ancestor");
      return false;
    }
    stop_height = tools::get_next_pruned_block_height(start_height, current_height, pruning_seed);
  }
  size_t count = 0;
  const size_t reserve = std::min((size_t)(stop_height - start_height), (size_t)BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT);
  hashes.reserve(reserve);
  if (weights)
    weights->reserve(reserve);
  for(size_t i = start_height; i < stop_height && count < BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT; i++, count++)
  {
    hashes.push_back(m_db->get_block_hash_from_height(i));
    if (weights)
      weights->push_back(m_db->get_block_weight(i));
  }

  return true;
}

bool Blockchain::find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, bool clip_pruned, NOTIFY_RESPONSE_CHAIN_ENTRY::request& resp) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  bool result = find_blockchain_supplement(qblock_ids, resp.m_block_ids, &resp.m_block_weights, resp.start_height, resp.total_height, clip_pruned);
  if (result)
  {
    cryptonote::difficulty_type wide_cumulative_difficulty = m_db->get_block_cumulative_difficulty(resp.total_height - 1);
    resp.cumulative_difficulty = (wide_cumulative_difficulty & 0xffffffffffffffff).convert_to<uint64_t>();
    resp.cumulative_difficulty_top64 = ((wide_cumulative_difficulty >> 64) & 0xffffffffffffffff).convert_to<uint64_t>();
  }

  return result;
}
 
 
 
 
 
bool Blockchain::find_blockchain_supplement(const uint64_t req_start_block, const std::list<crypto::hash>& qblock_ids, std::vector<std::pair<std::pair<cryptonote::blobdata, crypto::hash>, std::vector<std::pair<crypto::hash, cryptonote::blobdata> > > >& blocks, uint64_t& total_height, uint64_t& start_height, bool pruned, bool get_miner_tx_hash, size_t max_block_count, size_t max_tx_count) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

   
  if(req_start_block > 0)
  {
     
    if (req_start_block >= m_db->height())
    {
      return false;
    }
    start_height = req_start_block;
  }
  else
  {
    if(!find_blockchain_supplement(qblock_ids, start_height))
    {
      return false;
    }
  }

  db_rtxn_guard rtxn_guard(m_db);
  total_height = get_current_blockchain_height();
  blocks.reserve(std::min(std::min(max_block_count, (size_t)10000), (size_t)(total_height - start_height)));
  CHECK_AND_ASSERT_MES(m_db->get_blocks_from(start_height, 3, max_block_count, max_tx_count, FIND_BLOCKCHAIN_SUPPLEMENT_MAX_SIZE, blocks, pruned, true, get_miner_tx_hash),
      false, "Error getting blocks");

  return true;
}
 
bool Blockchain::add_block_as_invalid(const block& bl, const crypto::hash& h)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  block_extended_info bei = AUTO_VAL_INIT(bei);
  bei.bl = bl;
  return add_block_as_invalid(bei, h);
}
 
bool Blockchain::add_block_as_invalid(const block_extended_info& bei, const crypto::hash& h)
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  auto i_res = m_invalid_blocks.insert(std::map<crypto::hash, block_extended_info>::value_type(h, bei));
  CHECK_AND_ASSERT_MES(i_res.second, false, "at insertion invalid by tx returned status existed");
  MINFO("BLOCK ADDED AS INVALID: " << h << std::endl << ", prev_id=" << bei.bl.prev_id << ", m_invalid_blocks count=" << m_invalid_blocks.size());
  return true;
}
 
void Blockchain::flush_invalid_blocks()
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  m_invalid_blocks.clear();
}
 
bool Blockchain::have_block_unlocked(const crypto::hash& id, int *where) const
{
   
   
   
   
  LOG_PRINT_L3("Blockchain::" << __func__);

  if(m_db->block_exists(id))
  {
    LOG_PRINT_L2("block " << id << " found in main chain");
    if (where) *where = HAVE_BLOCK_MAIN_CHAIN;
    return true;
  }

  if(m_db->get_alt_block(id, NULL, NULL))
  {
    LOG_PRINT_L2("block " << id << " found in alternative chains");
    if (where) *where = HAVE_BLOCK_ALT_CHAIN;
    return true;
  }

  if(m_invalid_blocks.count(id))
  {
    LOG_PRINT_L2("block " << id << " found in m_invalid_blocks");
    if (where) *where = HAVE_BLOCK_INVALID;
    return true;
  }

  return false;
}
 
bool Blockchain::have_block(const crypto::hash& id, int *where) const
{
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  return have_block_unlocked(id, where);
}
 
bool Blockchain::handle_block_to_main_chain(const block& bl, block_verification_context& bvc, bool notify )
{
    LOG_PRINT_L3("Blockchain::" << __func__);
    crypto::hash id = get_block_hash(bl);
    return handle_block_to_main_chain(bl, id, bvc, notify);
}
 
size_t Blockchain::get_total_transactions() const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
   
   
   
   
  return m_db->get_tx_count();
}
 
 
 
 
 
 
 
bool Blockchain::check_for_double_spend(const transaction& tx, key_images_container& keys_this_block) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  struct add_transaction_input_visitor: public boost::static_visitor<bool>
  {
    key_images_container& m_spent_keys;
    BlockchainDB* m_db;
    add_transaction_input_visitor(key_images_container& spent_keys, BlockchainDB* db) :
      m_spent_keys(spent_keys), m_db(db)
    {
    }
    bool operator()(const txin_to_key& in) const
    {
      const crypto::key_image& ki = in.k_image;

       
       
       
       
       
       
       
      auto r = m_spent_keys.insert(ki);
      if(!r.second || m_db->has_key_image(ki))
      {
         
        return false;
      }

       
      return true;
    }

    bool operator()(const txin_gen& tx) const
    {
      return true;
    }
    bool operator()(const txin_to_script& tx) const
    {
      return false;
    }
    bool operator()(const txin_to_scripthash& tx) const
    {
      return false;
    }
  };

  for (const txin_v& in : tx.vin)
  {
    if(!boost::apply_visitor(add_transaction_input_visitor(keys_this_block, m_db), in))
    {
      LOG_ERROR("Double spend detected!");
      return false;
    }
  }

  return true;
}
 
bool Blockchain::get_tx_outputs_gindexs(const crypto::hash& tx_id, size_t n_txes, std::vector<std::vector<uint64_t>>& indexs) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  uint64_t tx_index;
  if (!m_db->tx_exists(tx_id, tx_index))
  {
    MERROR_VER("get_tx_outputs_gindexs failed to find transaction with id = " << tx_id);
    return false;
  }
  indexs = m_db->get_tx_amount_output_indices(tx_index, n_txes);
  CHECK_AND_ASSERT_MES(n_txes == indexs.size(), false, "Wrong indexs size");

  return true;
}
 
bool Blockchain::get_tx_outputs_gindexs(const crypto::hash& tx_id, std::vector<uint64_t>& indexs) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  uint64_t tx_index;
  if (!m_db->tx_exists(tx_id, tx_index))
  {
    MERROR_VER("get_tx_outputs_gindexs failed to find transaction with id = " << tx_id);
    return false;
  }
  std::vector<std::vector<uint64_t>> indices = m_db->get_tx_amount_output_indices(tx_index, 1);
  CHECK_AND_ASSERT_MES(indices.size() == 1, false, "Wrong indices size");
  indexs = indices.front();
  return true;
}
 
void Blockchain::on_new_tx_from_block(const cryptonote::transaction &tx)
{
#if defined(PER_BLOCK_CHECKPOINT)
   
  if (m_db->height() < m_blocks_hash_check.size())
  {
    TIME_MEASURE_START(a);
    m_blocks_txs_check.push_back(get_transaction_hash(tx));
    TIME_MEASURE_FINISH(a);
    if(m_show_time_stats)
    {
      size_t ring_size = !tx.vin.empty() && tx.vin[0].type() == typeid(txin_to_key) ? boost::get<txin_to_key>(tx.vin[0]).key_offsets.size() : 0;
      MINFO("HASH: " << "-" << " I/M/O: " << tx.vin.size() << "/" << ring_size << "/" << tx.vout.size() << " H: " << 0 << " chcktx: " << a);
    }
  }
#endif
}
 
 
 
 
 
 
 
 
 
bool Blockchain::check_tx_inputs(transaction& tx, uint64_t& max_used_block_height, crypto::hash& max_used_block_id, tx_verification_context &tvc, bool kept_by_block) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

#if defined(PER_BLOCK_CHECKPOINT)
   
  if (m_db->height() < m_blocks_hash_check.size() && kept_by_block)
  {
    max_used_block_id = null_hash;
    max_used_block_height = 0;
    return true;
  }
#endif

  TIME_MEASURE_START(a);
  bool res = check_tx_inputs(tx, tvc, &max_used_block_height);
  TIME_MEASURE_FINISH(a);
  if(m_show_time_stats)
  {
    size_t ring_size = !tx.vin.empty() && tx.vin[0].type() == typeid(txin_to_key) ? boost::get<txin_to_key>(tx.vin[0]).key_offsets.size() : 0;
    MINFO("HASH: " <<  get_transaction_hash(tx) << " I/M/O: " << tx.vin.size() << "/" << ring_size << "/" << tx.vout.size() << " H: " << max_used_block_height << " ms: " << a + m_fake_scan_time << " B: " << get_object_blobsize(tx) << " W: " << get_transaction_weight(tx));
  }
  if (!res)
    return false;

  CHECK_AND_ASSERT_MES(max_used_block_height < m_db->height(), false,  "internal error: max used block index=" << max_used_block_height << " is not less then blockchain size = " << m_db->height());
  max_used_block_id = m_db->get_block_hash_from_height(max_used_block_height);
  return true;
}
 
bool Blockchain::check_tx_outputs(const transaction& tx, tx_verification_context &tvc) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

   
   
  if (!check_tx_output_amounts(tx))
  {
    tvc.m_invalid_output = true;
    return false;
  }

  const uint8_t hf_version = m_hardfork->get_current_version();

   
 
 
 
 
 
 
 
 
 
 
 

 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 

   
  if (hf_version > HF_VERSION_BULLETPROOF_PLUS) {
    if (tx.version >= 2) {
      const bool bulletproof = rct::is_rct_bulletproof(tx.rct_signatures.type);
      if (bulletproof)
      {
        MERROR_VER("Bulletproof range proofs are not allowed after v" + std::to_string(HF_VERSION_BULLETPROOF_PLUS));
        tvc.m_invalid_output = true;
        return false;
      }
    }
  }

   
  if (!check_output_types(tx, hf_version))
  {
    tvc.m_invalid_output = true;
    return false;
  }

  return true;
}
 
bool Blockchain::have_tx_keyimges_as_spent(const transaction &tx) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  for (const txin_v& in: tx.vin)
  {
    CHECKED_GET_SPECIFIC_VARIANT(in, const txin_to_key, in_to_key, true);
    if(have_tx_keyimg_as_spent(in_to_key.k_image))
      return true;
  }
  return false;
}
bool Blockchain::expand_transaction_2(transaction &tx, const crypto::hash &tx_prefix_hash, const std::vector<std::vector<rct::ctkey>> &pubkeys)
{
  PERF_TIMER(expand_transaction_2);
  CHECK_AND_ASSERT_MES(tx.version > 1 && tx.version <= CURRENT_TRANSACTION_VERSION, false, "Unsupported RingCT transaction version");

  rct::rctSig &rv = tx.rct_signatures;

   
  rv.message = rct::hash2rct(tx_prefix_hash);

   
  if (rv.type == rct::RCTTypeFull)
  {
    CHECK_AND_ASSERT_MES(!pubkeys.empty() && !pubkeys[0].empty(), false, "empty pubkeys");
    rv.mixRing.resize(pubkeys[0].size());
    for (size_t m = 0; m < pubkeys[0].size(); ++m)
      rv.mixRing[m].clear();
    for (size_t n = 0; n < pubkeys.size(); ++n)
    {
      CHECK_AND_ASSERT_MES(pubkeys[n].size() <= pubkeys[0].size(), false, "More inputs that first ring");
      for (size_t m = 0; m < pubkeys[n].size(); ++m)
      {
        rv.mixRing[m].push_back(pubkeys[n][m]);
      }
    }
  }
  else if (rv.type == rct::RCTTypeSimple || rv.type == rct::RCTTypeBulletproof || rv.type == rct::RCTTypeBulletproof2 || rv.type == rct::RCTTypeCLSAG || rv.type == rct::RCTTypeBulletproofPlus)
  {
    CHECK_AND_ASSERT_MES(!pubkeys.empty() && !pubkeys[0].empty(), false, "empty pubkeys");
    rv.mixRing.resize(pubkeys.size());
    for (size_t n = 0; n < pubkeys.size(); ++n)
    {
      rv.mixRing[n].clear();
      for (size_t m = 0; m < pubkeys[n].size(); ++m)
      {
        rv.mixRing[n].push_back(pubkeys[n][m]);
      }
    }
  }
  else
  {
    CHECK_AND_ASSERT_MES(false, false, "Unsupported rct tx type: " + boost::lexical_cast<std::string>(rv.type));
  }

   
  if (rv.type == rct::RCTTypeFull)
  {
    if (!tx.pruned)
    {
      rv.p.MGs.resize(1);
      rv.p.MGs[0].II.resize(tx.vin.size());
      for (size_t n = 0; n < tx.vin.size(); ++n)
        rv.p.MGs[0].II[n] = rct::ki2rct(boost::get<txin_to_key>(tx.vin[n]).k_image);
    }
  }
  else if (rv.type == rct::RCTTypeSimple || rv.type == rct::RCTTypeBulletproof || rv.type == rct::RCTTypeBulletproof2)
  {
    if (!tx.pruned)
    {
      CHECK_AND_ASSERT_MES(rv.p.MGs.size() == tx.vin.size(), false, "Bad MGs size");
      for (size_t n = 0; n < tx.vin.size(); ++n)
      {
        rv.p.MGs[n].II.resize(1);
        rv.p.MGs[n].II[0] = rct::ki2rct(boost::get<txin_to_key>(tx.vin[n]).k_image);
      }
    }
  }
  else if (rv.type == rct::RCTTypeCLSAG || rv.type == rct::RCTTypeBulletproofPlus)
  {
    if (!tx.pruned)
    {
      CHECK_AND_ASSERT_MES(rv.p.CLSAGs.size() == tx.vin.size(), false, "Bad CLSAGs size");
      for (size_t n = 0; n < tx.vin.size(); ++n)
      {
        rv.p.CLSAGs[n].I = rct::ki2rct(boost::get<txin_to_key>(tx.vin[n]).k_image);
      }
    }
  }
  else
  {
    CHECK_AND_ASSERT_MES(false, false, "Unsupported rct tx type: " + boost::lexical_cast<std::string>(rv.type));
  }

   

  return true;
}
 
 
 
 
 
 
bool Blockchain::check_tx_inputs(transaction& tx, tx_verification_context &tvc, uint64_t* pmax_used_block_height) const
{
  PERF_TIMER(check_tx_inputs);
  LOG_PRINT_L3("Blockchain::" << __func__);
   
  if (!check_tx_burn_rules(tx) || !check_tx_output_amounts(tx))
  {
    tvc.m_verifivation_failed = true;
    return false;
  }
  size_t sig_index = 0;
  if(pmax_used_block_height)
    *pmax_used_block_height = 0;

   
  if (tx.pruned)
    return true;

  crypto::hash tx_prefix_hash = get_transaction_prefix_hash(tx);

  const uint8_t hf_version = m_hardfork->get_current_version();
 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
   
  if (hf_version >= 2)
  {
    size_t n_unmixable = 0, n_mixable = 0;
    size_t min_actual_mixin = std::numeric_limits<size_t>::max();
    size_t max_actual_mixin = 0;
    const size_t min_mixin = 15;
    for (const auto& txin : tx.vin)
    {
       
      if (txin.type() == typeid(txin_to_key))
      {
        const txin_to_key& in_to_key = boost::get<txin_to_key>(txin);
        if (in_to_key.amount == 0)
        {
           
           
           
          ++n_mixable;
        }
        else
        {
          uint64_t n_outputs = m_db->get_num_outputs(in_to_key.amount);
          MDEBUG("output size " << print_money(in_to_key.amount) << ": " << n_outputs << " available");
           
          if (n_outputs <= min_mixin)
            ++n_unmixable;
          else
            ++n_mixable;
        }
        size_t ring_mixin = in_to_key.key_offsets.size() - 1;
        if (ring_mixin < min_actual_mixin)
          min_actual_mixin = ring_mixin;
        if (ring_mixin > max_actual_mixin)
          max_actual_mixin = ring_mixin;
      }
    }
    MDEBUG("Mixin: " << min_actual_mixin << "-" << max_actual_mixin);

      if (min_actual_mixin != max_actual_mixin)
      {
        MERROR_VER("Tx " << get_transaction_hash(tx) << " has varying ring size (" << (min_actual_mixin + 1) << "-" << (max_actual_mixin + 1) << "), it should be constant");
        tvc.m_low_mixin = true;
        return false;
      }

     
     
     
     
    if (min_actual_mixin < min_mixin)
    {
      if (n_unmixable == 0)
      {
        MERROR_VER("Tx " << get_transaction_hash(tx) << " has too low ring size (" << (min_actual_mixin + 1) << "), and no unmixable inputs");
        tvc.m_low_mixin = true;
        return false;
      }
      if (n_mixable > 1)
      {
        MERROR_VER("Tx " << get_transaction_hash(tx) << " has too low ring size (" << (min_actual_mixin + 1) << "), and more than one mixable input with unmixable inputs");
        tvc.m_low_mixin = true;
        return false;
      }
    } else if (min_actual_mixin > 15)
    {
      MERROR_VER("Tx " << get_transaction_hash(tx) << " has invalid ring size (" << (min_actual_mixin + 1) << "), it should be " << (min_mixin + 1));
      tvc.m_low_mixin = true;
      return false;
    }

    const size_t min_tx_version = (n_unmixable > 0 ? 1 : 2);
    if (tx.version < min_tx_version)
    {
      MERROR_VER("transaction version " << (unsigned)tx.version << " is lower than min accepted version " << min_tx_version);
      tvc.m_verifivation_failed = true;
      return false;
    }
  }

   
  if (hf_version >= 7) {
    const crypto::key_image *last_key_image = NULL;
    for (size_t n = 0; n < tx.vin.size(); ++n)
    {
      const txin_v &txin = tx.vin[n];
      if (txin.type() == typeid(txin_to_key))
      {
        const txin_to_key& in_to_key = boost::get<txin_to_key>(txin);
        if (last_key_image && memcmp(&in_to_key.k_image, last_key_image, sizeof(*last_key_image)) >= 0)
        {
          MERROR_VER("transaction has unsorted inputs");
          tvc.m_verifivation_failed = true;
          return false;
        }
        last_key_image = &in_to_key.k_image;
      }
    }
  }

  std::vector<std::vector<rct::ctkey>> pubkeys(tx.vin.size());
  std::vector < uint64_t > results;
  results.resize(tx.vin.size(), 0);

  tools::threadpool& tpool = tools::threadpool::getInstanceForCompute();
  tools::threadpool::waiter waiter(tpool);
  int threads = tpool.get_max_concurrency();

  uint64_t max_used_block_height = 0;
  if (!pmax_used_block_height)
    pmax_used_block_height = &max_used_block_height;
  for (const auto& txin : tx.vin)
  {
     
     
    CHECK_AND_ASSERT_MES(txin.type() == typeid(txin_to_key), false, "wrong type id in tx input at Blockchain::check_tx_inputs");
    const txin_to_key& in_to_key = boost::get<txin_to_key>(txin);

     
    CHECK_AND_ASSERT_MES(in_to_key.key_offsets.size(), false, "empty in_to_key.key_offsets in transaction with id " << get_transaction_hash(tx));

    if(have_tx_keyimg_as_spent(in_to_key.k_image))
    {
      MERROR_VER("Key image already spent in blockchain: " << epee::string_tools::pod_to_hex(in_to_key.k_image));
      tvc.m_double_spend = true;
      return false;
    }

    if (tx.version == 1)
    {
       
      CHECK_AND_ASSERT_MES(sig_index < tx.signatures.size(), false, "wrong transaction: not signature entry for input with index= " << sig_index);
    }

     
     
    if (!check_tx_input(tx.version, in_to_key, tx_prefix_hash, tx.version == 1 ? tx.signatures[sig_index] : std::vector<crypto::signature>(), tx.rct_signatures, pubkeys[sig_index], pmax_used_block_height, hf_version))
    {
      MERROR_VER("Failed to check ring signature for tx " << get_transaction_hash(tx) << "  vin key with k_image: " << in_to_key.k_image << "  sig_index: " << sig_index);
      if (pmax_used_block_height)  
      {
        MERROR_VER("  *pmax_used_block_height: " << *pmax_used_block_height);
      }

      return false;
    }

    if (tx.version == 1)
    {
      if (threads > 1)
      {
         
         
        tpool.submit(&waiter, boost::bind(&Blockchain::check_ring_signature, this, std::cref(tx_prefix_hash), std::cref(in_to_key.k_image), std::cref(pubkeys[sig_index]), std::cref(tx.signatures[sig_index]), std::ref(results[sig_index])), true);
      }
      else
      {
        check_ring_signature(tx_prefix_hash, in_to_key.k_image, pubkeys[sig_index], tx.signatures[sig_index], results[sig_index]);
        if (!results[sig_index])
        {
          MERROR_VER("Failed to check ring signature for tx " << get_transaction_hash(tx) << "  vin key with k_image: " << in_to_key.k_image << "  sig_index: " << sig_index);

          if (pmax_used_block_height)   
          {
            MERROR_VER("*pmax_used_block_height: " << *pmax_used_block_height);
          }

          return false;
        }
      }
    }

    sig_index++;
  }
  if (tx.version == 1 && threads > 1)
    if (!waiter.wait())
      return false;

   
    CHECK_AND_ASSERT_MES(*pmax_used_block_height + CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE <= m_db->height(),
        false, "Transaction spends at least one output which is too young");

   
  static constexpr const std::uint8_t RCT_CACHE_TYPE = rct::RCTTypeBulletproofPlus;
  if (tx.rct_signatures.type > RCT_CACHE_TYPE)
  {
    MWARNING("RCT cache is not caching new verification results. Please update RCT_CACHE_TYPE!");
  }

  if (tx.version == 1)
  {
    if (threads > 1)
    {
       
      bool failed = false;
      for (size_t i = 0; i < tx.vin.size(); i++)
      {
        if(!failed && !results[i])
          failed = true;
      }

      if (failed)
      {
        MERROR_VER("Failed to check ring signatures!");
        return false;
      }
    }
  }
  else
  {
     
     
     
    const rct::rctSig &rv = tx.rct_signatures;
    switch (rv.type)
    {
    case rct::RCTTypeNull: {
       
      MERROR_VER("Null rct signature on non-coinbase tx");
      return false;
    }
    case rct::RCTTypeSimple:
    case rct::RCTTypeBulletproof:
    case rct::RCTTypeBulletproof2:
    case rct::RCTTypeCLSAG:
    case rct::RCTTypeBulletproofPlus:
    {
      if (!ver_rct_non_semantics_simple_cached(tx, pubkeys, m_rct_ver_cache, RCT_CACHE_TYPE))
      {
        MERROR_VER("Failed to check ringct signatures!");
        return false;
      }
      break;
    }
    case rct::RCTTypeFull:
    {
      if (!expand_transaction_2(tx, tx_prefix_hash, pubkeys))
      {
        MERROR_VER("Failed to expand rct signatures!");
        return false;
      }

       
      {
        bool size_matches = true;
        for (size_t i = 0; i < pubkeys.size(); ++i)
          size_matches &= pubkeys[i].size() == rv.mixRing.size();
        for (size_t i = 0; i < rv.mixRing.size(); ++i)
          size_matches &= pubkeys.size() == rv.mixRing[i].size();
        if (!size_matches)
        {
          MERROR_VER("Failed to check ringct signatures: mismatched pubkeys/mixRing size");
          return false;
        }

        for (size_t n = 0; n < pubkeys.size(); ++n)
        {
          for (size_t m = 0; m < pubkeys[n].size(); ++m)
          {
            if (pubkeys[n][m].dest != rct::rct2pk(rv.mixRing[m][n].dest))
            {
              MERROR_VER("Failed to check ringct signatures: mismatched pubkey at vin " << n << ", index " << m);
              return false;
            }
            if (pubkeys[n][m].mask != rct::rct2pk(rv.mixRing[m][n].mask))
            {
              MERROR_VER("Failed to check ringct signatures: mismatched commitment at vin " << n << ", index " << m);
              return false;
            }
          }
        }
      }

      if (rv.p.MGs.size() != 1)
      {
        MERROR_VER("Failed to check ringct signatures: Bad MGs size");
        return false;
      }
      if (rv.p.MGs.empty() || rv.p.MGs[0].II.size() != tx.vin.size())
      {
        MERROR_VER("Failed to check ringct signatures: mismatched II/vin sizes");
        return false;
      }
      for (size_t n = 0; n < tx.vin.size(); ++n)
      {
        if (memcmp(&boost::get<txin_to_key>(tx.vin[n]).k_image, &rv.p.MGs[0].II[n], 32))
        {
          MERROR_VER("Failed to check ringct signatures: mismatched II/vin sizes");
          return false;
        }
      }

      if (!rct::verRct(rv, false))
      {
        MERROR_VER("Failed to check ringct signatures!");
        return false;
      }
      break;
    }
    default:
      MERROR_VER("Unsupported rct type: " << rv.type);
      return false;
    }

     
    if (rct::is_rct_bulletproof(rv.type))
    {
      if (hf_version < 8)
      {
        for (const rct::Bulletproof &proof: rv.p.bulletproofs)
        {
          if (proof.V.size() > 1)
          {
            MERROR_VER("Multi output bulletproofs are invalid before v8");
            return false;
          }
        }
      }
    }
  }
  return true;
}

 
void Blockchain::check_ring_signature(const crypto::hash &tx_prefix_hash, const crypto::key_image &key_image, const std::vector<rct::ctkey> &pubkeys, const std::vector<crypto::signature>& sig, uint64_t &result) const
{
  std::vector<const crypto::public_key *> p_output_keys;
  p_output_keys.reserve(pubkeys.size());
  for (auto &key : pubkeys)
  {
     
    p_output_keys.push_back(&(const crypto::public_key&)key.dest);
  }

  result = crypto::check_ring_signature(tx_prefix_hash, key_image, p_output_keys, sig.data()) ? 1 : 0;
}

 
uint64_t Blockchain::get_dynamic_base_fee(uint64_t block_reward, size_t median_block_weight, uint8_t version)
{
  const uint64_t min_block_weight = get_min_block_weight(version);
  if (median_block_weight < min_block_weight)
    median_block_weight = min_block_weight;
  uint64_t hi, lo;

    lo = mul128(block_reward, DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT, &hi);
    div128_64(hi, lo, median_block_weight, &hi, &lo, NULL, NULL);
       
       
      div128_64(hi, lo, median_block_weight, &hi, &lo, NULL, NULL);
      assert(hi == 0);
      lo -= lo / 20;
      return lo;
 
 

 
 
 
 

 
 
 
 

 
}

 
bool Blockchain::check_fee(size_t tx_weight, uint64_t fee) const
{
  const uint8_t version = get_current_hard_fork_version();

  uint64_t median = 0;
  uint64_t already_generated_coins = 0;
  uint64_t base_reward = 0;
    median = m_current_block_cumul_weight_limit / 2;
    const uint64_t blockchain_height = m_db->height();
    already_generated_coins = blockchain_height ? m_db->get_block_already_generated_coins(blockchain_height - 1) : 0;
    if (!get_block_reward(median, 1, already_generated_coins, base_reward, version, blockchain_height))
      return false;

  uint64_t needed_fee;
    const bool use_long_term_median_in_fee = true;
    uint64_t fee_per_byte = get_dynamic_base_fee(base_reward, use_long_term_median_in_fee ? std::min<uint64_t>(median, m_long_term_effective_median_block_weight) : median, version);
    MDEBUG("Using " << print_money(fee_per_byte) << "/byte fee");
    needed_fee = tx_weight * fee_per_byte;
     
    const uint64_t mask = get_fee_quantization_mask();
    needed_fee = (needed_fee + mask - 1) / mask * mask;

  if (fee < needed_fee - needed_fee / 50)  
  {
    MERROR_VER("transaction fee is not enough: " << print_money(fee) << ", minimum fee: " << print_money(needed_fee));
    return false;
  }
  return true;
}

 
void Blockchain::get_dynamic_base_fee_estimate_2021_scaling(uint64_t grace_blocks, uint64_t base_reward, uint64_t Mnw, uint64_t Mlw, std::vector<uint64_t> &fees) const
{
   
   
  const uint64_t Mfw = std::min(Mnw, Mlw);

   
   

   
   

   
  const uint64_t Fl = base_reward *   DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT / (Mfw * Mfw);

   
   
  const uint64_t Fn = 4 * base_reward *   DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT / (Mfw * Mfw);

   
  const uint64_t Fm = 16 * base_reward * DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT / (CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5 * Mfw);

   

   
   
  const uint64_t Fh = std::max<uint64_t>(4 * Fm, 4 * Fm * Mfw / (32 * DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT * Mnw / CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5));

  fees.resize(4);
  fees[0] = cryptonote::round_money_up(Fl, CRYPTONOTE_SCALING_2021_FEE_ROUNDING_PLACES);
  fees[1] = cryptonote::round_money_up(Fn, CRYPTONOTE_SCALING_2021_FEE_ROUNDING_PLACES);
  fees[2] = cryptonote::round_money_up(Fm, CRYPTONOTE_SCALING_2021_FEE_ROUNDING_PLACES);
  fees[3] = cryptonote::round_money_up(Fh, CRYPTONOTE_SCALING_2021_FEE_ROUNDING_PLACES);
}

void Blockchain::get_dynamic_base_fee_estimate_2021_scaling(uint64_t grace_blocks, std::vector<uint64_t> &fees) const
{
  const uint8_t version = get_current_hard_fork_version();
  const uint64_t db_height = m_db->height();

  CHECK_AND_ASSERT_THROW_MES(grace_blocks <= CRYPTONOTE_REWARD_BLOCKS_WINDOW, "Grace blocks invalid In 2021 fee scaling estimate.");

   
   
   
   
   
   

  epee::misc_utils::rolling_median_t<uint64_t> rm = m_long_term_block_weights_cache_rolling_median;
  for (size_t i = 0; i < grace_blocks; ++i)
    rm.insert(0);
  const uint64_t Mlw_penalty_free_zone_for_wallet = std::max<uint64_t>(rm.median(), CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5);

   
  std::vector<uint64_t> weights;
  get_last_n_blocks_weights(weights, 100 - grace_blocks);
  weights.reserve(100);
  for (size_t i = 0; i < grace_blocks; ++i)
    weights.push_back(0);
  const uint64_t Msw_effective_short_term_median = std::max(epee::misc_utils::median(weights), Mlw_penalty_free_zone_for_wallet);

  const uint64_t Mnw = std::min(Msw_effective_short_term_median, 50 * Mlw_penalty_free_zone_for_wallet);

  uint64_t already_generated_coins = db_height ? m_db->get_block_already_generated_coins(db_height - 1) : 0;
  uint64_t base_reward;
  if (!get_block_reward(m_current_block_cumul_weight_limit / 2, 1, already_generated_coins, base_reward, version, db_height))
  {
    MERROR("Failed to determine block reward, using placeholder " << print_money(BLOCK_REWARD_OVERESTIMATE) << " as a high bound");
    base_reward = BLOCK_REWARD_OVERESTIMATE;
  }

  get_dynamic_base_fee_estimate_2021_scaling(grace_blocks, base_reward, Mnw, Mlw_penalty_free_zone_for_wallet, fees);
}

 
uint64_t Blockchain::get_dynamic_base_fee_estimate(uint64_t grace_blocks) const
{
  const uint8_t version = get_current_hard_fork_version();
  const uint64_t db_height = m_db->height();


  if(grace_blocks >= CRYPTONOTE_REWARD_BLOCKS_WINDOW){
    grace_blocks = CRYPTONOTE_REWARD_BLOCKS_WINDOW - 1;
  }
    std::vector<uint64_t> fees;
    get_dynamic_base_fee_estimate_2021_scaling(grace_blocks, fees);
    return fees[0];
}

 
 
 
bool Blockchain::is_tx_spendtime_unlocked(uint64_t unlock_time, uint8_t hf_version) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  if(unlock_time < CRYPTONOTE_MAX_BLOCK_NUMBER)
  {
     
     
    if(m_db->height()-1 + CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS >= unlock_time)
      return true;
    else
      return false;
  }
  else
  {
     
    const uint64_t current_time = get_adjusted_time(m_db->height());
    if(current_time + (get_current_hard_fork_version() < 2 ? CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V1 : CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V2) >= unlock_time)
      return true;
    else
      return false;
  }
  return false;
}
 
 
 
 
bool Blockchain::check_tx_input(size_t tx_version, const txin_to_key& txin, const crypto::hash& tx_prefix_hash, const std::vector<crypto::signature>& sig, const rct::rctSig &rct_signatures, std::vector<rct::ctkey> &output_keys, uint64_t* pmax_related_block_height, uint8_t hf_version) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);

   
   
   

  struct outputs_visitor
  {
    std::vector<rct::ctkey >& m_output_keys;
    const Blockchain& m_bch;
    const uint8_t hf_version;
    outputs_visitor(std::vector<rct::ctkey>& output_keys, const Blockchain& bch, uint8_t hf_version) :
      m_output_keys(output_keys), m_bch(bch), hf_version(hf_version)
    {
    }
    bool handle_output(uint64_t unlock_time, const crypto::public_key &pubkey, const rct::key &commitment)
    {
       
      if (!m_bch.is_tx_spendtime_unlocked(unlock_time, hf_version))
      {
        MERROR_VER("One of outputs for one of inputs has wrong tx.unlock_time = " << unlock_time);
        return false;
      }

       
       
       
       
       
       

      m_output_keys.push_back(rct::ctkey({rct::pk2rct(pubkey), commitment}));
      return true;
    }
  };

  output_keys.clear();

   
  outputs_visitor vi(output_keys, *this, hf_version);
  if (!scan_outputkeys_for_indexes(tx_version, txin, vi, tx_prefix_hash, pmax_related_block_height))
  {
    MERROR_VER("Failed to get output keys for tx with amount = " << print_money(txin.amount) << " and count indexes " << txin.key_offsets.size());
    return false;
  }

  if(txin.key_offsets.size() != output_keys.size())
  {
    MERROR_VER("Output keys for tx with amount = " << txin.amount << " and count indexes " << txin.key_offsets.size() << " returned wrong keys count " << output_keys.size());
    return false;
  }
  if (tx_version == 1) {
    CHECK_AND_ASSERT_MES(sig.size() == output_keys.size(), false, "internal error: tx signatures count=" << sig.size() << " mismatch with outputs keys count for inputs=" << output_keys.size());
  }
   
  return true;
}
 
 
uint64_t Blockchain::get_adjusted_time(uint64_t height) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);

   
  if(height < BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW)
  {
      return static_cast<uint64_t>(time(NULL));
  }
  std::vector<uint64_t> timestamps;

   
  size_t offset = height - BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW;
  timestamps.reserve(height - offset);
  for(;offset < height; ++offset)
  {
    timestamps.push_back(m_db->get_block_timestamp(offset));
  }
  uint64_t median_ts = epee::misc_utils::median(timestamps);

   
   
  median_ts += (BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW + 1) * DIFFICULTY_TARGET_V2 / 2;

   
   
  uint64_t adjusted_current_block_ts = timestamps.back() + DIFFICULTY_TARGET_V2;

   
   
  return (adjusted_current_block_ts < median_ts ? adjusted_current_block_ts : median_ts);
}
 
 
bool Blockchain::check_block_timestamp(std::vector<uint64_t>& timestamps, const block& b, uint64_t& median_ts) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  median_ts = epee::misc_utils::median(timestamps);

  if(b.timestamp < median_ts)
  {
    MERROR_VER("Timestamp of block with id: " << get_block_hash(b) << ", " << b.timestamp << ", less than median of last " << BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW << " blocks, " << median_ts);
    return false;
  }

  return true;
}
 
 
 
 
 
 
 
 
bool Blockchain::check_block_timestamp(const block& b, uint64_t& median_ts) const
{
  LOG_PRINT_L3("Blockchain::" << __func__);
  if(b.timestamp > (uint64_t)time(NULL) + CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT)
  {
    MERROR_VER("Timestamp of block with id: " << get_block_hash(b) << ", " << b.timestamp << ", bigger than local time + 2 hours");
    return false;
  }

  const auto h = m_db->height();

   
  if(h < BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW)
  {
    return true;
  }

  std::vector<uint64_t> timestamps;

   
  size_t offset = h - BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW;
  timestamps.reserve(h - offset);
  for(;offset < h; ++offset)
  {
    timestamps.push_back(m_db->get_block_timestamp(offset));
  }

  return check_block_timestamp(timestamps, b, median_ts);
}
 
void Blockchain::return_tx_to_pool(std::vector<std::pair<transaction, blobdata>> &txs)
{
  uint8_t version = get_current_hard_fork_version();
  for (auto& tx : txs)
  {
    cryptonote::tx_verification_context tvc = AUTO_VAL_INIT(tvc);
     
     
     
     
     
    const size_t weight = get_transaction_weight(tx.first, tx.second.size());
    const crypto::hash tx_hash = get_transaction_hash(tx.first);
    if (!m_tx_pool.add_tx(tx.first, tx_hash, tx.second, weight, tvc, relay_method::block, true, version))
    {
      MERROR("Failed to return taken transaction with hash: " << get_transaction_hash(tx.first) << " to tx_pool");
    }
  }
}
 
bool Blockchain::flush_txes_from_pool(const std::vector<crypto::hash> &txids)
{
  CRITICAL_REGION_LOCAL(m_tx_pool);

  bool res = true;
  for (const auto &txid: txids)
  {
    cryptonote::transaction tx;
    cryptonote::blobdata txblob;
    size_t tx_weight;
    uint64_t fee;
    bool relayed, do_not_relay, double_spend_seen, pruned;
    MINFO("Removing txid " << txid << " from the pool");
    if(m_tx_pool.have_tx(txid, relay_category::all) && !m_tx_pool.take_tx(txid, tx, txblob, tx_weight, fee, relayed, do_not_relay, double_spend_seen, pruned))
    {
      MERROR("Failed to remove txid " << txid << " from the pool");
      res = false;
    }
  }
  return res;
}
 
 
 
 
bool Blockchain::handle_block_to_main_chain(const block& bl, const crypto::hash& id, block_verification_context& bvc, bool notify )
{
  LOG_PRINT_L3("Blockchain::" << __func__);

  if (!check_block_approval(bl, m_nettype)) {
    bvc.m_verifivation_failed = true;
    return false;
  }
  TIME_MEASURE_START(block_processing_time);
  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  TIME_MEASURE_START(t1);

  static bool seen_future_version = false;

  db_rtxn_guard rtxn_guard(m_db);
  uint64_t blockchain_height;
  const crypto::hash top_hash = get_tail_id(blockchain_height);
  ++blockchain_height;  
  if(bl.prev_id != top_hash)
  {
    MERROR_VER("Block with id: " << id << std::endl << "has wrong prev_id: " << bl.prev_id << std::endl << "expected: " << top_hash);
    bvc.m_verifivation_failed = true;
leave:
    return false;
  }

   
  if (!seen_future_version && bl.major_version > m_hardfork->get_ideal_version())
  {
    seen_future_version = true;
    const el::Level level = el::Level::Warning;
    MCLOG_RED(level, "global", "**********************************************************************");
    MCLOG_RED(level, "global", "A block was seen on the network with a version higher than the last");
    MCLOG_RED(level, "global", "known one. This may be an old version of the daemon, and a software");
    MCLOG_RED(level, "global", "update may be required to sync further. Try running: update check");
    MCLOG_RED(level, "global", "**********************************************************************");
  }

   
  const uint8_t hf_version = get_current_hard_fork_version();
  if (!m_hardfork->check(bl))
  {
    MERROR_VER("Block with id: " << id << std::endl << "has old version: " << (unsigned)bl.major_version << std::endl << "current: " << (unsigned)hf_version);
    bvc.m_verifivation_failed = true;
    goto leave;
  }

  TIME_MEASURE_FINISH(t1);
  TIME_MEASURE_START(t2);

   
   
  if(!check_block_timestamp(bl))
  {
    MERROR_VER("Block with id: " << id << std::endl << "has invalid timestamp: " << bl.timestamp);
    bvc.m_verifivation_failed = true;
    goto leave;
  }

  TIME_MEASURE_FINISH(t2);
   
  TIME_MEASURE_START(target_calculating_time);

   
   
   
   
   
  difficulty_type current_diffic = get_difficulty_for_next_block();
  CHECK_AND_ASSERT_MES(current_diffic, false, "!!!!!!!!! difficulty overhead !!!!!!!!!");

  TIME_MEASURE_FINISH(target_calculating_time);

  TIME_MEASURE_START(longhash_calculating_time);

  crypto::hash proof_of_work;
  memset(proof_of_work.data, 0xff, sizeof(proof_of_work.data));

   
   
   
   
   
   
   
   
   
  bool precomputed = false;
  bool fast_check = false;
#if defined(PER_BLOCK_CHECKPOINT)
  if (blockchain_height < m_blocks_hash_check.size())
  {
    const auto &expected_hash = m_blocks_hash_check[blockchain_height].first;
    if (expected_hash != crypto::null_hash)
    {
      if (memcmp(&id, &expected_hash, sizeof(hash)) != 0)
      {
        MERROR_VER("Block with id is INVALID: " << id << ", expected " << expected_hash);
        bvc.m_verifivation_failed = true;
        goto leave;
      }
      fast_check = true;
    }
    else
    {
      MCINFO("verify", "No pre-validated hash at height " << blockchain_height << ", verifying fully");
    }
  }
#endif
  if (!fast_check)
  {
    auto it = m_blocks_longhash_table.find(id);
    if (it != m_blocks_longhash_table.end())
    {
      precomputed = true;
      proof_of_work = it->second;
    }
    else
      proof_of_work = get_block_longhash(this, bl, blockchain_height, 0);

     
    if(!check_hash(proof_of_work, current_diffic))
    {
      MERROR_VER("Block with id: " << id << std::endl << "does not have enough proof of work: " << proof_of_work << " at height " << blockchain_height << ", unexpected difficulty: " << current_diffic);
      bvc.m_verifivation_failed = true;
      bvc.m_bad_pow = true;
      goto leave;
    }
  }

   
   
  if(m_checkpoints.is_in_checkpoint_zone(blockchain_height))
  {
    if(!m_checkpoints.check_block(blockchain_height, id))
    {
      LOG_ERROR("CHECKPOINT VALIDATION FAILED");
      bvc.m_verifivation_failed = true;
      goto leave;
    }
  }

  TIME_MEASURE_FINISH(longhash_calculating_time);
  if (precomputed)
    longhash_calculating_time += m_fake_pow_calc_time;

  TIME_MEASURE_START(t3);

   
  if(!prevalidate_miner_transaction(bl, blockchain_height, hf_version))
  {
    MERROR_VER("Block with id: " << id << " failed to pass prevalidation");
    bvc.m_verifivation_failed = true;
    goto leave;
  }

  size_t coinbase_weight = get_transaction_weight(bl.miner_tx);
  size_t cumulative_block_weight = coinbase_weight;

  std::vector<std::pair<transaction, blobdata>> txs;
  key_images_container keys;

  uint64_t fee_summary = 0;
  uint64_t t_checktx = 0;
  uint64_t t_exists = 0;
  uint64_t t_pool = 0;
  uint64_t t_dblspnd = 0;
  uint64_t n_pruned = 0;
  TIME_MEASURE_FINISH(t3);

 

  size_t tx_index = 0;
   
   
   
  txs.reserve(bl.tx_hashes.size());
  for (const crypto::hash& tx_id : bl.tx_hashes)
  {
    transaction tx_tmp;
    blobdata txblob;
    size_t tx_weight = 0;
    uint64_t fee = 0;
    bool relayed = false, do_not_relay = false, double_spend_seen = false, pruned = false;
    TIME_MEASURE_START(aa);

 
    if (m_db->tx_exists(tx_id))
    {
      MERROR("Block with id: " << id << " attempting to add transaction already in blockchain with id: " << tx_id);
      bvc.m_verifivation_failed = true;
      return_tx_to_pool(txs);
      goto leave;
    }

    TIME_MEASURE_FINISH(aa);
    t_exists += aa;
    TIME_MEASURE_START(bb);

     
    if(!m_tx_pool.take_tx(tx_id, tx_tmp, txblob, tx_weight, fee, relayed, do_not_relay, double_spend_seen, pruned))
    {
      MERROR_VER("Block with id: " << id  << " has at least one unknown transaction with id: " << tx_id);
      bvc.m_verifivation_failed = true;
      return_tx_to_pool(txs);
      goto leave;
    }
    if (pruned)
      ++n_pruned;

    TIME_MEASURE_FINISH(bb);
    t_pool += bb;
     
     
     
    txs.push_back(std::make_pair(std::move(tx_tmp), std::move(txblob)));
    transaction &tx = txs.back().first;
    TIME_MEASURE_START(dd);

     
     
     
     
     
     
     
     
     
     
     
     

    TIME_MEASURE_FINISH(dd);
    t_dblspnd += dd;
    TIME_MEASURE_START(cc);

#if defined(PER_BLOCK_CHECKPOINT)
    if (!fast_check)
#endif
    {
       
      tx_verification_context tvc;
      if(!check_tx_inputs(tx, tvc))
      {
        MERROR_VER("Block with id: " << id  << " has at least one transaction (id: " << tx_id << ") with wrong inputs.");

         
        add_block_as_invalid(bl, id);
        MERROR_VER("Block with id " << id << " added as invalid because of wrong inputs in transactions");
        MERROR_VER("tx_index " << tx_index << ", m_blocks_txs_check " << m_blocks_txs_check.size() << ":");
        for (const auto &h: m_blocks_txs_check) MERROR_VER("  " << h);
        bvc.m_verifivation_failed = true;
        return_tx_to_pool(txs);
        goto leave;
      }
    }
#if defined(PER_BLOCK_CHECKPOINT)
    else
    {
       
       
      if (tx_index >= m_blocks_txs_check.size() || memcmp(&m_blocks_txs_check[tx_index++], &tx_id, sizeof(tx_id)) != 0)
      {
        MERROR_VER("Block with id: " << id << " has at least one transaction (id: " << tx_id << ") with wrong inputs.");
         
        add_block_as_invalid(bl, id);
        MERROR_VER("Block with id " << id << " added as invalid because of wrong inputs in transactions");
        bvc.m_verifivation_failed = true;
        return_tx_to_pool(txs);
        goto leave;
      }
    }
#endif
    TIME_MEASURE_FINISH(cc);
    t_checktx += cc;
    if (fee > std::numeric_limits<uint64_t>::max() - fee_summary)
    {
      bvc.m_verifivation_failed = true;
      return_tx_to_pool(txs);
      goto leave;
    }
    fee_summary += fee;
    cumulative_block_weight += tx_weight;
  }

   
  if (n_pruned > 0)
  {
    if (blockchain_height >= m_blocks_hash_check.size() || m_blocks_hash_check[blockchain_height].second == 0)
    {
      MERROR("Block at " << blockchain_height << " is pruned, but we do not have a weight for it");
      goto leave;
    }
    cumulative_block_weight = m_blocks_hash_check[blockchain_height].second;
  }

  m_blocks_txs_check.clear();

  TIME_MEASURE_START(vmt);
  uint64_t base_reward = 0;
  uint64_t already_generated_coins = blockchain_height ? m_db->get_block_already_generated_coins(blockchain_height - 1) : 0;
  if(!validate_miner_transaction(bl, cumulative_block_weight, fee_summary, base_reward, already_generated_coins, bvc.m_partial_block_reward, m_hardfork->get_current_version()))
  {
    MERROR_VER("Block with id: " << id << " has incorrect miner transaction");
    bvc.m_verifivation_failed = true;
    return_tx_to_pool(txs);
    goto leave;
  }

  TIME_MEASURE_FINISH(vmt);
  size_t block_weight;
  difficulty_type cumulative_difficulty;

   
  block_weight = cumulative_block_weight;
  cumulative_difficulty = current_diffic;
   
   
   
   
  already_generated_coins = base_reward < (MONEY_SUPPLY-already_generated_coins) ? already_generated_coins + base_reward : MONEY_SUPPLY;
  if(blockchain_height)
    cumulative_difficulty += m_db->get_block_cumulative_difficulty(blockchain_height - 1);

  TIME_MEASURE_FINISH(block_processing_time);
  if(precomputed)
    block_processing_time += m_fake_pow_calc_time;

  rtxn_guard.stop();
  TIME_MEASURE_START(addblock);
  uint64_t new_height = 0;
  if (!bvc.m_verifivation_failed)
  {
    try
    {
      uint64_t long_term_block_weight = get_next_long_term_block_weight(block_weight);
      block committed = bl;

      cryptonote::blobdata bd = cryptonote::block_to_blob(committed);
      new_height = m_db->add_block(std::make_pair(std::move(committed), std::move(bd)), block_weight, long_term_block_weight, cumulative_difficulty, already_generated_coins, txs);
    }
    catch (const KEY_IMAGE_EXISTS& e)
    {
      LOG_ERROR("Error adding block with hash: " << id << " to blockchain, what = " << e.what());
      m_batch_success = false;
      bvc.m_verifivation_failed = true;
      return_tx_to_pool(txs);
      return false;
    }
    catch (const std::exception& e)
    {
       
      LOG_ERROR("Error adding block with hash: " << id << " to blockchain, what = " << e.what());
      m_batch_success = false;
      bvc.m_verifivation_failed = true;
      return_tx_to_pool(txs);
      return false;
    }
  }
  else
  {
    LOG_ERROR("Blocks that failed verification should not reach here");
  }

  TIME_MEASURE_FINISH(addblock);

   
  if (!update_next_cumulative_weight_limit())
  {
    MERROR("Failed to update next cumulative weight limit");
    pop_block_from_blockchain();
    return false;
  }

  MINFO("+++++ BLOCK SUCCESSFULLY ADDED" << std::endl << "id:\t" << id << std::endl << "PoW:\t" << proof_of_work << std::endl << "HEIGHT " << new_height-1 << ", difficulty:\t" << current_diffic << std::endl << "block reward: " << print_money(fee_summary + base_reward) << "(" << print_money(base_reward) << " + " << print_money(fee_summary) << "), coinbase_weight: " << coinbase_weight << ", cumulative weight: " << cumulative_block_weight << ", " << block_processing_time << "(" << target_calculating_time << "/" << longhash_calculating_time << ")ms");
  if(m_show_time_stats)
  {
    MINFO("Height: " << new_height << " coinbase weight: " << coinbase_weight << " cumm: "
        << cumulative_block_weight << " p/t: " << block_processing_time << " ("
        << target_calculating_time << "/" << longhash_calculating_time << "/"
        << t1 << "/" << t2 << "/" << t3 << "/" << t_exists << "/" << t_pool
        << "/" << t_checktx << "/" << t_dblspnd << "/" << vmt << "/" << addblock << ")ms");
  }

  bvc.m_added_to_main_chain = true;
  ++m_sync_counter;

   
  m_tx_pool.on_blockchain_inc(new_height, id);
  get_difficulty_for_next_block();  
  invalidate_block_template_cache();

  const uint8_t new_hf_version = get_current_hard_fork_version();
  if (new_hf_version != hf_version)
  {
     
     
    const bool is_genesis_block = new_height == 1;
    if (!is_genesis_block)
    {
      MGINFO("Validating txpool for v" << (unsigned)new_hf_version);
      m_tx_pool.validate(new_hf_version);
    }
  }

  const crypto::hash seedhash = get_block_id_by_height(crypto::rx_seedheight(new_height));
  send_miner_notifications(new_height, seedhash, id, already_generated_coins);

  for (const auto& notifier: m_block_notifiers)
    notifier(new_height - 1, {std::addressof(bl), 1});

  rx_set_main_seedhash(seedhash.data, tools::get_max_concurrency());

  return true;
}
 
bool Blockchain::prune_blockchain(uint32_t pruning_seed)
{
  m_tx_pool.lock();
  epee::misc_utils::auto_scope_leave_caller unlocker = epee::misc_utils::create_scope_leave_handler([&](){m_tx_pool.unlock();});
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  return m_db->prune_blockchain(pruning_seed);
}
 
bool Blockchain::update_blockchain_pruning()
{
  m_tx_pool.lock();
  epee::misc_utils::auto_scope_leave_caller unlocker = epee::misc_utils::create_scope_leave_handler([&](){m_tx_pool.unlock();});
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  return m_db->update_pruning();
}
 
bool Blockchain::check_blockchain_pruning()
{
  m_tx_pool.lock();
  epee::misc_utils::auto_scope_leave_caller unlocker = epee::misc_utils::create_scope_leave_handler([&](){m_tx_pool.unlock();});
  CRITICAL_REGION_LOCAL(m_blockchain_lock);

  return m_db->check_pruning();
}
 
 
uint64_t Blockchain::get_next_long_term_block_weight(uint64_t block_weight) const
{
  PERF_TIMER(get_next_long_term_block_weight);

  const uint64_t db_height = m_db->height();
  const uint64_t nblocks = std::min<uint64_t>(m_long_term_block_weights_window, db_height);

  const uint8_t hf_version = get_current_hard_fork_version();
  if (db_height < CRYPTONOTE_LONG_TERM_BLOCK_WEIGHT_WINDOW_SIZE)
    return block_weight;

  uint64_t long_term_median = get_long_term_block_weight_median(db_height - nblocks, nblocks);
  uint64_t long_term_effective_median_block_weight = std::max<uint64_t>(CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5, long_term_median);

  uint64_t short_term_constraint;
     
    block_weight = std::max<uint64_t>(block_weight, long_term_effective_median_block_weight * 10 / 17);
    short_term_constraint = long_term_effective_median_block_weight + long_term_effective_median_block_weight * 7 / 10;
  uint64_t long_term_block_weight = std::min<uint64_t>(block_weight, short_term_constraint);

  return long_term_block_weight;
}
 
bool Blockchain::update_next_cumulative_weight_limit(uint64_t *long_term_effective_median_block_weight)
{
  PERF_TIMER(update_next_cumulative_weight_limit);

  LOG_PRINT_L3("Blockchain::" << __func__);

   
  const uint64_t db_height = m_db->height();
  const uint8_t hf_version = get_current_hard_fork_version();
  uint64_t full_reward_zone = get_min_block_weight(hf_version);

  const uint64_t block_weight = m_db->get_block_weight(db_height - 1);

  uint64_t long_term_median;
  if (db_height == 1)
  {
    long_term_median = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5;
  }
  else
  {
    uint64_t nblocks = std::min<uint64_t>(m_long_term_block_weights_window, db_height);
    if (nblocks == db_height)
      --nblocks;
    long_term_median = get_long_term_block_weight_median(db_height - nblocks - 1, nblocks);
  }

  m_long_term_effective_median_block_weight = std::max<uint64_t>(CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5, long_term_median);

  uint64_t short_term_constraint = m_long_term_effective_median_block_weight;
  short_term_constraint += m_long_term_effective_median_block_weight * 7 / 10;
  uint64_t long_term_block_weight = std::min<uint64_t>(block_weight, short_term_constraint);

  if (db_height == 1)
  {
    long_term_median = long_term_block_weight;
  }
  else
  {
    m_long_term_block_weights_cache_tip_hash = m_db->get_block_hash_from_height(db_height - 1);
    m_long_term_block_weights_cache_rolling_median.insert(long_term_block_weight);
    long_term_median = m_long_term_block_weights_cache_rolling_median.median();
  }
  m_long_term_effective_median_block_weight = std::max<uint64_t>(CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5, long_term_median);
  
  std::vector<uint64_t> weights;
  get_last_n_blocks_weights(weights, CRYPTONOTE_REWARD_BLOCKS_WINDOW);
  
  uint64_t short_term_median = epee::misc_utils::median(weights);
  uint64_t effective_median_block_weight;
   
   
  effective_median_block_weight = std::min<uint64_t>(std::max<uint64_t>(m_long_term_effective_median_block_weight, short_term_median), CRYPTONOTE_SHORT_TERM_BLOCK_WEIGHT_SURGE_FACTOR * m_long_term_effective_median_block_weight);
  
  m_current_block_cumul_weight_median = effective_median_block_weight;

  if (m_current_block_cumul_weight_median <= full_reward_zone)
    m_current_block_cumul_weight_median = full_reward_zone;

  m_current_block_cumul_weight_limit = m_current_block_cumul_weight_median * 2;

  if (long_term_effective_median_block_weight)
    *long_term_effective_median_block_weight = m_long_term_effective_median_block_weight;

  if (!m_db->is_read_only())
    m_db->add_max_block_size(m_current_block_cumul_weight_limit);

  return true;
}

 
bool Blockchain::add_new_block(const block& bl, block_verification_context& bvc)
{
  try
  {

  LOG_PRINT_L3("Blockchain::" << __func__);
  crypto::hash id = get_block_hash(bl);
  if (!check_block_approval(bl, m_nettype)) {
    bvc.m_verifivation_failed = true;
    return false;
  }
  CRITICAL_REGION_LOCAL(m_tx_pool); 
  CRITICAL_REGION_LOCAL1(m_blockchain_lock);
  db_rtxn_guard rtxn_guard(m_db);
  if(have_block(id))
  {
    LOG_PRINT_L3("block with id = " << id << " already exists");
    bvc.m_already_exists = true;
    m_blocks_txs_check.clear();
    return false;
  }

   
  if(!(bl.prev_id == get_tail_id()))
  {
     
    bvc.m_added_to_main_chain = false;
    rtxn_guard.stop();
    bool r = handle_alternative_block(bl, id, bvc);
    m_blocks_txs_check.clear();
    return r;
     
  }

  rtxn_guard.stop();
  return handle_block_to_main_chain(bl, id, bvc, true);

  }
  catch (const std::exception &e)
  {
    LOG_ERROR("Exception at [add_new_block], what=" << e.what());
    bvc.m_verifivation_failed = true;
    return false;
  }
}
 
 
 
void Blockchain::check_against_checkpoints(const checkpoints& points, bool enforce)
{
  const auto& pts = points.get_points();
  bool stop_batch;

  CRITICAL_REGION_LOCAL(m_blockchain_lock);
  stop_batch = m_db->batch_start();
  const uint64_t blockchain_height = m_db->height();
  for (const auto& pt : pts)
  {
     
    if (pt.first >= blockchain_height)
    {
      continue;
    }

    if (!points.check_block(pt.first, m_db->get_block_hash_from_height(pt.first)))
    {
       
      if (enforce)
      {
        LOG_ERROR("Local blockchain failed to pass a checkpoint, rolling back!");
        std::list<block> empty;
        rollback_blockchain_switching(empty, pt.first - 2);
      }
      else
      {
        LOG_ERROR("WARNING: local blockchain failed to pass a DNS checkpoint, and you could be on a fork. You should either sync up from scratch, OR download a fresh blockchain bootstrap, OR enable checkpoint enforcing with the --enforce-dns-checkpointing command-line option");
      }
    }
  }
  if (stop_batch)
    m_db->batch_stop();
}
 
 
 
 
bool Blockchain::update_checkpoints(const std::string& file_path, bool check_dns)
{
  if (!m_checkpoints.load_checkpoints_from_json(file_path))
  {
      return false;
  }

   
   
  if (m_enforce_dns_checkpoints && check_dns && !m_offline)
  {
    if (!m_checkpoints.load_checkpoints_from_dns())
    {
      return false;
    }
  }
  else if (check_dns && !m_offline)
  {
    checkpoints dns_points;
    dns_points.load_checkpoints_from_dns();
    if (m_checkpoints.check_for_conflicts(dns_points))
    {
      check_against_checkpoints(dns_points, false);
    }
    else
    {
      MERROR("One or more checkpoints fetched from DNS conflicted with existing checkpoints!");
    }
  }

  check_against_checkpoints(m_checkpoints, true);

  return true;
}
 
void Blockchain::set_enforce_dns_checkpoints(bool enforce_checkpoints)
{
  m_enforce_dns_checkpoints = enforce_checkpoints;
}

 
void Blockchain::block_longhash_worker(uint64_t height, const epee::span<const block> &blocks, std::unordered_map<crypto::hash, crypto::hash> &map) const
{
  TIME_MEASURE_START(t);
  slow_hash_allocate_state();

  for (const auto & block : blocks)
  {
    if (m_cancel)
       break;
    crypto::hash id = get_block_hash(block);
    crypto::hash pow = get_block_longhash(this, block, height++, 0);
    map.emplace(id, pow);
  }

  slow_hash_free_state();
  TIME_MEASURE_FINISH(t);
}

 
bool Blockchain::cleanup_handle_incoming_blocks(bool force_sync)
{
  bool success = false;

  MTRACE("Blockchain::" << __func__);
  CRITICAL_REGION_BEGIN(m_blockchain_lock);
  TIME_MEASURE_START(t1);

  try
  {
    if (m_batch_success)
    {
      m_db->batch_stop();
      if (m_reset_timestamps_and_difficulties_height)
      {
        m_timestamps_and_difficulties_height = 0;
        m_reset_timestamps_and_difficulties_height = false;
      }
    }
    else
      m_db->batch_abort();
    success = true;
  }
  catch (const std::exception &e)
  {
    MERROR("Exception in cleanup_handle_incoming_blocks: " << e.what());
  }

  if (success && m_sync_counter > 0)
  {
    if (force_sync)
    {
      if(m_db_sync_mode != db_nosync)
        store_blockchain();
      m_sync_counter = 0;
    }
    else if (m_db_sync_threshold && ((m_db_sync_on_blocks && m_sync_counter >= m_db_sync_threshold) || (!m_db_sync_on_blocks && m_bytes_to_sync >= m_db_sync_threshold)))
    {
      MDEBUG("Sync threshold met, syncing");
      if(m_db_sync_mode == db_async)
      {
        m_sync_counter = 0;
        m_bytes_to_sync = 0;
        m_async_service.dispatch(boost::bind(&Blockchain::store_blockchain, this));
      }
      else if(m_db_sync_mode == db_sync)
      {
        store_blockchain();
      }
      else  
      {
         
      }
    }
  }

  TIME_MEASURE_FINISH(t1);
  m_blocks_longhash_table.clear();
  m_scan_table.clear();
  m_blocks_txs_check.clear();

   
  if (!m_blocks_hash_check.empty() && m_db->height() > m_blocks_hash_check.size() + 4096)
  {
    MINFO("Dumping block hashes, we're now 4k past " << m_blocks_hash_check.size());
    m_blocks_hash_check.clear();
    m_blocks_hash_check.shrink_to_fit();
  }

  CRITICAL_REGION_END();
  m_tx_pool.unlock();

  update_blockchain_pruning();

  return success;
}

 
void Blockchain::output_scan_worker(const uint64_t amount, const std::vector<uint64_t> &offsets, std::vector<output_data_t> &outputs) const
{
  try
  {
    m_db->get_output_key(epee::span<const uint64_t>(&amount, 1), offsets, outputs, true);
  }
  catch (const std::exception& e)
  {
    MERROR_VER("EXCEPTION: " << e.what());
  }
  catch (...)
  {

  }
}

uint64_t Blockchain::prevalidate_block_hashes(uint64_t height, const std::vector<crypto::hash> &hashes, const std::vector<uint64_t> &weights)
{
   
   

  CHECK_AND_ASSERT_MES(weights.empty() || weights.size() == hashes.size(), 0, "Unexpected weights size");

  CRITICAL_REGION_LOCAL(m_blockchain_lock);

   
  if (height >= m_blocks_hash_of_hashes.size() * HASH_OF_HASHES_STEP)
    return hashes.size();

   
  if (m_blocks_hash_check.empty())
    return hashes.size();

   
  size_t first_index = height / HASH_OF_HASHES_STEP;
  size_t last_index = (height + hashes.size() - 1) / HASH_OF_HASHES_STEP;
  MDEBUG("Blocks " << height << " - " << (height + hashes.size() - 1) << " start at " << first_index << " and end at " << last_index);

   
  if (first_index == last_index && hashes.size() < HASH_OF_HASHES_STEP && (height + hashes.size()) % HASH_OF_HASHES_STEP)
    return hashes.size();

   
  std::vector<crypto::hash> data_hashes;
  std::vector<uint64_t> data_weights;
  data_hashes.reserve(hashes.size() + HASH_OF_HASHES_STEP - 1);  
  if (!weights.empty())
    data_weights.reserve(data_hashes.size());

   
  bool disconnected = (height > m_db->height());
  size_t pop;
  if (disconnected && height % HASH_OF_HASHES_STEP)
  {
    ++first_index;
    pop = HASH_OF_HASHES_STEP - height % HASH_OF_HASHES_STEP;
  }
  else
  {
     
    for (uint64_t h = first_index * HASH_OF_HASHES_STEP; h < height; ++h)
    {
      data_hashes.push_back(m_db->get_block_hash_from_height(h));
      if (!weights.empty())
        data_weights.push_back(m_db->get_block_weight(h));
    }
    pop = 0;
  }

   
  for (size_t i = 0; i < hashes.size(); ++i)
  {
    if (pop)
      --pop;
    else
    {
      data_hashes.push_back(hashes[i]);
      if (!weights.empty())
        data_weights.push_back(weights[i]);
    }
  }

   
  uint64_t usable = first_index * HASH_OF_HASHES_STEP - height;  
  for (size_t n = first_index; n <= last_index; ++n)
  {
    if (n < m_blocks_hash_of_hashes.size())
    {
       
      if (data_hashes.size() < (n - first_index) * HASH_OF_HASHES_STEP + HASH_OF_HASHES_STEP)
        break;

      crypto::hash hash;
      cn_fast_hash(data_hashes.data() + (n - first_index) * HASH_OF_HASHES_STEP, HASH_OF_HASHES_STEP * sizeof(crypto::hash), hash);
      bool valid = hash == m_blocks_hash_of_hashes[n].first;
      if (valid && !weights.empty())
      {
        cn_fast_hash(data_weights.data() + (n - first_index) * HASH_OF_HASHES_STEP, HASH_OF_HASHES_STEP * sizeof(uint64_t), hash);
        valid &= hash == m_blocks_hash_of_hashes[n].second;
      }

       
      if (!valid)
      {
        MDEBUG("invalid hash for blocks " << n * HASH_OF_HASHES_STEP << " - " << (n * HASH_OF_HASHES_STEP + HASH_OF_HASHES_STEP - 1));
        break;
      }

      size_t end = n * HASH_OF_HASHES_STEP + HASH_OF_HASHES_STEP;
      for (size_t i = n * HASH_OF_HASHES_STEP; i < end; ++i)
      {
        CHECK_AND_ASSERT_MES(m_blocks_hash_check[i].first == crypto::null_hash || m_blocks_hash_check[i].first == data_hashes[i - first_index * HASH_OF_HASHES_STEP],
            0, "Consistency failure in m_blocks_hash_check construction");
        m_blocks_hash_check[i].first = data_hashes[i - first_index * HASH_OF_HASHES_STEP];
        if (!weights.empty())
        {
          CHECK_AND_ASSERT_MES(m_blocks_hash_check[i].second == 0 || m_blocks_hash_check[i].second == data_weights[i - first_index * HASH_OF_HASHES_STEP],
              0, "Consistency failure in m_blocks_hash_check construction");
          m_blocks_hash_check[i].second = data_weights[i - first_index * HASH_OF_HASHES_STEP];
        }
      }
      usable += HASH_OF_HASHES_STEP;
    }
    else
    {
       
      usable += HASH_OF_HASHES_STEP;
      if (usable > hashes.size())
        usable = hashes.size();
    }
  }
  MDEBUG("usable: " << usable << " / " << hashes.size());
  CHECK_AND_ASSERT_MES(usable < std::numeric_limits<uint64_t>::max() / 2, 0, "usable is negative");
  return usable;
}

bool Blockchain::has_block_weights(uint64_t height, uint64_t nblocks) const
{
  CHECK_AND_ASSERT_MES(nblocks > 0, false, "nblocks is 0");
  uint64_t last_block_height = height + nblocks - 1;
  if (last_block_height >= m_blocks_hash_check.size())
    return false;
  for (uint64_t h = height; h <= last_block_height; ++h)
    if (m_blocks_hash_check[h].second == 0)
      return false;
  return true;
}

 
 
 
 
 
 
 
bool Blockchain::prepare_handle_incoming_blocks(const std::vector<block_complete_entry> &blocks_entry, std::vector<block> &blocks)
{
  MTRACE("Blockchain::" << __func__);
  TIME_MEASURE_START(prepare);
  bool stop_batch;
  uint64_t bytes = 0;
  size_t total_txs = 0;
  blocks.clear();

   
   
   
   
   
   
   
   
   
   
   
   
   

  m_tx_pool.lock();
  CRITICAL_REGION_LOCAL1(m_blockchain_lock);

  if(blocks_entry.size() == 0)
    return false;

  for (const auto &entry : blocks_entry)
  {
    bytes += entry.block.size();
    for (const auto &tx_blob : entry.txs)
    {
      bytes += tx_blob.blob.size();
    }
    total_txs += entry.txs.size();
  }
  m_bytes_to_sync += bytes;
  while (!(stop_batch = m_db->batch_start(blocks_entry.size(), bytes))) {
    m_blockchain_lock.unlock();
    m_tx_pool.unlock();
    epee::misc_utils::sleep_no_w(1000);
    m_tx_pool.lock();
    m_blockchain_lock.lock();
  }
  m_batch_success = true;

  const uint64_t height = m_db->height();
  if ((height + blocks_entry.size()) < m_blocks_hash_check.size())
    return true;

  bool blocks_exist = false;
  tools::threadpool& tpool = tools::threadpool::getInstanceForCompute();
  unsigned threads = tpool.get_max_concurrency();
  blocks.resize(blocks_entry.size());

  if (1)
  {
     
    if(threads > m_max_prepare_blocks_threads)
      threads = m_max_prepare_blocks_threads;

    unsigned int batches = blocks_entry.size() / threads;
    unsigned int extra = blocks_entry.size() % threads;
    MDEBUG("block_batches: " << batches);
    std::vector<std::unordered_map<crypto::hash, crypto::hash>> maps(threads);
    auto it = blocks_entry.begin();
    unsigned blockidx = 0;

    const crypto::hash tophash = m_db->top_block_hash();
    for (unsigned i = 0; i < threads; i++)
    {
      for (unsigned int j = 0; j < batches; j++, ++blockidx)
      {
        block &block = blocks[blockidx];
        crypto::hash block_hash;

        if (!parse_and_validate_block_from_blob(it->block, block, block_hash))
          return false;

         
        if (blockidx == 0)
        {
          if (block.prev_id != tophash)
          {
            MDEBUG("Skipping prepare blocks. New blocks don't belong to chain.");
            blocks.clear();
            return true;
          }
        }
        if (have_block(block_hash))
          blocks_exist = true;

        std::advance(it, 1);
      }
    }

    for (unsigned i = 0; i < extra && !blocks_exist; i++, blockidx++)
    {
      block &block = blocks[blockidx];
      crypto::hash block_hash;

      if (!parse_and_validate_block_from_blob(it->block, block, block_hash))
        return false;

      if (have_block(block_hash))
        blocks_exist = true;

      std::advance(it, 1);
    }

    if (!blocks_exist)
    {
      m_blocks_longhash_table.clear();
      uint64_t thread_height = height;
      tools::threadpool::waiter waiter(tpool);
      m_prepare_height = height;
      m_prepare_nblocks = blocks_entry.size();
      m_prepare_blocks = &blocks;
      for (unsigned int i = 0; i < threads; i++)
      {
        unsigned nblocks = batches;
        if (i < extra)
          ++nblocks;
        if (nblocks == 0)
          break;
        tpool.submit(&waiter, boost::bind(&Blockchain::block_longhash_worker, this, thread_height, epee::span<const block>(&blocks[thread_height - height], nblocks), std::ref(maps[i])), true);
        thread_height += nblocks;
      }

      if (!waiter.wait())
        return false;
      m_prepare_height = 0;

      if (m_cancel)
         return false;

      for (const auto & map : maps)
      {
        m_blocks_longhash_table.insert(map.begin(), map.end());
      }
    }
  }

  if (m_cancel)
    return false;

  if (blocks_exist)
  {
    MDEBUG("Skipping remainder of prepare blocks. Blocks exist.");
    return true;
  }

  m_fake_scan_time = 0;
  m_fake_pow_calc_time = 0;

  m_scan_table.clear();

  TIME_MEASURE_FINISH(prepare);
  m_fake_pow_calc_time = prepare / blocks_entry.size();

  if (blocks_entry.size() > 1 && threads > 1 && m_show_time_stats)
    MDEBUG("Prepare blocks took: " << prepare << " ms");

  TIME_MEASURE_START(scantable);

   
  std::vector < uint64_t > amounts;
   
  std::map<uint64_t, std::vector<uint64_t>> offset_map;
   
  std::map<uint64_t, std::vector<output_data_t>> tx_map;
  std::vector<std::pair<cryptonote::transaction, crypto::hash>> txes(total_txs);

#define SCAN_TABLE_QUIT(m) \
        do { \
            MERROR_VER(m) ;\
            m_scan_table.clear(); \
            return false; \
        } while(0); \

   
  size_t tx_index = 0, block_index = 0;
  for (const auto &entry : blocks_entry)
  {
    if (m_cancel)
      return false;

    for (const auto &tx_blob : entry.txs)
    {
      if (tx_index >= txes.size())
        SCAN_TABLE_QUIT("tx_index is out of sync");
      transaction &tx = txes[tx_index].first;
      crypto::hash &tx_prefix_hash = txes[tx_index].second;
      ++tx_index;

      if (!parse_and_validate_tx_base_from_blob(tx_blob.blob, tx))
        SCAN_TABLE_QUIT("Could not parse tx from incoming blocks.");
      cryptonote::get_transaction_prefix_hash(tx, tx_prefix_hash);

      auto its = m_scan_table.find(tx_prefix_hash);
      if (its != m_scan_table.end())
        SCAN_TABLE_QUIT("Duplicate tx found from incoming blocks.");

      m_scan_table.emplace(tx_prefix_hash, std::unordered_map<crypto::key_image, std::vector<output_data_t>>());
      its = m_scan_table.find(tx_prefix_hash);
      assert(its != m_scan_table.end());

       
      for (const auto &txin : tx.vin)
      {
        const txin_to_key &in_to_key = boost::get < txin_to_key > (txin);

         
        auto it = its->second.find(in_to_key.k_image);
        if (it != its->second.end())
          SCAN_TABLE_QUIT("Duplicate key_image found from incoming blocks.");

        amounts.push_back(in_to_key.amount);
      }

       
      std::sort(amounts.begin(), amounts.end());
      auto last = std::unique(amounts.begin(), amounts.end());
      amounts.erase(last, amounts.end());

       
      for (const uint64_t &amount : amounts)
      {
        if (offset_map.find(amount) == offset_map.end())
          offset_map.emplace(amount, std::vector<uint64_t>());

        if (tx_map.find(amount) == tx_map.end())
          tx_map.emplace(amount, std::vector<output_data_t>());
      }

       
      for (const auto &txin : tx.vin)
      {
        const txin_to_key &in_to_key = boost::get < txin_to_key > (txin);
         
        auto absolute_offsets = relative_output_offsets_to_absolute(in_to_key.key_offsets);
        for (const auto & offset : absolute_offsets)
          offset_map[in_to_key.amount].push_back(offset);

      }
    }
    ++block_index;
  }

   
  for (auto &offsets : offset_map)
  {
    std::sort(offsets.second.begin(), offsets.second.end());
    auto last = std::unique(offsets.second.begin(), offsets.second.end());
    offsets.second.erase(last, offsets.second.end());
  }

   
  threads = tpool.get_max_concurrency();
  if (!m_db->can_thread_bulk_indices())
    threads = 1;

  if (threads > 1 && amounts.size() > 1)
  {
    tools::threadpool::waiter waiter(tpool);

    for (size_t i = 0; i < amounts.size(); i++)
    {
      uint64_t amount = amounts[i];
      tpool.submit(&waiter, boost::bind(&Blockchain::output_scan_worker, this, amount, std::cref(offset_map[amount]), std::ref(tx_map[amount])), true);
    }
    if (!waiter.wait())
      return false;
  }
  else
  {
    for (size_t i = 0; i < amounts.size(); i++)
    {
      uint64_t amount = amounts[i];
      output_scan_worker(amount, offset_map[amount], tx_map[amount]);
    }
  }

   
  tx_index = 0;
  for (const auto &entry : blocks_entry)
  {
    if (m_cancel)
      return false;

    for (size_t i = 0; i < entry.txs.size(); ++i)
    {
      if (tx_index >= txes.size())
        SCAN_TABLE_QUIT("tx_index is out of sync");
      const transaction &tx = txes[tx_index].first;
      const crypto::hash &tx_prefix_hash = txes[tx_index].second;
      ++tx_index;

      auto its = m_scan_table.find(tx_prefix_hash);
      if (its == m_scan_table.end())
        SCAN_TABLE_QUIT("Tx not found on scan table from incoming blocks.");

      for (const auto &txin : tx.vin)
      {
        const txin_to_key &in_to_key = boost::get < txin_to_key > (txin);
        auto needed_offsets = relative_output_offsets_to_absolute(in_to_key.key_offsets);

        std::vector<output_data_t> outputs;
        for (const uint64_t & offset_needed : needed_offsets)
        {
          size_t pos = 0;
          bool found = false;

          for (const uint64_t &offset_found : offset_map[in_to_key.amount])
          {
            if (offset_needed == offset_found)
            {
              found = true;
              break;
            }

            ++pos;
          }

          if (found && pos < tx_map[in_to_key.amount].size())
            outputs.push_back(tx_map[in_to_key.amount].at(pos));
          else
            break;
        }

        its->second.emplace(in_to_key.k_image, outputs);
      }
    }
  }

  TIME_MEASURE_FINISH(scantable);
  if (total_txs > 0)
  {
    m_fake_scan_time = scantable / total_txs;
    if(m_show_time_stats)
      MDEBUG("Prepare scantable took: " << scantable << " ms");
  }

  return true;
}

void Blockchain::add_txpool_tx(const crypto::hash &txid, const cryptonote::blobdata &blob, const txpool_tx_meta_t &meta)
{
  m_db->add_txpool_tx(txid, blob, meta);
}

void Blockchain::update_txpool_tx(const crypto::hash &txid, const txpool_tx_meta_t &meta)
{
  m_db->update_txpool_tx(txid, meta);
}

void Blockchain::remove_txpool_tx(const crypto::hash &txid)
{
  m_db->remove_txpool_tx(txid);
}

uint64_t Blockchain::get_txpool_tx_count(bool include_sensitive) const
{
  return m_db->get_txpool_tx_count(include_sensitive ? relay_category::all : relay_category::broadcasted);
}

bool Blockchain::get_txpool_tx_meta(const crypto::hash& txid, txpool_tx_meta_t &meta) const
{
  return m_db->get_txpool_tx_meta(txid, meta);
}

bool Blockchain::get_txpool_tx_blob(const crypto::hash& txid, cryptonote::blobdata &bd, relay_category tx_category) const
{
  return m_db->get_txpool_tx_blob(txid, bd, tx_category);
}

cryptonote::blobdata Blockchain::get_txpool_tx_blob(const crypto::hash& txid, relay_category tx_category) const
{
  return m_db->get_txpool_tx_blob(txid, tx_category);
}

bool Blockchain::for_all_txpool_txes(std::function<bool(const crypto::hash&, const txpool_tx_meta_t&, const cryptonote::blobdata_ref*)> f, bool include_blob, relay_category tx_category) const
{
  return m_db->for_all_txpool_txes(f, include_blob, tx_category);
}

bool Blockchain::txpool_tx_matches_category(const crypto::hash& tx_hash, relay_category category)
{
  return m_db->txpool_tx_matches_category(tx_hash, category);
}

void Blockchain::set_user_options(uint64_t maxthreads, bool sync_on_blocks, uint64_t sync_threshold, blockchain_db_sync_mode sync_mode, bool fast_sync)
{
  if (sync_mode == db_defaultsync)
  {
    m_db_default_sync = true;
    sync_mode = db_async;
  }
  m_db_sync_mode = sync_mode;
  m_fast_sync = fast_sync;
  m_db_sync_on_blocks = sync_on_blocks;
  m_db_sync_threshold = sync_threshold;
  m_max_prepare_blocks_threads = maxthreads;
}

void Blockchain::add_block_notify(BlockNotifyCallback&& notify)
{
  if (notify)
  {
    CRITICAL_REGION_LOCAL(m_blockchain_lock);
    m_block_notifiers.push_back(std::move(notify));
  }
}

void Blockchain::add_miner_notify(MinerNotifyCallback&& notify)
{
  if (notify)
  {
    CRITICAL_REGION_LOCAL(m_blockchain_lock);
    m_miner_notifiers.push_back(std::move(notify));
  }
}

void Blockchain::safesyncmode(const bool onoff)
{
   
 
 
  if (m_db_default_sync)
  {
    m_db->safesyncmode(onoff);
    m_db_sync_mode = onoff ? db_nosync : db_async;
  }
}

HardFork::State Blockchain::get_hard_fork_state() const
{
  return m_hardfork->get_state();
}

bool Blockchain::get_hard_fork_voting_info(uint8_t version, uint32_t &window, uint32_t &votes, uint32_t &threshold, uint64_t &earliest_height, uint8_t &voting) const
{
  return m_hardfork->get_voting_info(version, window, votes, threshold, earliest_height, voting);
}

uint64_t Blockchain::get_difficulty_target() const
{
  return get_current_hard_fork_version() < 2 ? DIFFICULTY_TARGET_V1 : DIFFICULTY_TARGET_V2;
}

std::map<uint64_t, std::tuple<uint64_t, uint64_t, uint64_t>> Blockchain:: get_output_histogram(const std::vector<uint64_t> &amounts, bool unlocked, uint64_t recent_cutoff, uint64_t min_count) const
{
  return m_db->get_output_histogram(amounts, unlocked, recent_cutoff, min_count);
}

std::vector<std::pair<Blockchain::block_extended_info,std::vector<crypto::hash>>> Blockchain::get_alternative_chains() const
{
  std::vector<std::pair<Blockchain::block_extended_info,std::vector<crypto::hash>>> chains;

  blocks_ext_by_hash alt_blocks;
  alt_blocks.reserve(m_db->get_alt_block_count());
  m_db->for_all_alt_blocks([&alt_blocks](const crypto::hash &blkid, const cryptonote::alt_block_data_t &data, const cryptonote::blobdata_ref *blob) {
    if (!blob)
    {
      MERROR("No blob, but blobs were requested");
      return false;
    }
    cryptonote::block bl;
    block_extended_info bei;
    if (cryptonote::parse_and_validate_block_from_blob(*blob, bei.bl))
    {
      bei.height = data.height;
      bei.block_cumulative_weight = data.cumulative_weight;
      bei.cumulative_difficulty = data.cumulative_difficulty_high;
      bei.cumulative_difficulty = (bei.cumulative_difficulty << 64) + data.cumulative_difficulty_low;
      bei.already_generated_coins = data.already_generated_coins;
      alt_blocks.insert(std::make_pair(cryptonote::get_block_hash(bei.bl), std::move(bei)));
    }
    else
      MERROR("Failed to parse block from blob");
    return true;
  }, true);

  for (const auto &i: alt_blocks)
  {
    const crypto::hash top = cryptonote::get_block_hash(i.second.bl);
    bool found = false;
    for (const auto &j: alt_blocks)
    {
      if (j.second.bl.prev_id == top)
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      std::vector<crypto::hash> chain;
      auto h = i.second.bl.prev_id;
      chain.push_back(top);
      blocks_ext_by_hash::const_iterator prev;
      while ((prev = alt_blocks.find(h)) != alt_blocks.end())
      {
        chain.push_back(h);
        h = prev->second.bl.prev_id;
      }
      chains.push_back(std::make_pair(i.second, chain));
    }
  }
  return chains;
}

void Blockchain::cancel()
{
  m_cancel = true;
}

#if defined(PER_BLOCK_CHECKPOINT)
static const char expected_block_hashes_hash[] = "8ada865350270fd008397684d978dac75ea4029a8a1ffcaa9975c43be119ec19";
void Blockchain::load_compiled_in_block_hashes(const GetCheckpointsCallback& get_checkpoints)
{
  if (get_checkpoints == nullptr || !m_fast_sync)
  {
    return;
  }
  const epee::span<const unsigned char> &checkpoints = get_checkpoints(m_nettype);
  if (!checkpoints.empty())
  {
    MINFO("Loading precomputed blocks (" << checkpoints.size() << " bytes)");
    if (m_nettype == MAINNET)
    {
       
      crypto::hash hash;
      if (!tools::sha256sum(checkpoints.data(), checkpoints.size(), hash))
      {
        MERROR("Failed to hash precomputed blocks data");
        return;
      }
      MINFO("precomputed blocks hash: " << hash << ", expected " << expected_block_hashes_hash);
      cryptonote::blobdata expected_hash_data;
      if (!epee::string_tools::parse_hexstr_to_binbuff(std::string(expected_block_hashes_hash), expected_hash_data) || expected_hash_data.size() != sizeof(crypto::hash))
      {
        MERROR("Failed to parse expected block hashes hash");
        return;
      }
      const crypto::hash expected_hash = *reinterpret_cast<const crypto::hash*>(expected_hash_data.data());
      if (hash != expected_hash)
      {
        MERROR("Block hash data does not match expected hash");
        return;
      }
    }

    if (checkpoints.size() > 4)
    {
      const unsigned char *p = checkpoints.data();
      const uint32_t nblocks = *p | ((*(p+1))<<8) | ((*(p+2))<<16) | ((*(p+3))<<24);
      if (nblocks > (std::numeric_limits<uint32_t>::max() - 4) / sizeof(hash))
      {
        MERROR("Block hash data is too large");
        return;
      }
      const size_t size_needed = 4 + nblocks * (sizeof(crypto::hash) * 2);
      if(checkpoints.size() != size_needed)
      {
        MERROR("Failed to load hashes - unexpected data size");
        return;
      }
      else if(nblocks > 0 && nblocks > (m_db->height() + HASH_OF_HASHES_STEP - 1) / HASH_OF_HASHES_STEP)
      {
        p += sizeof(uint32_t);
        m_blocks_hash_of_hashes.reserve(nblocks);
        for (uint32_t i = 0; i < nblocks; i++)
        {
          crypto::hash hash_hashes, hash_weights;
          memcpy(hash_hashes.data, p, sizeof(hash_hashes.data));
          p += sizeof(hash_hashes.data);
          memcpy(hash_weights.data, p, sizeof(hash_weights.data));
          p += sizeof(hash_weights.data);
          m_blocks_hash_of_hashes.push_back(std::make_pair(hash_hashes, hash_weights));
        }
        m_blocks_hash_check.resize(m_blocks_hash_of_hashes.size() * HASH_OF_HASHES_STEP, std::make_pair(crypto::null_hash, 0));
        MINFO(nblocks << " block hashes loaded");

         
         
         
         
         
        CRITICAL_REGION_LOCAL(m_tx_pool);

        std::vector<transaction> txs;
        m_tx_pool.get_transactions(txs, true);

        size_t tx_weight;
        uint64_t fee;
        bool relayed, do_not_relay, double_spend_seen, pruned;
        transaction pool_tx;
        blobdata txblob;
        for(const transaction &tx : txs)
        {
          crypto::hash tx_hash = get_transaction_hash(tx);
          m_tx_pool.take_tx(tx_hash, pool_tx, txblob, tx_weight, fee, relayed, do_not_relay, double_spend_seen, pruned);
        }
      }
    }
  }
}
#endif

bool Blockchain::is_within_compiled_block_hash_area(uint64_t height) const
{
#if defined(PER_BLOCK_CHECKPOINT)
  return height < m_blocks_hash_of_hashes.size() * HASH_OF_HASHES_STEP;
#else
  return false;
#endif
}

void Blockchain::lock()
{
  m_blockchain_lock.lock();
}

void Blockchain::unlock()
{
  m_blockchain_lock.unlock();
}

bool Blockchain::for_all_key_images(std::function<bool(const crypto::key_image&)> f) const
{
  return m_db->for_all_key_images(f);
}

bool Blockchain::for_blocks_range(const uint64_t& h1, const uint64_t& h2, std::function<bool(uint64_t, const crypto::hash&, const block&)> f) const
{
  return m_db->for_blocks_range(h1, h2, f);
}

bool Blockchain::for_all_transactions(std::function<bool(const crypto::hash&, const cryptonote::transaction&)> f, bool pruned) const
{
  return m_db->for_all_transactions(f, pruned);
}

bool Blockchain::for_all_outputs(std::function<bool(uint64_t amount, const crypto::hash &tx_hash, uint64_t height, size_t tx_idx)> f) const
{
  return m_db->for_all_outputs(f);
}

bool Blockchain::for_all_outputs(uint64_t amount, std::function<bool(uint64_t height)> f) const
{
  return m_db->for_all_outputs(amount, f);
}

void Blockchain::invalidate_block_template_cache()
{
  MDEBUG("Invalidating block template cache");
  m_btc_valid = false;
}

void Blockchain::cache_block_template(const block &b, const cryptonote::account_public_address &address, const blobdata &nonce, const difficulty_type &diff, uint64_t height, uint64_t expected_reward, uint64_t seed_height, const crypto::hash &seed_hash, uint64_t pool_cookie)
{
  MDEBUG("Setting block template cache");
  m_btc = b;
  m_btc_address = address;
  m_btc_nonce = nonce;
  m_btc_difficulty = diff;
  m_btc_height = height;
  m_btc_expected_reward = expected_reward;
  m_btc_seed_hash = seed_hash;
  m_btc_seed_height = seed_height;
  m_btc_pool_cookie = pool_cookie;
  m_btc_valid = true;
}

void Blockchain::send_miner_notifications(uint64_t height, const crypto::hash &seed_hash, const crypto::hash &prev_id, uint64_t already_generated_coins)
{
  if (m_miner_notifiers.empty())
    return;

  const uint8_t major_version = m_hardfork->get_ideal_version(height);
  const difficulty_type diff = get_difficulty_for_next_block();
  const uint64_t median_weight = m_current_block_cumul_weight_median;

  std::vector<tx_block_template_backlog_entry> tx_backlog;
  m_tx_pool.get_block_template_backlog(tx_backlog);

  for (const auto& notifier : m_miner_notifiers)
  {
    notifier(major_version, height, prev_id, seed_hash, diff, median_weight, already_generated_coins, tx_backlog);
  }
}

namespace cryptonote {
template bool Blockchain::get_transactions(const std::vector<crypto::hash>&, std::vector<transaction>&, std::vector<crypto::hash>&, bool) const;
template bool Blockchain::get_split_transactions_blobs(const std::vector<crypto::hash>&, std::vector<std::tuple<crypto::hash, cryptonote::blobdata, crypto::hash, cryptonote::blobdata>>&, std::vector<crypto::hash>&) const;
}
