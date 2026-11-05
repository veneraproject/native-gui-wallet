                                       
                                         
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                      
                          
                 
                  
                          
                  

#include <string.h>
#include "ge25519.h"

extern void choose_tp(ge25519_pniels *t, unsigned long long pos, signed long long b, const ge25519_pniels *base_multiples);

 
 
static int unpack_vartime(ge25519_p3 *r, const unsigned char p[32])
{
  fe25519 t, chk, num, den, den2, den4, den6;
  unsigned char par = p[31] >> 7;

  fe25519_setint(&r->z,1);
  fe25519_unpack(&r->y, p); 
  fe25519_square(&num, &r->y);  
  fe25519_mul(&den, &num, &ecd);  
  fe25519_sub(&num, &num, &r->z);  
  fe25519_add(&den, &r->z, &den);  

   
 
 
  fe25519_square(&den2, &den);
  fe25519_square(&den4, &den2);
  fe25519_mul(&den6, &den4, &den2);
  fe25519_mul(&t, &den6, &num);
  fe25519_mul(&t, &t, &den);

  fe25519_pow2523(&t, &t);
   
 
  fe25519_mul(&t, &t, &num);
  fe25519_mul(&t, &t, &den);
  fe25519_mul(&t, &t, &den);
  fe25519_mul(&r->x, &t, &den);

   
 
  fe25519_square(&chk, &r->x);
  fe25519_mul(&chk, &chk, &den);
  if (!fe25519_iseq_vartime(&chk, &num))
    fe25519_mul(&r->x, &r->x, &sqrtm1);

   
 
  fe25519_square(&chk, &r->x);
  fe25519_mul(&chk, &chk, &den);
  if (!fe25519_iseq_vartime(&chk, &num))
    return -1;

   
 
  if(fe25519_getparity(&r->x) == (1-par))  
    fe25519_neg(&r->x, &r->x);

  fe25519_mul(&r->t, &r->x, &r->y);
  return 0;
}

static void p3_to_pniels(ge25519_pniels* out, ge25519_p3 const* src)
{
  fe25519_sub(&out->ysubx, &src->y, &src->x);
  fe25519_add(&out->xaddy, &src->x, &src->y);
  fe25519_mul(&out->t2d, &src->t, &ec2d);
  out->z = src->z;
}

static void negate(ge25519* out)
{
  fe25519_neg(&out->x, &out->x);
  fe25519_neg(&out->t, &out->t);
}

 
static void scalarmult_p1p1(ge25519_p1p1* r, ge25519_pniels const* base, char const* sec)
{
  signed char b[64];
  ge25519_pniels t;
  ge25519_p3 tp3;

  sc25519 s;
  memcpy(s.v, sec, sizeof(s));
  sc25519_window4(b, &s);

   
  fe25519_setint(&tp3.x, 0);
  fe25519_setint(&tp3.y, 1);
  fe25519_setint(&tp3.t, 0);
  fe25519_setint(&tp3.z, 1);
   

  for (int i = 63;   ; --i)
  {
    choose_tp(&t, (unsigned long long) 0, (signed long long) b[i], base);
    ge25519_pnielsadd_p1p1(r, &tp3, &t);

    if (i == 0) break;

    ge25519_p1p1_to_p2((ge25519_p2*)&tp3, r);

    ge25519_dbl_p1p1(r,(ge25519_p2 *)&tp3);
    ge25519_p1p1_to_p2((ge25519_p2 *)&tp3, r);
    ge25519_dbl_p1p1(r,(ge25519_p2 *)&tp3);
    ge25519_p1p1_to_p2((ge25519_p2 *)&tp3, r);
    ge25519_dbl_p1p1(r,(ge25519_p2 *)&tp3);
    ge25519_p1p1_to_p2((ge25519_p2 *)&tp3, r);
    ge25519_dbl_p1p1(r,(ge25519_p2 *)&tp3);
    ge25519_p1p1_to_p3(&tp3, r);
  }
}

 
static void base_precomp(ge25519_pniels* base, ge25519_p3 const* r)
{
  ge25519_p1p1 tp1p1;

  p3_to_pniels(&base[0], r);
  for (int i = 0; i < 7; ++i)
  {
    ge25519_pnielsadd_p1p1(&tp1p1, r, &base[i]);
    ge25519_p1p1_to_pniels(&base[i + 1], &tp1p1);
  }
}

static int scalarmult(char* out, char const* pub, char const* sec)
{
  ge25519 unpacked;
  ge25519_pniels base[8];
  ge25519_p1p1 tp1p1;
  if (unpack_vartime(&unpacked, (unsigned char const*)pub) != 0)
    return -1;

  base_precomp(base, &unpacked);
  scalarmult_p1p1(&tp1p1, base, sec);
  ge25519_p1p1_to_p3(&unpacked, &tp1p1);

  ge25519_pack((unsigned char*)out, &unpacked);
  return 0;
}

static int generate_key_derivation(char* out, char const* tx_pub, char const* view_sec)
{
  ge25519 unpacked;
  ge25519_pniels base[8];
  ge25519_p1p1 tp1p1;
  if (unpack_vartime(&unpacked, (unsigned char const*)tx_pub) != 0)
    return -1;

  base_precomp(base, &unpacked);
  scalarmult_p1p1(&tp1p1, base, view_sec);

   
  ge25519_p1p1_to_p2((ge25519_p2*)&unpacked, &tp1p1);
  ge25519_dbl_p1p1(&tp1p1,(ge25519_p2 *)&unpacked);
  ge25519_p1p1_to_p2((ge25519_p2 *)&unpacked, &tp1p1);
  ge25519_dbl_p1p1(&tp1p1,(ge25519_p2 *)&unpacked);
  ge25519_p1p1_to_p2((ge25519_p2 *)&unpacked, &tp1p1);
  ge25519_dbl_p1p1(&tp1p1,(ge25519_p2 *)&unpacked);
  ge25519_p1p1_to_p3(&unpacked, &tp1p1);

  ge25519_pack((unsigned char*)out, &unpacked);
  return 0;
}

static void generate_subaddress_public_key_base(char* out, ge25519 const* output_pub, char const* special_sec)
{
  ge25519 p1;
  sc25519 p2;

  memcpy(p2.v, special_sec, sizeof(p2.v));
  ge25519_scalarmult_base(&p1, &p2);
  negate(&p1);  
  ge25519_add(&p1, output_pub, &p1);
  ge25519_pack((unsigned char*)out, &p1);
}

static int generate_subaddress_public_key(char* out, char const* output_pub, char const* special_sec)
{
  ge25519 p;
  if (unpack_vartime(&p, (unsigned char const*)output_pub) != 0)
    return -1;

  generate_subaddress_public_key_base(out, &p, special_sec);
  return 0;
}

