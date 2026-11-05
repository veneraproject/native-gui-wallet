                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#pragma once
#include "include_base_utils.h"

#include <atomic>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <boost/serialization/version.hpp>
#include <boost/utility.hpp>

#include "span.h"
#include "string_tools.h"
#include "syncobj.h"
#include "math_helper.h"
#include "cryptonote_basic/cryptonote_basic_impl.h"
#include "cryptonote_basic/verification_context.h"
#include "cryptonote_protocol/enums.h"
#include "blockchain_db/blockchain_db.h"
#include "crypto/hash.h"
#include "rpc/core_rpc_server_commands_defs.h"
#include "rpc/message_data_structs.h"

namespace cryptonote
{
  class Blockchain;
   
   
   

   
  typedef std::pair<std::pair<double, std::time_t>, crypto::hash> tx_by_fee_and_receive_time_entry;

  class txCompare
  {
  public:
    bool operator()(const tx_by_fee_and_receive_time_entry& a, const tx_by_fee_and_receive_time_entry& b) const
    {
       
      if (a.first.first > b.first.first) return true;
      if (a.first.first < b.first.first) return false;

      if (a.first.second < b.first.second) return true;
      if (a.first.second > b.first.second) return false;

      return memcmp(a.second.data, b.second.data, sizeof(crypto::hash)) < 0;
    }
  };

   
  typedef std::set<tx_by_fee_and_receive_time_entry, txCompare> sorted_tx_container;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
  class tx_memory_pool: boost::noncopyable
  {
  public:
     
 
 
 
 
    tx_memory_pool(Blockchain& bchs);


     
 
 
 
 
 
 
    bool add_tx(transaction &tx, const crypto::hash &id, const cryptonote::blobdata &blob, size_t tx_weight, tx_verification_context& tvc, relay_method tx_relay, bool relayed, uint8_t version);

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool add_tx(transaction &tx, tx_verification_context& tvc, relay_method tx_relay, bool relayed, uint8_t version);

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool take_tx(const crypto::hash &id, transaction &tx, cryptonote::blobdata &txblob, size_t& tx_weight, uint64_t& fee, bool &relayed, bool &do_not_relay, bool &double_spend_seen, bool &pruned);

     
 
 
 
 
 
 
 
    bool have_tx(const crypto::hash &id, relay_category tx_category) const;

     
 
 
 
 
 
 
 
 
 
    bool on_blockchain_inc(uint64_t new_block_height, const crypto::hash& top_block_id);

     
 
 
 
 
 
 
 
 
 
    bool on_blockchain_dec(uint64_t new_block_height, const crypto::hash& top_block_id);

     
 
 
 
 
    void on_idle();

     
 
 
    void lock() const;

     
 
 
    void unlock() const;

     

     
 
 
 
 
 
 
 
    bool init(size_t max_txpool_weight = 0, bool mine_stem_txes = false);

     
 
 
 
 
 
 
 
 
    bool deinit();

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool fill_block_template(block &bl, size_t median_weight, uint64_t already_generated_coins, size_t &total_weight, uint64_t &fee, uint64_t &expected_reward, uint8_t version, bool include_sensitive = true);

     
 
 
 
 
 
 
    void get_transactions(std::vector<transaction>& txs, bool include_sensitive = false) const;

     
 
 
 
 
 
 
    void get_transaction_hashes(std::vector<crypto::hash>& txs, bool include_sensitive = false) const;

     
 
 
 
 
 
 
    void get_transaction_backlog(std::vector<tx_backlog_entry>& backlog, bool include_sensitive = false) const;

     
 
 
 
 
 
 
 
 
 
 
    void get_block_template_backlog(std::vector<tx_block_template_backlog_entry>& backlog, bool include_sensitive = false) const;

     
 
 
 
 
 
 
    void get_transaction_stats(struct txpool_stats& stats, bool include_sensitive = false) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool get_transactions_and_spent_keys_info(std::vector<tx_info>& tx_infos, std::vector<spent_key_image_info>& key_image_infos, bool include_sensitive_data = false) const;

     
 
 
 
 
 
 
 
 
 
 
    bool get_pool_for_rpc(std::vector<cryptonote::rpc::tx_in_pool>& tx_infos, cryptonote::rpc::key_images_with_tx_hashes& key_image_infos, bool include_sensitive) const;

     
 
 
 
 
 
 
 
    bool check_for_key_images(const std::vector<crypto::key_image>& key_images, std::vector<bool>& spent) const;

     
 
 
 
 
 
 
 
 
    bool get_transaction(const crypto::hash& h, cryptonote::blobdata& txblob, relay_category tx_category) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
    bool get_relayable_transactions(std::vector<std::tuple<crypto::hash, cryptonote::blobdata, relay_method>>& txs);

     
 
 
 
 
 
 
 
    void set_relayed(epee::span<const crypto::hash> hashes, relay_method tx_relay, std::vector<bool> &just_broadcasted);

     
 
 
 
 
    size_t get_transactions_count(bool include_sensitive = false) const;

     
 
 
 
 
 
 
    std::string print_pool(bool short_format) const;

     
 
 
 
 
 
 
 
 
 
 
    size_t validate(uint8_t version);

      
 
 
 
 
    uint64_t cookie() const { return m_cookie; }

     
 
 
 
 
    size_t get_txpool_weight() const;

     
 
 
 
 
    void set_txpool_max_weight(size_t bytes);

     
 
 
 
 
    void reduce_txpool_weight(size_t weight);

#define CURRENT_MEMPOOL_ARCHIVE_VER    11
#define CURRENT_MEMPOOL_TX_DETAILS_ARCHIVE_VER    13

     
 
 
    struct tx_details
    {
      transaction tx;   
      cryptonote::blobdata tx_blob;  
      size_t blob_size;   
      size_t weight;   
      uint64_t fee;   
      crypto::hash max_used_block_id;   
      uint64_t max_used_block_height;   

       
       
 
 
      bool kept_by_block;  

       
       
 
 
 
      uint64_t last_failed_height;  

       
       
 
 
 
      crypto::hash last_failed_id;

      time_t receive_time;   

      time_t last_relayed_time;   
      bool relayed;   
      bool do_not_relay;  

      bool double_spend_seen;  
    };

     
 
 
    bool get_transaction_info(const crypto::hash &txid, tx_details &td, bool include_sensitive_data, bool include_blob = false) const;

     
 
 
    bool get_transactions_info(const std::vector<crypto::hash>& txids, std::vector<std::pair<crypto::hash, tx_details>>& txs, bool include_sensitive_data = false) const;

     
 
 
    bool get_complement(std::vector<crypto::hash> hashes, std::vector<cryptonote::blobdata> &txes) const;

     
 
 
 
 
    bool get_pool_info(time_t start_time, bool include_sensitive, size_t max_tx_count, std::vector<std::pair<crypto::hash, tx_details>>& added_txs, std::vector<crypto::hash>& remaining_added_txids, std::vector<crypto::hash>& removed_txs, bool& incremental) const;

  private:

     
 
 
 
 
    bool insert_key_images(const transaction_prefix &tx, const crypto::hash &txid, relay_method tx_relay);

     
 
 
 
 
 
 
 
 
    bool remove_stuck_transactions();

     
 
 
 
 
 
 
 
    bool have_tx_keyimg_as_spent(const crypto::key_image& key_im, const crypto::hash& txid) const;

     
 
 
 
 
 
 
 
 
 
 
 
 
    bool have_tx_keyimges_as_spent(const transaction& tx, const crypto::hash& txid) const;

     
 
 
 
 
 
 
 
 
 
 
 
    bool remove_transaction_keyimages(const transaction_prefix& tx, const crypto::hash &txid);

     
 
 
 
 
 
 
 
    static bool have_key_images(const std::unordered_set<crypto::key_image>& kic, const transaction_prefix& tx);

     
 
 
 
 
 
 
 
    static bool append_key_images(std::unordered_set<crypto::key_image>& kic, const transaction_prefix& tx);

     
 
 
 
 
 
 
 
 
 
    bool is_transaction_ready_to_go(txpool_tx_meta_t& txd, const crypto::hash &txid, const cryptonote::blobdata_ref &txblob, transaction&tx) const;
    bool is_transaction_ready_to_go(txpool_tx_meta_t& txd, const crypto::hash &txid, const cryptonote::blobdata &txblob, transaction&tx) const;

     
 
 
    void mark_double_spend(const transaction &tx);

     
 
 
 
 
    void prune(size_t bytes = 0);

    void add_tx_to_transient_lists(const crypto::hash& txid, double fee, time_t receive_time);
    void remove_tx_from_transient_lists(const cryptonote::sorted_tx_container::iterator& sorted_it, const crypto::hash& txid, bool sensitive);
    void track_removed_tx(const crypto::hash& txid, bool sensitive);

     
     
     
     
 
 
 
 
 
    typedef std::unordered_map<crypto::key_image, std::unordered_set<crypto::hash>> key_images_container;

#if defined(DEBUG_CREATE_BLOCK_TEMPLATE)
public:
#endif
    mutable epee::critical_section m_transactions_lock;   
#if defined(DEBUG_CREATE_BLOCK_TEMPLATE)
private:
#endif

     
    key_images_container m_spent_key_images;  

     
     
    epee::math_helper::once_a_time_seconds<30> m_remove_stuck_tx_interval;

     
     
    sorted_tx_container m_txs_by_fee_and_receive_time;

    std::atomic<uint64_t> m_cookie;  

     
    std::unordered_map<crypto::hash, time_t> m_added_txs_by_id;

     
    time_t m_added_txs_start_time;

    struct removed_tx_info
    {
      crypto::hash txid;
      bool sensitive;
    };

     
     
    std::multimap<time_t, removed_tx_info> m_removed_txs_by_time;

     
     
    time_t m_removed_txs_start_time;

     
 
 
 
 
 
 
    sorted_tx_container::iterator find_tx_in_sorted_container(const crypto::hash& id) const;

     
    bool check_tx_inputs(const std::function<cryptonote::transaction&(void)> &get_tx, const crypto::hash &txid, uint64_t &max_used_block_height, crypto::hash &max_used_block_id, tx_verification_context &tvc, bool kept_by_block = false) const;

     
     
 
 
    std::unordered_set<crypto::hash> m_timed_out_transactions;

    Blockchain& m_blockchain;   

    size_t m_txpool_max_weight;
    size_t m_txpool_weight;
    bool m_mine_stem_txes;

    mutable std::unordered_map<crypto::hash, std::tuple<bool, tx_verification_context, uint64_t, crypto::hash>> m_input_cache;

    std::unordered_map<crypto::hash, transaction> m_parsed_tx_cache;

     
    std::atomic<time_t> m_next_check;
  };
}

namespace boost
{
  namespace serialization
  {
    template<class archive_t>
    void serialize(archive_t & ar, cryptonote::tx_memory_pool::tx_details& td, const unsigned int version)
    {
      ar & td.blob_size;
      ar & td.fee;
      ar & td.tx;
      ar & td.max_used_block_height;
      ar & td.max_used_block_id;
      ar & td.last_failed_height;
      ar & td.last_failed_id;
      ar & td.receive_time;
      ar & td.last_relayed_time;
      ar & td.relayed;
      if (version < 11)
        return;
      ar & td.kept_by_block;
      if (version < 12)
        return;
      ar & td.do_not_relay;
      if (version < 13)
        return;
      ar & td.weight;
    }
  }
}
BOOST_CLASS_VERSION(cryptonote::tx_memory_pool, CURRENT_MEMPOOL_ARCHIVE_VER)
BOOST_CLASS_VERSION(cryptonote::tx_memory_pool::tx_details, CURRENT_MEMPOOL_TX_DETAILS_ARCHIVE_VER)



