                                       
                                           
  
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

 

#ifndef RCTSIGS_H
#define RCTSIGS_H

#include <cstddef>
#include <vector>
#include <tuple>

#include "crypto/generic-ops.h"

extern "C" {
#include "crypto/random.h"
#include "crypto/keccak.h"
}
#include "crypto/crypto.h"


#include "rctTypes.h"
#include "rctOps.h"

 
#ifdef DBG
#define DP(x) dp(x)
#else
#define DP(x)
#endif

namespace hw {
    class device;
}


namespace rct {

    boroSig genBorromean(const key64 x, const key64 P1, const key64 P2, const bits indices);
    bool verifyBorromean(const boroSig &bb, const key64 P1, const key64 P2);

     
     
     
     
     
     
    mgSig MLSAG_Gen(const key &message, const keyM & pk, const keyV & xx, const unsigned int index, size_t dsRows, hw::device &hwdev);
    bool MLSAG_Ver(const key &message, const keyM &pk, const mgSig &sig, size_t dsRows);

    clsag CLSAG_Gen(const key &message, const keyV & P, const key & p, const keyV & C, const key & z, const keyV & C_nonzero, const key & C_offset, const unsigned int l, hw::device &hwdev);
    clsag CLSAG_Gen(const key &message, const keyV & P, const key & p, const keyV & C, const key & z, const keyV & C_nonzero, const key & C_offset, const unsigned int l);
    clsag proveRctCLSAGSimple(const key &, const ctkeyV &, const ctkey &, const key &, const key &, unsigned int, hw::device &);
    bool verRctCLSAGSimple(const key &, const clsag &, const ctkeyV &, const key &);

     
     
     
     
     
     
     
    rangeSig proveRange(key & C, key & mask, const xmr_amount & amount);
    bool verRange(const key & C, const rangeSig & as);

     
     
     
     
     
     
     
     
    mgSig proveRctMG(const ctkeyM & pubs, const ctkeyV & inSk, const keyV &outMasks, const ctkeyV & outPk, unsigned int index, const key &txnFee, const key &message, hw::device &hwdev);
    mgSig proveRctMGSimple(const key & message, const ctkeyV & pubs, const ctkey & inSk, const key &a , const key &Cout, unsigned int index, hw::device &hwdev);
    bool verRctMG(const mgSig &mg, const ctkeyM & pubs, const ctkeyV & outPk, const key &txnFee, const key &message);
    bool verRctMGSimple(const key &message, const mgSig &mg, const ctkeyV & pubs, const key & C);

     
     
     
     
     
    void getKeyFromBlockchain(ctkey & a, size_t reference_index);
    std::tuple<ctkeyM, xmr_amount> populateFromBlockchain(ctkeyV inPk, int mixin);

     
     
     
     
     
     
     
     
     
     
    rctSig genRct(const key &message, const ctkeyV & inSk, const keyV & destinations, const std::vector<xmr_amount> & amounts, const ctkeyM &mixRing, const keyV &amount_keys, unsigned int index, ctkeyV &outSk, const RCTConfig &rct_config, hw::device &hwdev);
    rctSig genRct(const key &message, const ctkeyV & inSk, const ctkeyV  & inPk, const keyV & destinations, const std::vector<xmr_amount> & amounts, const keyV &amount_keys, const int mixin, const RCTConfig &rct_config, hw::device &hwdev);
    rctSig genRctSimple(const key & message, const ctkeyV & inSk, const ctkeyV & inPk, const keyV & destinations, const std::vector<xmr_amount> & inamounts, const std::vector<xmr_amount> & outamounts, const keyV &amount_keys, xmr_amount txnFee, unsigned int mixin, const RCTConfig &rct_config, hw::device &hwdev);
    rctSig genRctSimple(const key & message, const ctkeyV & inSk, const keyV & destinations, const std::vector<xmr_amount> & inamounts, const std::vector<xmr_amount> & outamounts, xmr_amount txnFee, const ctkeyM & mixRing, const keyV &amount_keys, const std::vector<unsigned int> & index, ctkeyV &outSk, const RCTConfig &rct_config, hw::device &hwdev);
    bool verRct(const rctSig & rv, bool semantics);
    static inline bool verRct(const rctSig & rv) { return verRct(rv, true) && verRct(rv, false); }
    bool verRctSemanticsSimple(const rctSig & rv);
    bool verRctSemanticsSimple(const std::vector<const rctSig*> & rv);
    bool verRctNonSemanticsSimple(const rctSig & rv);
    static inline bool verRctSimple(const rctSig & rv) { return verRctSemanticsSimple(rv) && verRctNonSemanticsSimple(rv); }
    xmr_amount decodeRct(const rctSig & rv, const key & sk, unsigned int i, key & mask, hw::device &hwdev);
    xmr_amount decodeRct(const rctSig & rv, const key & sk, unsigned int i, hw::device &hwdev);
    xmr_amount decodeRctSimple(const rctSig & rv, const key & sk, unsigned int i, key & mask, hw::device &hwdev);
    xmr_amount decodeRctSimple(const rctSig & rv, const key & sk, unsigned int i, hw::device &hwdev);
    key get_pre_mlsag_hash(const rctSig &rv, hw::device &hwdev);
}
#endif   

