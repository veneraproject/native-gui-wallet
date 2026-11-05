                                       
                                         
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                     
                          
                 
                  
                          
                  

#include <stddef.h>
#include "fe25519.h"

 

 
static const fe25519 ecd = {{929955233495203, 466365720129213, 1662059464998953, 2033849074728123, 1442794654840575}};
 
static const fe25519 ec2d = {{1859910466990425, 932731440258426, 1072319116312658, 1815898335770999, 633789495995903}};
 
static const fe25519 sqrtm1 = {{1718705420411056, 234908883556509, 2233514472574048, 2117202627021982, 765476049583133}};

#define choose_tp crypto_sign_ed25519_amd64_51_30k_batch_choose_tp
#include "amd64.c.inc"

int monero_crypto_amd64_51_30k_ge25519_scalarmult(char* out, char const* pub, char const* sec)
{
  return scalarmult(out, pub, sec);
}

int monero_crypto_amd64_51_30k_generate_key_derivation(char* out, char const* tx_pub, char const* view_sec)
{
  return generate_key_derivation(out, tx_pub, view_sec);
}

int monero_crypto_amd64_51_30k_generate_subaddress_public_key(char* out, char const* output_pub, char const* special_sec)
{
  return generate_subaddress_public_key(out, output_pub, special_sec);
}

