                                       
                                         
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "cryptonote_core/blockchain.h"
#include "cryptonote_core/tx_verification_utils.h"
#include "ringct/rctSigs.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "blockchain"

#define VER_ASSERT(cond, msgexpr) CHECK_AND_ASSERT_MES(cond, false, msgexpr)

using namespace cryptonote;

 
static bool expand_tx_and_ver_rct_non_sem(transaction& tx, const rct::ctkeyM& mix_ring)
{
     
    VER_ASSERT(!tx.pruned, "Pruned transaction will not pass verRctNonSemanticsSimple");

     
    const crypto::hash tx_prefix_hash = get_transaction_prefix_hash(tx);

     
    const bool exp_res = Blockchain::expand_transaction_2(tx, tx_prefix_hash, mix_ring);
    VER_ASSERT(exp_res, "Failed to expand rct signatures!");

    const rct::rctSig& rv = tx.rct_signatures;

     
    VER_ASSERT(rv.mixRing == mix_ring, "Failed to check ringct signatures: mismatched pubkeys/mixRing");

     
    const size_t n_sigs = rct::is_rct_clsag(rv.type) ? rv.p.CLSAGs.size() : rv.p.MGs.size();
    VER_ASSERT(n_sigs == tx.vin.size(), "Failed to check ringct signatures: mismatched input sigs/vin sizes");

     
    for (size_t n = 0; n < n_sigs; ++n)
    {
        const crypto::key_image& nth_vin_image = boost::get<txin_to_key>(tx.vin[n]).k_image;

        if (rct::is_rct_clsag(rv.type))
        {
            const bool ki_match = 0 == memcmp(&nth_vin_image, &rv.p.CLSAGs[n].I, 32);
            VER_ASSERT(ki_match, "Failed to check ringct signatures: mismatched CLSAG key image");
        }
        else
        {
            const bool mg_nonempty = !rv.p.MGs[n].II.empty();
            VER_ASSERT(mg_nonempty, "Failed to check ringct signatures: missing MLSAG key image");
            const bool ki_match = 0 == memcmp(&nth_vin_image, &rv.p.MGs[n].II[0], 32);
            VER_ASSERT(ki_match, "Failed to check ringct signatures: mismatched MLSAG key image");
        }
    }

     
    return rct::verRctNonSemanticsSimple(rv);
}

 
static crypto::hash calc_tx_mixring_hash(const transaction& tx, const rct::ctkeyM& mix_ring)
{
    std::stringstream ss;

     
    ss << config::HASH_KEY_TXHASH_AND_MIXRING;

     
    const crypto::hash tx_hash = get_transaction_hash(tx);
    ss.write(tx_hash.data, sizeof(crypto::hash));

     
    binary_archive<true> ar(ss);
    ::do_serialize(ar, const_cast<rct::ctkeyM&>(mix_ring));

     
    crypto::hash tx_and_mixring_hash;
    get_blob_hash(ss.str(), tx_and_mixring_hash);

    return tx_and_mixring_hash;
}

 

namespace cryptonote
{

bool ver_rct_non_semantics_simple_cached
(
    transaction& tx,
    const rct::ctkeyM& mix_ring,
    rct_ver_cache_t& cache,
    const std::uint8_t rct_type_to_cache
)
{
     
     
     
     
     
     
     
     
     
     
     
    const bool untested_tx = tx.version > 3 || tx.rct_signatures.type > rct::RCTTypeBulletproofPlus;
    VER_ASSERT(!untested_tx, "Unknown TX type. Make sure RCT cache works correctly with this type and then enable it in the code here.");

     
     
     
    if (tx.rct_signatures.type != rct_type_to_cache)
    {
        MDEBUG("RCT cache: tx " << get_transaction_hash(tx) << " skipped");
        return expand_tx_and_ver_rct_non_sem(tx, mix_ring);
    }

     
    const crypto::hash tx_mixring_hash = calc_tx_mixring_hash(tx, mix_ring);

     
    if (cache.has(tx_mixring_hash))
    {
        MDEBUG("RCT cache: tx " << get_transaction_hash(tx) << " hit");
        return true;
    }

     
    MDEBUG("RCT cache: tx " << get_transaction_hash(tx) << " missed");
    if (!expand_tx_and_ver_rct_non_sem(tx, mix_ring))
    {
        return false;
    }

     
    cache.add(tx_mixring_hash);

    return true;
}

}  
