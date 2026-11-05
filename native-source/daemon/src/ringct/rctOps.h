                                       
             
                                           
  
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#ifndef RCTOPS_H
#define RCTOPS_H

#include <cstddef>
#include <tuple>

#include "crypto/generic-ops.h"

extern "C" {
#include "crypto/random.h"
#include "crypto/keccak.h"
#include "rctCryptoOps.h"
}
#include "crypto/crypto.h"

#include "rctTypes.h"

 
#ifdef DBG
#define DP(x) dp(x)
#else
#define DP(x)
#endif

namespace rct {

     

    static const key Z = { {0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00  } };
    static const key I = { {0x01, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00  } };
    static const key L = { {0xed, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58, 0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10 } };
    static const key G = { {0x58, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66 } };
    static const key EIGHT = { {0x08, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00 , 0x00, 0x00, 0x00,0x00  } };
    static const key INV_EIGHT = { { 0x79, 0x2f, 0xdc, 0xe2, 0x29, 0xe5, 0x06, 0x61, 0xd0, 0xda, 0x1c, 0x7d, 0xb3, 0x9d, 0xd3, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06 } };

     
    inline key zero() { return Z; }
    inline void zero(key &z) { memset(&z, 0, 32); }
     
    inline key identity() { return I; }
    inline void identity(key &Id) { memcpy(&Id, &I, 32); }
     
    inline key curveOrder() { return L; }
    inline void curveOrder(key &l) { l = L; }
     
    inline void copy(key &AA, const key &A) { memcpy(&AA, &A, 32); }
    inline key copy(const key & A) { key AA; memcpy(&AA, &A, 32); return AA; }

     
     
     
    keyM keyMInit(size_t rows, size_t cols);

     
    bool toPointCheckOrder(ge_p3 *P, const unsigned char *data);

     
    key skGen();
    void skGen(key &);
    
     
    keyV skvGen(size_t rows );
    
     
    key pkGen();
     
    void skpkGen(key &sk, key &pk);
    std::tuple<key, key> skpkGen();
     
    std::tuple<ctkey, ctkey> ctskpkGen(xmr_amount amount);
     
    void genC(key & C, const key & a, xmr_amount amount);
     
    std::tuple<ctkey, ctkey> ctskpkGen(const key &bH);
     
    key commit(xmr_amount amount, const key &mask);
     
    key zeroCommit(xmr_amount amount);
     
    xmr_amount randXmrAmount(xmr_amount upperlimit);

     

     
    void scalarmultBase(key & aG, const key &a);
    key scalarmultBase(const key & a);
     
    void scalarmultKey(key &aP, const key &P, const key &a);
    key scalarmultKey(const key &P, const key &a);
     
    key scalarmultH(const key & a);
     
    key scalarmult8(const key & P);
    void scalarmult8(ge_p3 &res, const key & P);
     
    bool isInMainSubgroup(const key & a);

     

     
    void addKeys(key &AB, const key &A, const key &B);
    rct::key addKeys(const key &A, const key &B);
    rct::key addKeys(const keyV &A);
     
    void addKeys1(key &aGB, const key &a, const key & B);
     
    void addKeys2(key &aGbB, const key &a, const key &b, const key &B);
     
     
    void precomp(ge_dsmp rv, const key &B);
     
     
    void addKeys3(key &aAbB, const key &a, const key &A, const key &b, const ge_dsmp B);
    void addKeys3(key &aAbB, const key &a, const ge_dsmp A, const key &b, const ge_dsmp B);

    void addKeys_aGbBcC(key &aGbBcC, const key &a, const key &b, const ge_dsmp B, const key &c, const ge_dsmp C);
    void addKeys_aAbBcC(key &aAbBcC, const key &a, const ge_dsmp A, const key &b, const ge_dsmp B, const key &c, const ge_dsmp C);

     
    void subKeys(key &AB, const key &A, const  key &B);
     
    bool equalKeys(const key & A, const key & B);

     
     
     
    void cn_fast_hash(key &hash, const void * data, const size_t l);
    void hash_to_scalar(key &hash, const void * data, const size_t l);
     
    void cn_fast_hash(key &hash, const key &in);
    void hash_to_scalar(key &hash, const key &in);
     
    key cn_fast_hash(const key &in);
    key hash_to_scalar(const key &in);
     
    key cn_fast_hash128(const void * in);
    key hash_to_scalar128(const void * in);
    key cn_fast_hash(const ctkeyV &PC);
    key hash_to_scalar(const ctkeyV &PC);
     
    key cn_fast_hash(const keyV &keys);
    key hash_to_scalar(const keyV &keys);
     
    key cn_fast_hash(const key64 keys);
    key hash_to_scalar(const key64 keys);

    void hash_to_p3(ge_p3 &hash8_p3, const key &k);

     
    void sumKeys(key & Csum, const key &Cis);

     
     
    key genCommitmentMask(const key &sk);
    void ecdhEncode(ecdhTuple & unmasked, const key & sharedSec, bool v2);
    void ecdhDecode(ecdhTuple & masked, const key & sharedSec, bool v2);
}
#endif   
