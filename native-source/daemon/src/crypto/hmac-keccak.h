                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#ifndef HMAC_KECCAK_H
#define HMAC_KECCAK_H

#include "keccak.h"

 
 
 
 
 
 
 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  KECCAK_CTX inner;
  KECCAK_CTX outer;
} hmac_keccak_state;

void hmac_keccak_init(hmac_keccak_state *S, const uint8_t *_key, size_t keylen);
void hmac_keccak_update(hmac_keccak_state *S, const uint8_t *data, size_t datalen);
void hmac_keccak_finish(hmac_keccak_state *S, uint8_t *digest);
void hmac_keccak_hash(uint8_t *out, const uint8_t *key, size_t keylen, const uint8_t *in, size_t inlen);

#ifdef __cplusplus
}
#endif
#endif  
