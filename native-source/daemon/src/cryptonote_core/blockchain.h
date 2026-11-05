                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#pragma once
#include <boost/asio/io_service.hpp>
#include <boost/function/function_fwd.hpp>
#if BOOST_VERSION >= 107400
#include <boost/serialization/library_version_type.hpp>
#endif
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/list.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/global_fun.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <atomic>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "span.h"
#include "syncobj.h"
#include "string_tools.h"
#include "rolling_median.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "common/powerof.h"
#include "common/util.h"
#include "cryptonote_protocol/cryptonote_protocol_defs.h"
#include "rpc/core_rpc_server_commands_defs.h"
#include "cryptonote_basic/difficulty.h"
#include "cryptonote_tx_utils.h"
#include "tx_verification_utils.h"
#include "cryptonote_basic/verification_context.h"
#include "crypto/hash.h"
#include "checkpoints/checkpoints.h"
#include "cryptonote_basic/hardfork.h"
#include "blockchain_db/blockchain_db.h"

namespace tools { class Notify; }

namespace cryptonote
{
  class tx_memory_pool;
  struct test_options;

   
 
 
  enum blockchain_db_sync_mode
  {
    db_defaultsync,  
    db_sync,   
    db_async,  
    db_nosync  
  };

   
 
 
 
 
 
 
  typedef std::function<const epee::span<const unsigned char>(cryptonote::network_type network)> GetCheckpointsCallback;

  typedef boost::function<void(uint64_t  , epee::span<const block>  )> BlockNotifyCallback;
  typedef boost::function<void(uint8_t  , uint64_t  , const crypto::hash&  , const crypto::hash&  , difficulty_type  , uint64_t  , uint64_t  , const std::vector<tx_block_template_backlog_entry>&  )> MinerNotifyCallback;

   
   
   
  class Blockchain
  {
  public:
     
 
 
    struct block_extended_info
    {
      block   bl;  
      uint64_t height;  
      uint64_t block_cumulative_weight;  
      difficulty_type cumulative_difficulty;  
      uint64_t already_generated_coins;  
    };

     
 
 
 
 
    Blockchain(tx_memory_pool& tx_pool);

     
 
 
    ~Blockchain();

     
 
 
 
 
 
 
 
 
 
 
 
    bool init(BlockchainDB* db, const network_type nettype = MAINNET, bool offline = false, const cryptonote::test_options *test_options = NULL, difficulty_type fixed_difficulty = 0, const GetCheckpointsCallback& get_checkpoints = nullptr);

     
 
 
 
 
 
 
 
 
 
    bool init(BlockchainDB* db, HardFork*& hf, const network_type nettype = MAINNET, bool offline = false);

     
 
 
 
 
 
 
    bool deinit();

     
 
 
 
 
    const checkpoints& get_checkpoints() const { return m_checkpoints; }

     
 
 
 
 
    void set_checkpoints(checkpoints&& chk_pts) { m_checkpoints = chk_pts; }

     
 
 
 
 
 
 
 
 
 
    bool get_blocks(uint64_t start_offset, size_t count, std::vector<std::pair<cryptonote::blobdata,block>>& blocks, std::vector<cryptonote::blobdata>& txs) const;

     
 
 
 
 
 
 
 
 
    bool get_blocks(uint64_t start_offset, size_t count, std::vector<std::pair<cryptonote::blobdata,block>>& blocks) const;

     
 
 
 
 
 
 
    bool get_alternative_blocks(std::vector<block>& blocks) const;

     
 
 
 
 
    size_t get_alternative_blocks_count() const;

     
 
 
 
 
 
 
    crypto::hash get_block_id_by_height(uint64_t height) const;

     
 
 
 
 
 
 
 
 
 
    crypto::hash get_pending_block_id_by_height(uint64_t height) const;

     
 
 
 
 
 
 
 
 
    bool get_block_by_hash(const crypto::hash &h, block &blk, bool *orphan = NULL) const;

     
 
 
 
 
 
 
 
    bool prepare_handle_incoming_blocks(const std::vector<block_complete_entry>  &blocks_entry, std::vector<block> &blocks);

     
 
 
 
 
 
 
    bool cleanup_handle_incoming_blocks(bool force_sync = false);

     
 
 
 
 
 
 
    bool have_tx(const crypto::hash &id) const;

     
 
 
 
 
 
 
    bool have_tx_keyimges_as_spent(const transaction &tx) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool have_tx_keyimg_as_spent(const crypto::key_image &key_im) const;

     
 
 
 
 
    uint64_t get_current_blockchain_height() const;

     
 
 
 
 
    crypto::hash get_tail_id() const;

     
 
 
 
 
 
 
    crypto::hash get_tail_id(uint64_t& height) const;

     
 
 
 
 
    difficulty_type get_difficulty_for_next_block();

     
 
 
 
 
    std::pair<bool, uint64_t> check_difficulty_checkpoints() const;

     
 
 
 
 
 
 
    size_t recalculate_difficulties(boost::optional<uint64_t> start_height = boost::none);

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool add_new_block(const block& bl_, block_verification_context& bvc);

     
 
 
 
 
 
 
    bool reset_and_set_genesis_block(const block& b);

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool create_block_template(block& b, const account_public_address& miner_address, difficulty_type& di, uint64_t& height, uint64_t& expected_reward, const blobdata& ex_nonce, uint64_t &seed_height, crypto::hash &seed_hash);
    bool create_block_template(block& b, const crypto::hash *from_block, const account_public_address& miner_address, difficulty_type& di, uint64_t& height, uint64_t& expected_reward, const blobdata& ex_nonce, uint64_t &seed_height, crypto::hash &seed_hash, bool include_sensitive = true);

     
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool get_miner_data(uint8_t& major_version, uint64_t& height, crypto::hash& prev_id, crypto::hash& seed_hash, difficulty_type& difficulty, uint64_t& median_weight, uint64_t& already_generated_coins, std::vector<tx_block_template_backlog_entry>& tx_backlog);

     
 
 
 
 
 
 
 
 
 
 
    bool have_block_unlocked(const crypto::hash& id, int *where = NULL) const;
    bool have_block(const crypto::hash& id, int *where = NULL) const;

     
 
 
 
 
    size_t get_total_transactions() const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool get_short_chain_history(std::list<crypto::hash>& ids) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, std::vector<crypto::hash>& hashes, std::vector<uint64_t>* weights, uint64_t& start_height, uint64_t& current_height, bool clip_pruned) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, bool clip_pruned, NOTIFY_RESPONSE_CHAIN_ENTRY::request& resp) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, uint64_t& starter_offset) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool find_blockchain_supplement(const uint64_t req_start_block, const std::list<crypto::hash>& qblock_ids, std::vector<std::pair<std::pair<cryptonote::blobdata, crypto::hash>, std::vector<std::pair<crypto::hash, cryptonote::blobdata> > > >& blocks, uint64_t& total_height, uint64_t& start_height, bool pruned, bool get_miner_tx_hash, size_t max_block_count, size_t max_tx_count) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool handle_get_objects(NOTIFY_REQUEST_GET_OBJECTS::request& arg, NOTIFY_RESPONSE_GET_OBJECTS::request& rsp);

     
 
 
 
 
 
 
    uint64_t get_num_mature_outputs(uint64_t amount) const;

     
 
 
 
 
 
 
 
    crypto::public_key get_output_key(uint64_t amount, uint64_t global_index) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool get_outs(const COMMAND_RPC_GET_OUTPUTS_BIN::request& req, COMMAND_RPC_GET_OUTPUTS_BIN::response& res) const;

     
 
 
 
 
 
 
 
 
    void get_output_key_mask_unlocked(const uint64_t& amount, const uint64_t& index, crypto::public_key& key, rct::key& mask, bool& unlocked) const;

     
 
 
 
 
 
 
 
 
 
    bool get_output_distribution(uint64_t amount, uint64_t from_height, uint64_t to_height, uint64_t &start_height, std::vector<uint64_t> &distribution, uint64_t &base) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool get_tx_outputs_gindexs(const crypto::hash& tx_id, std::vector<uint64_t>& indexs) const;
    bool get_tx_outputs_gindexs(const crypto::hash& tx_id, size_t n_txes, std::vector<std::vector<uint64_t>>& indexs) const;

     
 
 
 
 
 
 
 
    bool store_blockchain();

     
 
 
 
 
 
 
    static bool expand_transaction_2(transaction &tx, const crypto::hash &tx_prefix_hash, const std::vector<std::vector<rct::ctkey>> &pubkeys);

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool check_tx_inputs(transaction& tx, uint64_t& pmax_used_block_height, crypto::hash& max_used_block_id, tx_verification_context &tvc, bool kept_by_block = false) const;

     
 
 
 
 
 
 
    static uint64_t get_fee_quantization_mask()
    {
      return tools::PowerOf<10, CRYPTONOTE_DISPLAY_DECIMAL_POINT - PER_KB_FEE_QUANTIZATION_DECIMALS>::Value;
    }

     
 
 
 
 
 
 
 
 
 
 
 
 
    static uint64_t get_dynamic_base_fee(uint64_t block_reward, size_t median_block_weight, uint8_t version);

     
 
 
 
 
 
 
 
 
 
 
 
 
    uint64_t get_dynamic_base_fee_estimate(uint64_t grace_blocks) const;
    void get_dynamic_base_fee_estimate_2021_scaling(uint64_t grace_blocks, uint64_t base_reward, uint64_t Mnw, uint64_t Mlw, std::vector<uint64_t> &fees) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
    void get_dynamic_base_fee_estimate_2021_scaling(uint64_t grace_blocks, std::vector<uint64_t> &fees) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool check_fee(size_t tx_weight, uint64_t fee) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool check_tx_outputs(const transaction& tx, tx_verification_context &tvc) const;

     
 
 
 
 
    uint64_t get_current_cumulative_block_weight_limit() const;

     
 
 
 
 
    uint64_t get_next_long_term_block_weight(uint64_t block_weight) const;

     
 
 
 
 
    uint64_t get_current_cumulative_block_weight_median() const;

     
 
 
 
 
 
 
    difficulty_type block_difficulty(uint64_t i) const;

     
 
 
 
 
 
 
 
 
 
 
 
    template<class t_ids_container, class t_blocks_container, class t_missed_container>
    bool get_blocks(const t_ids_container& block_ids, t_blocks_container& blocks, t_missed_container& missed_bs) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool get_transactions_blobs(const std::vector<crypto::hash>& txs_ids, std::vector<cryptonote::blobdata>& txs, std::vector<crypto::hash>& missed_txs, bool pruned = false) const;
    bool get_transactions_blobs(const std::vector<crypto::hash>& txs_ids, std::vector<tx_blob_entry>& txs, std::vector<crypto::hash>& missed_txs, bool pruned = false) const;
    template<class t_ids_container, class t_tx_container, class t_missed_container>
    bool get_split_transactions_blobs(const t_ids_container& txs_ids, t_tx_container& txs, t_missed_container& missed_txs) const;
    template<class t_ids_container, class t_tx_container, class t_missed_container>
    bool get_transactions(const t_ids_container& txs_ids, t_tx_container& txs, t_missed_container& missed_txs, bool pruned = false) const;

     

     
 
 
 
 
 
 
 
 
 
 
    void check_against_checkpoints(const checkpoints& points, bool enforce);

     
 
 
 
 
    void set_enforce_dns_checkpoints(bool enforce);

     
 
 
 
 
 
 
 
    bool update_checkpoints(const std::string& file_path, bool check_dns);


     

     
 
 
 
 
 
 
 
 
    void set_user_options(uint64_t maxthreads, bool sync_on_blocks, uint64_t sync_threshold,
        blockchain_db_sync_mode sync_mode, bool fast_sync);

     
 
 
 
 
    void add_block_notify(BlockNotifyCallback&& notify);

     
 
 
 
 
    void add_miner_notify(MinerNotifyCallback&& notify);

     
 
 
 
 
    void set_reorg_notify(const std::shared_ptr<tools::Notify> &notify) { m_reorg_notify = notify; }

     
 
 
    void safesyncmode(const bool onoff);

     
 
 
 
 
    void set_show_time_stats(bool stats) { m_show_time_stats = stats; }

     
 
 
 
 
    HardFork::State get_hard_fork_state() const;

     
 
 
 
 
    uint8_t get_current_hard_fork_version() const { return m_hardfork->get_current_version(); }

     
 
 
 
 
    uint8_t get_ideal_hard_fork_version() const { return m_hardfork->get_ideal_version(); }

     
 
 
 
 
    uint8_t get_next_hard_fork_version() const { return m_hardfork->get_next_version(); }

     
 
 
 
 
 
 
 
    uint8_t get_ideal_hard_fork_version(uint64_t height) const { return m_hardfork->get_ideal_version(height); }

     
 
 
 
 
 
 
    uint8_t get_hard_fork_version(uint64_t height) const { return m_hardfork->get(height); }

     
 
 
 
 
    uint64_t get_earliest_ideal_height_for_version(uint8_t version) const { return m_hardfork->get_earliest_ideal_height_for_version(version); }

     
 
 
 
 
    const std::vector<hardfork_t>& get_hardforks() const { return m_hardfork->get_hardforks(); }

     
 
 
 
 
 
 
 
 
 
 
 
    bool get_hard_fork_voting_info(uint8_t version, uint32_t &window, uint32_t &votes, uint32_t &threshold, uint64_t &earliest_height, uint8_t &voting) const;

     
 
 
 
 
    uint64_t get_difficulty_target() const;

     
 
 
 
 
 
 
    bool flush_txes_from_pool(const std::vector<crypto::hash> &txids);

     
 
 
 
 
 
 
 
 
 
    std::map<uint64_t, std::tuple<uint64_t, uint64_t, uint64_t>> get_output_histogram(const std::vector<uint64_t> &amounts, bool unlocked, uint64_t recent_cutoff, uint64_t min_count = 0) const;

     
 
 
 
 
 
 
    bool for_all_key_images(std::function<bool(const crypto::key_image&)>) const;

     
 
 
 
 
 
 
 
 
    bool for_blocks_range(const uint64_t& h1, const uint64_t& h2, std::function<bool(uint64_t, const crypto::hash&, const block&)>) const;

     
 
 
 
 
 
 
 
    bool for_all_transactions(std::function<bool(const crypto::hash&, const cryptonote::transaction&)>, bool pruned) const;

     
 
 
 
 
 
 
    bool for_all_outputs(std::function<bool(uint64_t amount, const crypto::hash &tx_hash, uint64_t height, size_t tx_idx)>) const;

     
 
 
 
 
 
 
 
    bool for_all_outputs(uint64_t amount, std::function<bool(uint64_t height)>) const;

     
 
 
 
 
    const BlockchainDB& get_db() const
    {
      return *m_db;
    }

     
 
 
 
 
    BlockchainDB& get_db()
    {
      return *m_db;
    }

     
 
 
 
 
 
 
    void output_scan_worker(const uint64_t amount,const std::vector<uint64_t> &offsets,
        std::vector<output_data_t> &outputs) const;

     
 
 
 
 
 
 
    void block_longhash_worker(uint64_t height, const epee::span<const block> &blocks,
        std::unordered_map<crypto::hash, crypto::hash> &map) const;

     
 
 
 
 
    std::vector<std::pair<block_extended_info,std::vector<crypto::hash>>> get_alternative_chains() const;

    void add_txpool_tx(const crypto::hash &txid, const cryptonote::blobdata &blob, const txpool_tx_meta_t &meta);
    void update_txpool_tx(const crypto::hash &txid, const txpool_tx_meta_t &meta);
    void remove_txpool_tx(const crypto::hash &txid);
    uint64_t get_txpool_tx_count(bool include_sensitive = false) const;
    bool get_txpool_tx_meta(const crypto::hash& txid, txpool_tx_meta_t &meta) const;
    bool get_txpool_tx_blob(const crypto::hash& txid, cryptonote::blobdata &bd, relay_category tx_category) const;
    cryptonote::blobdata get_txpool_tx_blob(const crypto::hash& txid, relay_category tx_category) const;
    bool for_all_txpool_txes(std::function<bool(const crypto::hash&, const txpool_tx_meta_t&, const cryptonote::blobdata_ref*)>, bool include_blob = false, relay_category tx_category = relay_category::broadcasted) const;
    bool txpool_tx_matches_category(const crypto::hash& tx_hash, relay_category category);

    bool is_within_compiled_block_hash_area() const { return is_within_compiled_block_hash_area(m_db->height()); }
    uint64_t prevalidate_block_hashes(uint64_t height, const std::vector<crypto::hash> &hashes, const std::vector<uint64_t> &weights);
    uint32_t get_blockchain_pruning_seed() const { return m_db->get_blockchain_pruning_seed(); }
    bool prune_blockchain(uint32_t pruning_seed = 0);
    bool update_blockchain_pruning();
    bool check_blockchain_pruning();

    void lock();
    void unlock();

    void cancel();

     
 
 
 
 
    void on_new_tx_from_block(const cryptonote::transaction &tx);

     
 
 
    std::vector<time_t> get_last_block_timestamps(unsigned int blocks) const;

     
 
 
 
 
    void pop_blocks(uint64_t nblocks);

     
 
 
 
 
    bool is_within_compiled_block_hash_area(uint64_t height) const;

     
 
 
 
 
 
    bool has_block_weights(uint64_t height, uint64_t nblocks) const;

     
 
 
    void flush_invalid_blocks();

     
 
 
 
 
 
 
 
 
 
 
 
 
    uint64_t get_adjusted_time(uint64_t height) const;

#ifndef IN_UNIT_TESTS
  private:
#endif

     
    typedef std::unordered_set<crypto::key_image> key_images_container;

    typedef std::vector<block_extended_info> blocks_container;

    typedef std::unordered_map<crypto::hash, block_extended_info> blocks_ext_by_hash;


    BlockchainDB* m_db;

    tx_memory_pool& m_tx_pool;

    mutable epee::critical_section m_blockchain_lock;  

     
    size_t m_current_block_cumul_weight_limit;
    size_t m_current_block_cumul_weight_median;

     
    std::unordered_map<crypto::hash, std::unordered_map<crypto::key_image, std::vector<output_data_t>>> m_scan_table;
    std::unordered_map<crypto::hash, crypto::hash> m_blocks_longhash_table;

     
    std::vector<std::pair<crypto::hash, crypto::hash>> m_blocks_hash_of_hashes;
    std::vector<std::pair<crypto::hash, uint64_t>> m_blocks_hash_check;
    std::vector<crypto::hash> m_blocks_txs_check;

    blockchain_db_sync_mode m_db_sync_mode;
    bool m_fast_sync;
    bool m_show_time_stats;
    bool m_db_default_sync;
    bool m_db_sync_on_blocks;
    uint64_t m_db_sync_threshold;
    uint64_t m_max_prepare_blocks_threads;
    uint64_t m_fake_pow_calc_time;
    uint64_t m_fake_scan_time;
    uint64_t m_sync_counter;
    uint64_t m_bytes_to_sync;
    std::vector<uint64_t> m_timestamps;
    std::vector<difficulty_type> m_difficulties;
    uint64_t m_timestamps_and_difficulties_height;
    bool m_reset_timestamps_and_difficulties_height;
    uint64_t m_long_term_block_weights_window;
    uint64_t m_long_term_effective_median_block_weight;
    mutable crypto::hash m_long_term_block_weights_cache_tip_hash;
    mutable epee::misc_utils::rolling_median_t<uint64_t> m_long_term_block_weights_cache_rolling_median;

    epee::critical_section m_difficulty_lock;
    crypto::hash m_difficulty_for_next_block_top_hash;
    difficulty_type m_difficulty_for_next_block;

    boost::asio::io_service m_async_service;
    boost::thread_group m_async_pool;
    std::unique_ptr<boost::asio::io_service::work> m_async_work_idle;

     
    blocks_ext_by_hash m_invalid_blocks;      


    checkpoints m_checkpoints;
    bool m_enforce_dns_checkpoints;

    HardFork *m_hardfork;

    network_type m_nettype;
    bool m_offline;
    difficulty_type m_fixed_difficulty;

    std::atomic<bool> m_cancel;

     
    block m_btc;
    account_public_address m_btc_address;
    blobdata m_btc_nonce;
    difficulty_type m_btc_difficulty;
    uint64_t m_btc_height;
    uint64_t m_btc_pool_cookie;
    uint64_t m_btc_expected_reward;
    crypto::hash m_btc_seed_hash;
    uint64_t m_btc_seed_height;
    bool m_btc_valid;


    bool m_batch_success;

     
 
 

    std::vector<BlockNotifyCallback> m_block_notifiers;
    std::vector<MinerNotifyCallback> m_miner_notifiers;
    std::shared_ptr<tools::Notify> m_reorg_notify;

     
    uint64_t m_prepare_height;
    uint64_t m_prepare_nblocks;
    std::vector<block> *m_prepare_blocks;

     
    mutable rct_ver_cache_t m_rct_ver_cache;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    template<class visitor_t>
    inline bool scan_outputkeys_for_indexes(size_t tx_version, const txin_to_key& tx_in_to_key, visitor_t &vis, const crypto::hash &tx_prefix_hash, uint64_t* pmax_related_block_height = NULL) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool check_tx_input(size_t tx_version,const txin_to_key& txin, const crypto::hash& tx_prefix_hash, const std::vector<crypto::signature>& sig, const rct::rctSig &rct_signatures, std::vector<rct::ctkey> &output_keys, uint64_t* pmax_related_block_height, uint8_t hf_version) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool check_tx_inputs(transaction& tx, tx_verification_context &tvc, uint64_t* pmax_used_block_height = NULL) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool switch_to_alternative_blockchain(std::list<block_extended_info>& alt_chain, bool discard_disconnected_chain);

     
 
 
 
 
    block pop_block_from_blockchain();

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool handle_block_to_main_chain(const block& bl, block_verification_context& bvc, bool notify = true);

     
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool handle_block_to_main_chain(const block& bl, const crypto::hash& id, block_verification_context& bvc, bool notify = true);

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool handle_alternative_block(const block& b, const crypto::hash& id, block_verification_context& bvc);

     
 
 
 
 
 
 
 
 
 
    bool build_alt_chain(const crypto::hash &prev_id, std::list<block_extended_info>& alt_chain, std::vector<uint64_t> &timestamps, block_verification_context& bvc) const;

     
 
 
 
 
 
 
 
    difficulty_type get_next_difficulty_for_alternative_chain(const std::list<block_extended_info>& alt_chain, block_extended_info& bei) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool prevalidate_miner_transaction(const block& b, uint64_t height, uint8_t hf_version);

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool validate_miner_transaction(const block& b, size_t cumulative_block_weight, uint64_t fee, uint64_t& base_reward, uint64_t already_generated_coins, bool &partial_block_reward, uint8_t version);

     
 
 
 
 
 
 
 
 
 
 
 
    bool rollback_blockchain_switching(std::list<block>& original_chain, uint64_t rollback_height);

     
 
 
 
 
 
 
 
    void get_last_n_blocks_weights(std::vector<uint64_t>& weights, size_t count) const;

     
 
 
 
 
 
 
 
 
 
    uint64_t get_long_term_block_weight_median(uint64_t start_height, size_t count) const;

     
 
 
 
 
 
 
 
 
 
 
    bool is_tx_spendtime_unlocked(uint64_t unlock_time, uint8_t hf_version) const;

     
 
 
 
 
 
 
 
 
 
 
    bool add_block_as_invalid(const block& bl, const crypto::hash& h);

     
 
 
 
 
 
 
 
 
 
 
    bool add_block_as_invalid(const block_extended_info& bei, const crypto::hash& h);

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool check_block_timestamp(const block& b, uint64_t& median_ts) const;
    bool check_block_timestamp(const block& b) const { uint64_t median_ts; return check_block_timestamp(b, median_ts); }

     
 
 
 
 
 
 
 
 
 
 
    bool check_block_timestamp(std::vector<uint64_t>& timestamps, const block& b, uint64_t& median_ts) const;
    bool check_block_timestamp(std::vector<uint64_t>& timestamps, const block& b) const { uint64_t median_ts; return check_block_timestamp(timestamps, b, median_ts); }

     
 
 
 
 
 
 
 
 
 
 
    bool complete_timestamps_vector(uint64_t start_height, std::vector<uint64_t>& timestamps) const;

     
 
 
 
 
 
 
    bool update_next_cumulative_weight_limit(uint64_t *long_term_effective_median_block_weight = NULL);
    void return_tx_to_pool(std::vector<std::pair<transaction, blobdata>> &txs);

     
 
 
 
 
 
 
 
    bool check_for_double_spend(const transaction& tx, key_images_container& keys_this_block) const;

     
 
 
 
 
 
 
 
 
    void check_ring_signature(const crypto::hash &tx_prefix_hash, const crypto::key_image &key_image,
        const std::vector<rct::ctkey> &pubkeys, const std::vector<crypto::signature> &sig, uint64_t &result) const;

     
 
 
 
 
 
 
 
 
    void load_compiled_in_block_hashes(const GetCheckpointsCallback& get_checkpoints);

     
 
 
    void invalidate_block_template_cache();

     
 
 
 
 
    void cache_block_template(const block &b, const cryptonote::account_public_address &address, const blobdata &nonce, const difficulty_type &diff, uint64_t height, uint64_t expected_reward, uint64_t seed_height, const crypto::hash &seed_hash, uint64_t pool_cookie);

     
 
 
 
 
 
 
 
    void send_miner_notifications(uint64_t height, const crypto::hash &seed_hash, const crypto::hash &prev_id, uint64_t already_generated_coins);
  };
}   
