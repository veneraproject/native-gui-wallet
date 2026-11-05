                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#ifndef BLOCKCHAIN_DB_H
#define BLOCKCHAIN_DB_H

#pragma once

#include <string>
#include <exception>
#include <boost/program_options.hpp>
#include "common/command_line.h"
#include "crypto/hash.h"
#include "cryptonote_basic/blobdatatype.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_basic/difficulty.h"
#include "cryptonote_basic/hardfork.h"
#include "cryptonote_protocol/enums.h"

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

namespace cryptonote
{

 
typedef std::pair<crypto::hash, uint64_t> tx_out_index;

extern const command_line::arg_descriptor<std::string> arg_db_sync_mode;
extern const command_line::arg_descriptor<bool, false> arg_db_salvage;

enum class relay_category : uint8_t
{
  broadcasted = 0, 
  relayable,       
  legacy,          
  all              
};

bool matches_category(relay_method method, relay_category category) noexcept;

#pragma pack(push, 1)

 
 
 
struct output_data_t
{
  crypto::public_key pubkey;        
  uint64_t           unlock_time;   
  uint64_t           height;        
  rct::key           commitment;    
};
#pragma pack(pop)

#pragma pack(push, 1)
struct tx_data_t
{
  uint64_t tx_id;
  uint64_t unlock_time;
  uint64_t block_id;
};
#pragma pack(pop)

struct alt_block_data_t
{
  uint64_t height;
  uint64_t cumulative_weight;
  uint64_t cumulative_difficulty_low;
  uint64_t cumulative_difficulty_high;
  uint64_t already_generated_coins;
};

 
 
 
struct txpool_tx_meta_t
{
  crypto::hash max_used_block_id;
  crypto::hash last_failed_id;
  uint64_t weight;
  uint64_t fee;
  uint64_t max_used_block_height;
  uint64_t last_failed_height;
  uint64_t receive_time;
  uint64_t last_relayed_time;  
   
  uint8_t kept_by_block;
  uint8_t relayed;
  uint8_t do_not_relay;
  uint8_t double_spend_seen: 1;
  uint8_t pruned: 1;
  uint8_t is_local: 1;
  uint8_t dandelionpp_stem : 1;
  uint8_t is_forwarding: 1;
  uint8_t bf_padding: 3;

  uint8_t padding[76];  

  void set_relay_method(relay_method method) noexcept;
  relay_method get_relay_method() const noexcept;

   
  bool upgrade_relay_method(relay_method method) noexcept;

   
  bool matches(const relay_category category) const noexcept
  {
    return matches_category(get_relay_method(), category);
  }
};


#define DBF_SAFE       1
#define DBF_FAST       2
#define DBF_FASTEST    4
#define DBF_RDONLY     8
#define DBF_SALVAGE 0x10

 
 
 

 
 
 
class DB_EXCEPTION : public std::exception
{
  private:
    std::string m;

  protected:
    DB_EXCEPTION(const char *s) : m(s) { }

  public:
    virtual ~DB_EXCEPTION() { }

    const char* what() const throw()
    {
      return m.c_str();
    }
};

 
 
 
class DB_ERROR : public DB_EXCEPTION
{
  public:
    DB_ERROR() : DB_EXCEPTION("Generic DB Error") { }
    DB_ERROR(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class DB_ERROR_TXN_START : public DB_EXCEPTION
{
  public:
    DB_ERROR_TXN_START() : DB_EXCEPTION("DB Error in starting txn") { }
    DB_ERROR_TXN_START(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class DB_OPEN_FAILURE : public DB_EXCEPTION
{
  public:
    DB_OPEN_FAILURE() : DB_EXCEPTION("Failed to open the db") { }
    DB_OPEN_FAILURE(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class DB_CREATE_FAILURE : public DB_EXCEPTION
{
  public:
    DB_CREATE_FAILURE() : DB_EXCEPTION("Failed to create the db") { }
    DB_CREATE_FAILURE(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class DB_SYNC_FAILURE : public DB_EXCEPTION
{
  public:
    DB_SYNC_FAILURE() : DB_EXCEPTION("Failed to sync the db") { }
    DB_SYNC_FAILURE(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class BLOCK_DNE : public DB_EXCEPTION
{
  public:
    BLOCK_DNE() : DB_EXCEPTION("The block requested does not exist") { }
    BLOCK_DNE(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class BLOCK_PARENT_DNE : public DB_EXCEPTION
{
  public:
    BLOCK_PARENT_DNE() : DB_EXCEPTION("The parent of the block does not exist") { }
    BLOCK_PARENT_DNE(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class BLOCK_EXISTS : public DB_EXCEPTION
{
  public:
    BLOCK_EXISTS() : DB_EXCEPTION("The block to be added already exists!") { }
    BLOCK_EXISTS(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class BLOCK_INVALID : public DB_EXCEPTION
{
  public:
    BLOCK_INVALID() : DB_EXCEPTION("The block to be added did not pass validation!") { }
    BLOCK_INVALID(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class TX_DNE : public DB_EXCEPTION
{
  public:
    TX_DNE() : DB_EXCEPTION("The transaction requested does not exist") { }
    TX_DNE(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class TX_EXISTS : public DB_EXCEPTION
{
  public:
    TX_EXISTS() : DB_EXCEPTION("The transaction to be added already exists!") { }
    TX_EXISTS(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class OUTPUT_DNE : public DB_EXCEPTION
{
  public:
    OUTPUT_DNE() : DB_EXCEPTION("The output requested does not exist!") { }
    OUTPUT_DNE(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class OUTPUT_EXISTS : public DB_EXCEPTION
{
  public:
    OUTPUT_EXISTS() : DB_EXCEPTION("The output to be added already exists!") { }
    OUTPUT_EXISTS(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 
class KEY_IMAGE_EXISTS : public DB_EXCEPTION
{
  public:
    KEY_IMAGE_EXISTS() : DB_EXCEPTION("The spent key image to be added already exists!") { }
    KEY_IMAGE_EXISTS(const char* s) : DB_EXCEPTION(s) { }
};

 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
class BlockchainDB
{
private:
   
 
 

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void add_block( const block& blk
                , size_t block_weight
                , uint64_t long_term_block_weight
                , const difficulty_type& cumulative_difficulty
                , const uint64_t& coins_generated
                , uint64_t num_rct_outs
                , const crypto::hash& blk_hash
                ) = 0;

   
 
 
 
 
 
 
 
 
 
  virtual void remove_block() = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t add_transaction_data(const crypto::hash& blk_hash, const std::pair<transaction, blobdata_ref>& tx, const crypto::hash& tx_hash, const crypto::hash& tx_prunable_hash) = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void remove_transaction_data(const crypto::hash& tx_hash, const transaction& tx) = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t add_output(const crypto::hash& tx_hash, const tx_out& tx_output, const uint64_t& local_index, const uint64_t unlock_time, const rct::key *commitment) = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void add_tx_amount_output_indices(const uint64_t tx_id, const std::vector<uint64_t>& amount_output_indices) = 0;

   
 
 
 
 
 
 
 
 
 
  virtual void add_spent_key(const crypto::key_image& k_image) = 0;

   
 
 
 
 
 
 
 
 
 
  virtual void remove_spent_key(const crypto::key_image& k_image) = 0;


   
 
 
   
 
 
 
 
 
  void pop_block();

   
   
 
 
 
 
 
 
  void remove_transaction(const crypto::hash& tx_hash);

  uint64_t num_calls = 0;   
  uint64_t time_blk_hash = 0;   
  uint64_t time_add_block1 = 0;   
  uint64_t time_add_transaction = 0;   


protected:

   
 
 
 
 
 
 
 
 
 
 
  void add_transaction(const crypto::hash& blk_hash, const std::pair<transaction, blobdata_ref>& tx, const crypto::hash* tx_hash_ptr = NULL, const crypto::hash* tx_prunable_hash_ptr = NULL);

  mutable uint64_t time_tx_exists = 0;   
  uint64_t time_commit1 = 0;   
  bool m_auto_remove_logs = true;   

  HardFork* m_hardfork;

public:

   
 
 
  BlockchainDB(): m_hardfork(NULL), m_open(false) { }

   
 
 
  virtual ~BlockchainDB() { };

   
 
 
  static void init_options(boost::program_options::options_description& desc);

   
 
 
  void reset_stats();

   
 
 
 
 
 
  void show_stats();

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void open(const std::string& filename, const int db_flags = 0) = 0;

   
 
 
 
 
  bool is_open() const;

   
 
 
 
 
 
 
 
 
 
  virtual void close() = 0;

   
 
 
 
 
 
 
 
 
 
 
  virtual void sync() = 0;

   
 
 
 
 
  virtual void safesyncmode(const bool onoff) = 0;

   
 
 
 
 
 
 
 
 
 
  virtual void reset() = 0;

   
 
 
 
 
 
 
 
 
  virtual std::vector<std::string> get_filenames() const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual bool remove_data_file(const std::string& folder) const = 0;

   
   
 
 
 
 
 
 
 
  virtual std::string get_db_name() const = 0;


   
   
   
   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool lock() = 0;

   
   
 
 
 
 
 
 
 
 
 
  virtual void unlock() = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool batch_start(uint64_t batch_num_blocks=0, uint64_t batch_bytes=0) = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void batch_stop() = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void batch_abort() = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void set_batch_transactions(bool) = 0;

  virtual void block_wtxn_start() = 0;
  virtual void block_wtxn_stop() = 0;
  virtual void block_wtxn_abort() = 0;
  virtual bool block_rtxn_start() const = 0;
  virtual void block_rtxn_stop() const = 0;
  virtual void block_rtxn_abort() const = 0;

  virtual void set_hard_fork(HardFork* hf);

   
   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t add_block( const std::pair<block, blobdata>& blk
                            , size_t block_weight
                            , uint64_t long_term_block_weight
                            , const difficulty_type& cumulative_difficulty
                            , const uint64_t& coins_generated
                            , const std::vector<std::pair<transaction, blobdata>>& txs
                            );

   
 
 
 
 
 
 
 
  virtual bool block_exists(const crypto::hash& h, uint64_t *height = NULL) const = 0;

   
 
 
 
 
 
 
 
 
 
 
  virtual cryptonote::blobdata get_block_blob(const crypto::hash& h) const = 0;

   
 
 
 
 
 
 
 
 
 
 
  virtual block get_block(const crypto::hash& h) const;

   
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t get_block_height(const crypto::hash& h) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual block_header get_block_header(const crypto::hash& h) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual cryptonote::blobdata get_block_blob_from_height(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
 
 
  virtual block get_block_from_height(const uint64_t& height) const;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t get_block_timestamp(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual std::vector<uint64_t> get_block_cumulative_rct_outputs(const std::vector<uint64_t> &heights) const = 0;

   
 
 
 
 
 
 
  virtual uint64_t get_top_block_timestamp() const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual size_t get_block_weight(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
 
  virtual std::vector<uint64_t> get_block_weights(uint64_t start_height, size_t count) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual difficulty_type get_block_cumulative_difficulty(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual difficulty_type get_block_difficulty(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
  virtual void correct_block_cumulative_difficulties(const uint64_t& start_height, const std::vector<difficulty_type>& new_cumulative_difficulties) = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t get_block_already_generated_coins(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
 
  virtual uint64_t get_block_long_term_weight(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
 
  virtual std::vector<uint64_t> get_long_term_block_weights(uint64_t start_height, size_t count) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual crypto::hash get_block_hash_from_height(const uint64_t& height) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual std::vector<block> get_blocks_range(const uint64_t& h1, const uint64_t& h2) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual std::vector<crypto::hash> get_hashes_range(const uint64_t& h1, const uint64_t& h2) const = 0;

   
 
 
 
 
 
 
 
 
  virtual crypto::hash top_block_hash(uint64_t *block_height = NULL) const = 0;

   
 
 
 
 
 
 
  virtual block get_top_block() const = 0;

   
 
 
 
 
 
 
  virtual uint64_t height() const = 0;


   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual void pop_block(block& blk, std::vector<transaction>& txs);


   
 
 
 
 
 
 
 
 
 
 
  virtual bool tx_exists(const crypto::hash& h) const = 0;
  virtual bool tx_exists(const crypto::hash& h, uint64_t& tx_id) const = 0;

   
   
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t get_tx_unlock_time(const crypto::hash& h) const = 0;

   
   
   
 
 
 
 
 
 
 
 
  virtual transaction get_tx(const crypto::hash& h) const;

   
 
 
 
 
 
 
 
 
  virtual transaction get_pruned_tx(const crypto::hash& h) const;

   
 
 
 
 
 
 
 
 
  virtual bool get_tx(const crypto::hash& h, transaction &tx) const;

   
 
 
 
 
 
 
 
 
  virtual bool get_pruned_tx(const crypto::hash& h, transaction &tx) const;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual bool get_tx_blob(const crypto::hash& h, cryptonote::blobdata &tx) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual bool get_pruned_tx_blob(const crypto::hash& h, cryptonote::blobdata &tx) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool get_pruned_tx_blobs_from(const crypto::hash& h, size_t count, std::vector<cryptonote::blobdata> &bd) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool get_blocks_from(uint64_t start_height, size_t min_block_count, size_t max_block_count, size_t max_tx_count, size_t max_size, std::vector<std::pair<std::pair<cryptonote::blobdata, crypto::hash>, std::vector<std::pair<crypto::hash, cryptonote::blobdata>>>>& blocks, bool pruned, bool skip_coinbase, bool get_miner_tx_hash) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool get_prunable_tx_blob(const crypto::hash& h, cryptonote::blobdata &tx) const = 0;

   
 
 
 
 
 
 
 
 
 
 
  virtual bool get_prunable_tx_hash(const crypto::hash& tx_hash, crypto::hash &prunable_hash) const = 0;

   
 
 
 
 
 
 
 
  virtual uint64_t get_tx_count() const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual std::vector<transaction> get_tx_list(const std::vector<crypto::hash>& hlist) const = 0;

   
   
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t get_tx_block_height(const crypto::hash& h) const = 0;

   
   
 
 
 
 
 
 
 
 
 
 
 
 
  virtual uint64_t get_num_outputs(const uint64_t& amount) const = 0;

   
 
 
 
 
  virtual uint64_t get_indexing_base() const { return 0; }

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual output_data_t get_output_key(const uint64_t& amount, const uint64_t& index, bool include_commitmemt = true) const = 0;

   
 
 
 
 
 
 
 
 
 
  virtual tx_out_index get_output_tx_and_index_from_global(const uint64_t& index) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
  virtual tx_out_index get_output_tx_and_index(const uint64_t& amount, const uint64_t& index) const = 0;

   
 
 
 
 
 
 
 
 
 
 
  virtual void get_output_tx_and_index(const uint64_t& amount, const std::vector<uint64_t> &offsets, std::vector<tx_out_index> &indices) const = 0;

   
 
 
 
 
 
 
 
 
 
 
  virtual void get_output_key(const epee::span<const uint64_t> &amounts, const std::vector<uint64_t> &offsets, std::vector<output_data_t> &outputs, bool allow_partial = false) const = 0;
  
   
 
 
 
  virtual bool can_thread_bulk_indices() const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual std::vector<std::vector<uint64_t>> get_tx_amount_output_indices(const uint64_t tx_id, size_t n_txes = 1) const = 0;

   
 
 
 
 
 
 
  virtual bool has_key_image(const crypto::key_image& img) const = 0;

   
 
 
 
 
  virtual void add_txpool_tx(const crypto::hash &txid, const cryptonote::blobdata_ref &blob, const txpool_tx_meta_t& details) = 0;

   
 
 
 
 
 
  virtual void update_txpool_tx(const crypto::hash &txid, const txpool_tx_meta_t& details) = 0;

   
 
 
  virtual uint64_t get_txpool_tx_count(relay_category tx_category = relay_category::broadcasted) const = 0;

   
 
 
  virtual bool txpool_has_tx(const crypto::hash &txid, relay_category tx_category) const = 0;

   
 
 
 
 
  virtual void remove_txpool_tx(const crypto::hash& txid) = 0;

   
 
 
 
 
 
 
 
  virtual bool get_txpool_tx_meta(const crypto::hash& txid, txpool_tx_meta_t &meta) const = 0;

   
 
 
 
 
 
 
 
 
  virtual bool get_txpool_tx_blob(const crypto::hash& txid, cryptonote::blobdata &bd, relay_category tx_category) const = 0;

   
 
 
 
 
 
 
  virtual cryptonote::blobdata get_txpool_tx_blob(const crypto::hash& txid, relay_category tx_category) const = 0;

   
 
 
 
 
 
 
 
  bool txpool_tx_matches_category(const crypto::hash& tx_hash, relay_category category);

   
 
 
 
 
  virtual void prune_outputs(uint64_t amount) = 0;

   
 
 
 
  virtual uint32_t get_blockchain_pruning_seed() const = 0;

   
 
 
 
 
  virtual bool prune_blockchain(uint32_t pruning_seed = 0) = 0;

   
 
 
 
  virtual bool update_pruning() = 0;

   
 
 
 
  virtual bool check_pruning() = 0;

   
 
 
  virtual uint64_t get_max_block_size() = 0;

   
 
 
 
 
 
 
  virtual void add_max_block_size(uint64_t sz) = 0;

   
 
 
 
 
 
 
  virtual void add_alt_block(const crypto::hash &blkid, const cryptonote::alt_block_data_t &data, const cryptonote::blobdata_ref &blob) = 0;

   
 
 
 
 
 
 
 
 
  virtual bool get_alt_block(const crypto::hash &blkid, alt_block_data_t *data, cryptonote::blobdata *blob) = 0;

   
 
 
 
 
  virtual void remove_alt_block(const crypto::hash &blkid) = 0;

   
 
 
  virtual uint64_t get_alt_block_count() = 0;

   
 
 
  virtual void drop_alt_blocks() = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool for_all_txpool_txes(std::function<bool(const crypto::hash&, const txpool_tx_meta_t&, const cryptonote::blobdata_ref*)>, bool include_blob = false, relay_category category = relay_category::broadcasted) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool for_all_key_images(std::function<bool(const crypto::key_image&)>) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool for_blocks_range(const uint64_t& h1, const uint64_t& h2, std::function<bool(uint64_t, const crypto::hash&, const cryptonote::block&)>) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool for_all_transactions(std::function<bool(const crypto::hash&, const cryptonote::transaction&)>, bool pruned) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool for_all_outputs(std::function<bool(uint64_t amount, const crypto::hash &tx_hash, uint64_t height, size_t tx_idx)> f) const = 0;
  virtual bool for_all_outputs(uint64_t amount, const std::function<bool(uint64_t height)> &f) const = 0;

   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  virtual bool for_all_alt_blocks(std::function<bool(const crypto::hash &blkid, const alt_block_data_t &data, const cryptonote::blobdata_ref *blob)> f, bool include_blob = false) const = 0;


   
   
   

   
 
 
 
 
 
  virtual void set_hard_fork_version(uint64_t height, uint8_t version) = 0;

   
 
 
 
 
 
 
  virtual uint8_t get_hard_fork_version(uint64_t height) const = 0;

   
 
 
  virtual void check_hard_fork_info() = 0;

   
 
 
  virtual void drop_hard_fork_info() = 0;

   
 
 
 
 
 
 
 
 
 
  virtual std::map<uint64_t, std::tuple<uint64_t, uint64_t, uint64_t>> get_output_histogram(const std::vector<uint64_t> &amounts, bool unlocked, uint64_t recent_cutoff, uint64_t min_count) const = 0;

  virtual bool get_output_distribution(uint64_t amount, uint64_t from_height, uint64_t to_height, std::vector<uint64_t> &distribution, uint64_t &base) const = 0;

   
 
 
 
 
  virtual bool is_read_only() const = 0;

   
 
 
 
 
  virtual uint64_t get_database_size() const = 0;

   
   
   
 
 

   
 
 
 
 
 
 
 
  void set_auto_remove_logs(bool auto_remove) { m_auto_remove_logs = auto_remove; }

  bool m_open;   
  mutable epee::critical_section m_synchronization_lock;   

};   

class db_txn_guard
{
public:
  db_txn_guard(BlockchainDB *db, bool readonly): db(db), readonly(readonly), active(false)
  {
    if (readonly)
    {
      active = db->block_rtxn_start();
    }
    else
    {
      db->block_wtxn_start();
      active = true;
    }
  }
  virtual ~db_txn_guard()
  {
    stop();
  }
  void stop()
  {
    if (active)
    {
      if (readonly)
        db->block_rtxn_stop();
      else
        db->block_wtxn_stop();
      active = false;
    }
  }
  void abort()
  {
    if (readonly)
      db->block_rtxn_abort();
    else
      db->block_wtxn_abort();
    active = false;
  }

private:
  BlockchainDB *db;
  bool readonly;
  bool active;
};

class db_rtxn_guard: public db_txn_guard { public: db_rtxn_guard(BlockchainDB *db): db_txn_guard(db, true) {} };
class db_wtxn_guard: public db_txn_guard { public: db_wtxn_guard(BlockchainDB *db): db_txn_guard(db, false) {} };

BlockchainDB *new_db();

}   

#endif   
