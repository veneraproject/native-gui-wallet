                                       
                                         
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#ifndef MONERO_CRYPTO_AMD64_51_30K_H
#define MONERO_CRYPTO_AMD64_51_30K_H

#ifdef __cplusplus
extern "C"
{
#endif

  int monero_crypto_amd64_51_30k_ge25519_scalarmult(char* out, char const* pub, char const* sec);
  int monero_crypto_amd64_51_30k_generate_key_derivation(char* out, char const* tx_pub, char const* view_sec);
  int monero_crypto_amd64_51_30k_generate_subaddress_public_key(char* out, char const* output_pub, char const* special_sec);

#ifdef __cplusplus
}
#endif

#endif  
