                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#pragma once

#include <ctime>

#include <boost/function.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "cryptonote_basic/fwd.h"
#include "cryptonote_core/i_core_events.h"
#include "cryptonote_protocol/cryptonote_protocol_handler_common.h"
#include "cryptonote_protocol/enums.h"
#include "common/download.h"
#include "common/command_line.h"
#include "tx_pool.h"
#include "blockchain.h"
#include "cryptonote_basic/miner.h"
#include "cryptonote_basic/connection_context.h"
#include "warnings.h"
#include "crypto/hash.h"
#include "span.h"
#include "rpc/fwd.h"

PUSH_WARNINGS
DISABLE_VS_WARNINGS(4355)

enum { HAVE_BLOCK_MAIN_CHAIN, HAVE_BLOCK_ALT_CHAIN, HAVE_BLOCK_INVALID };

namespace cryptonote
{
   struct test_options {
     const std::pair<uint8_t, uint64_t> *hard_forks;
     const size_t long_term_block_weight_window;
   };

  extern const command_line::arg_descriptor<std::string, false, true, 2> arg_data_dir;
  extern const command_line::arg_descriptor<bool, false> arg_testnet_on;
  extern const command_line::arg_descriptor<bool, false> arg_stagenet_on;
  extern const command_line::arg_descriptor<bool, false> arg_regtest_on;
  extern const command_line::arg_descriptor<difficulty_type> arg_fixed_difficulty;
  extern const command_line::arg_descriptor<bool> arg_offline;
  extern const command_line::arg_descriptor<size_t> arg_block_download_max_size;
  extern const command_line::arg_descriptor<bool> arg_sync_pruned_blocks;

   
   
   

    
 
 
 
 
 
 
   class core final: public i_miner_handler, public i_core_events
   {
   public:

       
 
 
 
 
 
 
     core(i_cryptonote_protocol* pprotocol);

     
 
 
 
 
 
     bool handle_get_objects(NOTIFY_REQUEST_GET_OBJECTS::request& arg, NOTIFY_RESPONSE_GET_OBJECTS::request& rsp, cryptonote_connection_context& context);

      
 
 
 
 
 
 
     bool on_idle();

      
 
 
 
 
 
 
 
 
 
 
 
 
     bool handle_incoming_tx(const tx_blob_entry& tx_blob, tx_verification_context& tvc, relay_method tx_relay, bool relayed);

      
 
 
 
 
 
 
 
 
 
 
 
 
 
 
     bool handle_incoming_txs(epee::span<const tx_blob_entry> tx_blobs, epee::span<tx_verification_context> tvc, relay_method tx_relay, bool relayed);

      
 
 
 
 
 
 
 
 
 
 
 
 
     bool handle_incoming_txs(const std::vector<tx_blob_entry>& tx_blobs, std::vector<tx_verification_context>& tvc, relay_method tx_relay, bool relayed)
     {
       tvc.resize(tx_blobs.size());
       return handle_incoming_txs(epee::to_span(tx_blobs), epee::to_mut_span(tvc), tx_relay, relayed);
     }

      
 
 
 
 
 
 
 
 
 
 
 
 
 
 
     bool handle_incoming_block(const blobdata& block_blob, const block *b, block_verification_context& bvc, bool update_miner_blocktemplate = true);

      
 
 
 
 
     bool prepare_handle_incoming_blocks(const std::vector<block_complete_entry> &blocks_entry, std::vector<block> &blocks);

      
 
 
 
 
     bool cleanup_handle_incoming_blocks(bool force_sync = false);
     	     	
      
 
 
 
 
 
 
     bool check_incoming_block_size(const blobdata& block_blob) const;

      
 
 
 
 
     i_cryptonote_protocol* get_protocol(){return m_pprotocol;}

      

      
 
 
 
 
 
 
 
 
 
 
 
     virtual bool handle_block_found(block& b, block_verification_context &bvc) override;


      
 
 
 
 
     virtual bool get_block_template(block& b, const account_public_address& adr, difficulty_type& diffic, uint64_t& height, uint64_t& expected_reward, const blobdata& ex_nonce, uint64_t &seed_height, crypto::hash &seed_hash) override;
     virtual bool get_block_template(block& b, const crypto::hash *prev_block, const account_public_address& adr, difficulty_type& diffic, uint64_t& height, uint64_t& expected_reward, const blobdata& ex_nonce, uint64_t &seed_height, crypto::hash &seed_hash, bool include_sensitive = true);

      
 
 
 
 
     bool get_miner_data(uint8_t& major_version, uint64_t& height, crypto::hash& prev_id, crypto::hash& seed_hash, difficulty_type& difficulty, uint64_t& median_weight, uint64_t& already_generated_coins, std::vector<tx_block_template_backlog_entry>& tx_backlog);

      
 
 
 
     virtual void on_transactions_relayed(epee::span<const cryptonote::blobdata> tx_blobs, relay_method tx_relay) final;


      
 
 
 
 
     miner& get_miner(){return m_miner;}

      
 
 
 
 
     const miner& get_miner()const{return m_miner;}

      
 
 
 
 
 
 
 
     static void init_options(boost::program_options::options_description& desc);

      
 
 
 
 
 
 
 
 
 
 
 
 
     bool init(const boost::program_options::variables_map& vm, const test_options *test_options = NULL, const GetCheckpointsCallback& get_checkpoints = nullptr, bool allow_dns = true);

      
 
 
 
 
     bool set_genesis_block(const block& b);

      
 
 
 
 
 
 
     bool deinit();

      
 
 
     void test_drop_download();

      
 
 
 
 
     void test_drop_download_height(uint64_t height);

      
 
 
 
 
     bool get_test_drop_download() const;

      
 
 
 
 
 
 
 
     bool get_test_drop_download_height() const;

      
 
 
 
 
     virtual uint64_t get_current_blockchain_height() const final;

      
 
 
 
 
 
     void get_blockchain_top(uint64_t& height, crypto::hash& top_id) const;

      
 
 
 
 
     bool get_blocks(uint64_t start_offset, size_t count, std::vector<std::pair<cryptonote::blobdata,block>>& blocks, std::vector<cryptonote::blobdata>& txs) const;

      
 
 
 
 
     bool get_blocks(uint64_t start_offset, size_t count, std::vector<std::pair<cryptonote::blobdata,block>>& blocks) const;

      
 
 
 
 
     bool get_blocks(uint64_t start_offset, size_t count, std::vector<block>& blocks) const;

      
 
 
 
 
     template<class t_ids_container, class t_blocks_container, class t_missed_container>
     bool get_blocks(const t_ids_container& block_ids, t_blocks_container& blocks, t_missed_container& missed_bs) const
     {
       return m_blockchain_storage.get_blocks(block_ids, blocks, missed_bs);
     }

      
 
 
 
 
     crypto::hash get_block_id_by_height(uint64_t height) const;

      
 
 
 
 
     bool get_transactions(const std::vector<crypto::hash>& txs_ids, std::vector<cryptonote::blobdata>& txs, std::vector<crypto::hash>& missed_txs, bool pruned = false) const;

      
 
 
 
 
     bool get_split_transactions_blobs(const std::vector<crypto::hash>& txs_ids, std::vector<std::tuple<crypto::hash, cryptonote::blobdata, crypto::hash, cryptonote::blobdata>>& txs, std::vector<crypto::hash>& missed_txs) const;

      
 
 
 
 
     bool get_transactions(const std::vector<crypto::hash>& txs_ids, std::vector<transaction>& txs, std::vector<crypto::hash>& missed_txs, bool pruned = false) const;

      
 
 
 
 
     bool get_block_by_hash(const crypto::hash &h, block &blk, bool *orphan = NULL) const;

      
 
 
 
 
     bool get_alternative_blocks(std::vector<block>& blocks) const;

      
 
 
 
 
     size_t get_alternative_blocks_count() const;

      
 
 
 
 
     void set_cryptonote_protocol(i_cryptonote_protocol* pprotocol);

      
 
 
 
 
     const checkpoints& get_checkpoints() const;

      
 
 
 
 
     void set_checkpoints(checkpoints&& chk_pts);

      
 
 
 
 
     void set_checkpoints_file_path(const std::string& path);

      
 
 
 
 
     void set_enforce_dns_checkpoints(bool enforce_dns);

      
 
 
 
 
     void set_txpool_listener(boost::function<void(std::vector<txpool_event>)> zmq_pub);

      
 
 
 
 
     void disable_dns_checkpoints(bool disable = true) { m_disable_dns_checkpoints = disable; }

      
 
 
 
 
     bool pool_has_tx(const crypto::hash &txid) const;

      
 
 
 
 
 
     bool get_pool_transactions(std::vector<transaction>& txs, bool include_sensitive_txes = false) const;

      
 
 
 
 
 
     bool get_txpool_backlog(std::vector<tx_backlog_entry>& backlog, bool include_sensitive_txes = false) const;
     
      
 
 
 
 
 
     bool get_pool_transaction_hashes(std::vector<crypto::hash>& txs, bool include_sensitive_txes = false) const;

      
 
 
 
 
 
     bool get_pool_transactions_info(const std::vector<crypto::hash>& txids, std::vector<std::pair<crypto::hash, tx_memory_pool::tx_details>>& txs, bool include_sensitive_txes = false) const;

      
 
 
 
 
 
 
     bool get_pool_info(time_t start_time, bool include_sensitive_txes, size_t max_tx_count, std::vector<std::pair<crypto::hash, tx_memory_pool::tx_details>>& added_txs, std::vector<crypto::hash>& remaining_added_txids, std::vector<crypto::hash>& removed_txs, bool& incremental) const;

     
 
 
 
 
 
     bool get_pool_transaction_stats(struct txpool_stats& stats, bool include_sensitive_txes = false) const;

      
 
 
 
 
     bool get_pool_transaction(const crypto::hash& id, cryptonote::blobdata& tx, relay_category tx_category) const;

      
 
 
 
 
 
     bool get_pool_transactions_and_spent_keys_info(std::vector<tx_info>& tx_infos, std::vector<spent_key_image_info>& key_image_infos, bool include_sensitive_txes = false) const;

      
 
 
 
 
 
     bool get_pool_for_rpc(std::vector<cryptonote::rpc::tx_in_pool>& tx_infos, cryptonote::rpc::key_images_with_tx_hashes& key_image_infos, bool include_sensitive) const;

      
 
 
 
 
 
     size_t get_pool_transactions_count(bool include_sensitive_txes = false) const;

      
 
 
 
 
     size_t get_blockchain_total_transactions() const;

      
 
 
 
 
     bool have_block_unlocked(const crypto::hash& id, int *where = NULL) const;
     bool have_block(const crypto::hash& id, int *where = NULL) const;

      
 
 
 
 
     bool get_short_chain_history(std::list<crypto::hash>& ids) const;

      
 
 
 
 
     bool find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, bool clip_pruned, NOTIFY_RESPONSE_CHAIN_ENTRY::request& resp) const;

      
 
 
 
 
     bool find_blockchain_supplement(const uint64_t req_start_block, const std::list<crypto::hash>& qblock_ids, std::vector<std::pair<std::pair<cryptonote::blobdata, crypto::hash>, std::vector<std::pair<crypto::hash, cryptonote::blobdata> > > >& blocks, uint64_t& total_height, uint64_t& start_height, bool pruned, bool get_miner_tx_hash, size_t max_block_count, size_t max_tx_count) const;

      
 
 
 
 
     bool get_tx_outputs_gindexs(const crypto::hash& tx_id, std::vector<uint64_t>& indexs) const;
     bool get_tx_outputs_gindexs(const crypto::hash& tx_id, size_t n_txes, std::vector<std::vector<uint64_t>>& indexs) const;

      
 
 
 
 
     crypto::hash get_tail_id() const;

      
 
 
 
 
     difficulty_type get_block_cumulative_difficulty(uint64_t height) const;

      
 
 
 
 
     bool get_outs(const COMMAND_RPC_GET_OUTPUTS_BIN::request& req, COMMAND_RPC_GET_OUTPUTS_BIN::response& res) const;

      
 
 
 
 
     bool get_output_distribution(uint64_t amount, uint64_t from_height, uint64_t to_height, uint64_t &start_height, std::vector<uint64_t> &distribution, uint64_t &base) const;

      
 
 
 
 
     void pause_mine();

      
 
 
 
 
     void resume_mine();

      
 
 
 
 
     Blockchain& get_blockchain_storage(){return m_blockchain_storage;}

      
 
 
 
 
     const Blockchain& get_blockchain_storage()const{return m_blockchain_storage;}

      
 
 
 
 
     std::string print_pool(bool short_format) const;

      
 
 
 
 
     virtual bool is_synchronized() const final;

      
 
 
 
 
     void on_synchronized();

      
 
 
 
 
     void safesyncmode(const bool onoff);

      
 
 
 
 
     void set_target_blockchain_height(uint64_t target_blockchain_height);

      
 
 
 
 
     uint64_t get_target_blockchain_height() const;

      
 
 
 
 
     uint8_t get_ideal_hard_fork_version() const;

      
 
 
 
 
     uint8_t get_ideal_hard_fork_version(uint64_t height) const;

      
 
 
 
 
     uint8_t get_hard_fork_version(uint64_t height) const;

      
 
 
 
 
     uint64_t get_earliest_ideal_height_for_version(uint8_t version) const;

      
 
 
 
     std::time_t get_start_time() const;

      
 
 
 
 
 
 
 
 
 
     bool update_checkpoints(const bool skip_dns = false);

      
 
 
 
 
 
     void graceful_exit();

      
 
 
 
 
     void stop();

      
 
 
 
 
     bool is_key_image_spent(const crypto::key_image& key_im) const;

      
 
 
 
 
 
 
 
 
 
     bool are_key_images_spent(const std::vector<crypto::key_image>& key_im, std::vector<bool> &spent) const;

      
 
 
 
 
 
 
 
     bool are_key_images_spent_in_pool(const std::vector<crypto::key_image>& key_im, std::vector<bool> &spent) const;

      
 
 
 
 
     size_t get_block_sync_size(uint64_t height) const;

      
 
 
 
 
     std::pair<boost::multiprecision::uint128_t, boost::multiprecision::uint128_t> get_coinbase_tx_sum(const uint64_t start_offset, const size_t count);
     
      
 
 
 
      
     network_type get_nettype() const { return m_nettype; };

      
 
 
 
 
 
 
 
     bool is_update_available() const { return m_update_available; }

      
 
 
 
 
     bool fluffy_blocks_enabled() const { return m_fluffy_blocks_enabled; }

      
 
 
 
 
     uint64_t prevalidate_block_hashes(uint64_t height, const std::vector<crypto::hash> &hashes, const std::vector<uint64_t> &weights);

      
 
 
 
 
     uint64_t get_free_space() const;

      
 
 
 
 
     bool offline() const { return m_offline; }

      
 
 
 
 
     uint32_t get_blockchain_pruning_seed() const;

      
 
 
 
 
 
 
     bool prune_blockchain(uint32_t pruning_seed = 0);

      
 
 
 
 
     bool update_blockchain_pruning();

      
 
 
 
 
     bool check_blockchain_pruning();

      
 
 
 
 
     bool is_within_compiled_block_hash_area(uint64_t height) const;

      
 
 
     bool has_block_weights(uint64_t height, uint64_t nblocks) const;

      
 
 
     void flush_bad_txs_cache();

      
 
 
     void flush_invalid_blocks();

      
 
 
 
 
 
 
     bool get_txpool_complement(const std::vector<crypto::hash> &hashes, std::vector<cryptonote::blobdata> &txes);

   private:

      
 
 
 
 
 
 
 
 
 
     bool add_new_tx(transaction& tx, const crypto::hash& tx_hash, const cryptonote::blobdata &blob, size_t tx_weight, tx_verification_context& tvc, relay_method tx_relay, bool relayed);

      
 
 
 
 
 
 
 
 
 
 
 
 
 
     bool add_new_tx(transaction& tx, tx_verification_context& tvc, relay_method tx_relay, bool relayed);

      
 
 
 
 
     bool add_new_block(const block& b, block_verification_context& bvc);

      
 
 
 
 
 
 
     bool load_state_data();

      
 
 
 
 
     bool parse_tx_from_blob(transaction& tx, crypto::hash& tx_hash, const blobdata& blob) const;

      
 
 
 
 
 
 
 
 
 
     bool check_tx_syntax(const transaction& tx) const;

      
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
     bool check_tx_semantic(const transaction& tx, bool keeped_by_block) const;
     void set_semantics_failed(const crypto::hash &tx_hash);

     bool handle_incoming_tx_pre(const tx_blob_entry& tx_blob, tx_verification_context& tvc, cryptonote::transaction &tx, crypto::hash &tx_hash);
     bool handle_incoming_tx_post(const tx_blob_entry& tx_blob, tx_verification_context& tvc, cryptonote::transaction &tx, crypto::hash &tx_hash);
     struct tx_verification_batch_info { const cryptonote::transaction *tx; crypto::hash tx_hash; tx_verification_context &tvc; bool &result; };
     bool handle_incoming_tx_accumulated_batch(std::vector<tx_verification_batch_info> &tx_info, bool keeped_by_block);

      
 
 
 
 
 
 
     bool update_miner_block_template();

      
 
 
 
 
 
 
     bool handle_command_line(const boost::program_options::variables_map& vm);

      
 
 
 
 
 
 
     bool check_tx_inputs_keyimages_diff(const transaction& tx) const;

      
 
 
 
 
 
 
 
     bool check_tx_inputs_ring_members_diff(const transaction& tx, const uint8_t hf_version) const;

      
 
 
 
 
 
 
 
     bool check_tx_inputs_keyimages_domain(const transaction& tx) const;

      
 
 
 
 
     bool relay_txpool_transactions();

      
 
 
 
 
     bool notify_txpool_event(const epee::span<const cryptonote::blobdata> tx_blobs, epee::span<const crypto::hash> tx_hashes, epee::span<const cryptonote::transaction> txs, const std::vector<bool> &just_broadcasted) const;

      
 
 
 
 
     bool check_updates();

      
 
 
 
 
     bool check_disk_space();

      
 
 
 
 
     bool check_block_rate();

      
 
 
 
 
     bool recalculate_difficulties();

     bool m_test_drop_download = true;  

     uint64_t m_test_drop_download_height = 0;  

     tx_memory_pool m_mempool;  
     Blockchain m_blockchain_storage;  

     i_cryptonote_protocol* m_pprotocol;  

     epee::critical_section m_incoming_tx_lock;  

      
     miner m_miner;  

     std::string m_config_folder;  

     cryptonote_protocol_stub m_protocol_stub;  

     epee::math_helper::once_a_time_seconds<60*60*12, false> m_store_blockchain_interval;  
     epee::math_helper::once_a_time_seconds<60*60*2, true> m_fork_moaner;  
     epee::math_helper::once_a_time_seconds<60*60*12, true> m_check_updates_interval;  
     epee::math_helper::once_a_time_seconds<60*10, true> m_check_disk_space_interval;  
     epee::math_helper::once_a_time_seconds<90, false> m_block_rate_interval;  
     epee::math_helper::once_a_time_seconds<60*60*5, true> m_blockchain_pruning_interval;  
     epee::math_helper::once_a_time_seconds<60*60*24*7, false> m_diff_recalc_interval;  

     std::atomic<bool> m_starter_message_showed;  

     uint64_t m_target_blockchain_height;  

     network_type m_nettype;  

     std::atomic<bool> m_update_available;

     std::string m_checkpoints_path;  
     time_t m_last_dns_checkpoints_update;  
     time_t m_last_json_checkpoints_update;  

     std::atomic_flag m_checkpoints_updating;  
     bool m_disable_dns_checkpoints;

     size_t block_sync_size;

     time_t start_time;

     std::unordered_set<crypto::hash> bad_semantics_txes[2];
     boost::mutex bad_semantics_txes_lock;

     enum {
       UPDATES_DISABLED,
       UPDATES_NOTIFY,
       UPDATES_DOWNLOAD,
       UPDATES_UPDATE,
     } check_updates_level;

     tools::download_async_handle m_update_download;
     size_t m_last_update_length;
     boost::mutex m_update_mutex;

     bool m_fluffy_blocks_enabled;
     bool m_offline;

     
 
 

     std::shared_ptr<tools::Notify> m_block_rate_notify;
     boost::function<void(std::vector<txpool_event>)> m_zmq_pub;
   };
}

POP_WARNINGS
