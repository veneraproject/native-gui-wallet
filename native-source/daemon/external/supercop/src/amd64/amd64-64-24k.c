                                       
                                         
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                     
                          
                 
                  
                          
                  

#include <stddef.h>
#include "fe25519.h"
#include "ge25519.h"

 

 
static const fe25519 ecd = {{0x75EB4DCA135978A3, 0x00700A4D4141D8AB, 0x8CC740797779E898, 0x52036CEE2B6FFE73}};
 
static const fe25519 ec2d = {{0xEBD69B9426B2F146, 0x00E0149A8283B156, 0x198E80F2EEF3D130, 0xA406D9DC56DFFCE7}};
 
static const fe25519 sqrtm1 = {{0xC4EE1B274A0EA0B0, 0x2F431806AD2FE478, 0x2B4D00993DFBD7A7, 0x2B8324804FC1DF0B}};

 
static void ge25519_p1p1_to_pniels(ge25519_pniels* out, ge25519_p1p1 const* in)
{
  ge25519_p1p1_to_p3((ge25519_p3*)out, in);
  const fe25519 d = out->ysubx;
  fe25519_sub(&(out->ysubx), &(out->xaddy), &(out->ysubx));
  fe25519_add(&(out->xaddy), &(out->xaddy), &d);
  fe25519_mul(&(out->t2d), &(out->t2d), &ec2d);
}

#define choose_tp crypto_sign_ed25519_amd64_64_choose_tp
#include "amd64.c.inc"

int monero_crypto_amd64_64_24k_ge25519_scalarmult(char* out, char const* pub, char const* sec)
{
  return scalarmult(out, pub, sec);
}

int monero_crypto_amd64_64_24k_generate_key_derivation(char* out, char const* tx_pub, char const* view_sec)
{
  return generate_key_derivation(out, tx_pub, view_sec);
}

int monero_crypto_amd64_64_24k_generate_subaddress_public_key(char* out, char const* output_pub, char const* special_sec)
{
  return generate_subaddress_public_key(out, output_pub, special_sec);
}

