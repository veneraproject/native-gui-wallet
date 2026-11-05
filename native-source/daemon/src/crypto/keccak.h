 
 

#ifndef KECCAK_H
#define KECCAK_H

#include <stdint.h>
#include <string.h>

#ifndef KECCAK_ROUNDS
#define KECCAK_ROUNDS 24
#endif

#ifndef ROTL64
#define ROTL64(x, y) (((x) << (y)) | ((x) >> (64 - (y))))
#endif

 
typedef struct KECCAK_CTX
{
   
  uint64_t hash[25];
   
  uint64_t message[17];
   
  size_t rest;
} KECCAK_CTX;

 
void keccak(const uint8_t *in, size_t inlen, uint8_t *md, int mdlen);

 
void keccakf(uint64_t st[25], int norounds);

void keccak1600(const uint8_t *in, size_t inlen, uint8_t *md);

void keccak_init(KECCAK_CTX * ctx);
void keccak_update(KECCAK_CTX * ctx, const uint8_t *in, size_t inlen);
void keccak_finish(KECCAK_CTX * ctx, uint8_t *md);
#endif
