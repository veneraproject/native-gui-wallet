 
 
 
 
 
 
 
 
 

#include <stddef.h>
#include "groestl.h"
#include "groestl_tables.h"

#define P_TYPE 0
#define Q_TYPE 1

const uint8_t shift_Values[2][8] = {{0,1,2,3,4,5,6,7},{1,3,5,7,0,2,4,6}};

const uint8_t indices_cyclic[15] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6};


#if BYTE_ORDER == LITTLE_ENDIAN
#define ROTATE_COLUMN_DOWN(v1, v2, amount_bytes, temp_var) {temp_var = (v1<<(8*amount_bytes))|(v2>>(8*(4-amount_bytes))); \
															v2 = (v2<<(8*amount_bytes))|(v1>>(8*(4-amount_bytes))); \
															v1 = temp_var;}
#else
#define ROTATE_COLUMN_DOWN(v1, v2, amount_bytes, temp_var) {temp_var = (v1>>(8*amount_bytes))|(v2<<(8*(4-amount_bytes))); \
															v2 = (v2>>(8*amount_bytes))|(v1<<(8*(4-amount_bytes))); \
															v1 = temp_var;}
#endif
  

#define COLUMN(x,y,i,c0,c1,c2,c3,c4,c5,c6,c7,tv1,tv2,tu,tl,t)				\
   tu = T[2*(uint32_t)x[4*c0+0]];			    \
   tl = T[2*(uint32_t)x[4*c0+0]+1];		    \
   tv1 = T[2*(uint32_t)x[4*c1+1]];			\
   tv2 = T[2*(uint32_t)x[4*c1+1]+1];			\
   ROTATE_COLUMN_DOWN(tv1,tv2,1,t)	\
   tu ^= tv1;						\
   tl ^= tv2;						\
   tv1 = T[2*(uint32_t)x[4*c2+2]];			\
   tv2 = T[2*(uint32_t)x[4*c2+2]+1];			\
   ROTATE_COLUMN_DOWN(tv1,tv2,2,t)	\
   tu ^= tv1;						\
   tl ^= tv2;   					\
   tv1 = T[2*(uint32_t)x[4*c3+3]];			\
   tv2 = T[2*(uint32_t)x[4*c3+3]+1];			\
   ROTATE_COLUMN_DOWN(tv1,tv2,3,t)	\
   tu ^= tv1;						\
   tl ^= tv2;						\
   tl ^= T[2*(uint32_t)x[4*c4+0]];			\
   tu ^= T[2*(uint32_t)x[4*c4+0]+1];			\
   tv1 = T[2*(uint32_t)x[4*c5+1]];			\
   tv2 = T[2*(uint32_t)x[4*c5+1]+1];			\
   ROTATE_COLUMN_DOWN(tv1,tv2,1,t)	\
   tl ^= tv1;						\
   tu ^= tv2;						\
   tv1 = T[2*(uint32_t)x[4*c6+2]];			\
   tv2 = T[2*(uint32_t)x[4*c6+2]+1];			\
   ROTATE_COLUMN_DOWN(tv1,tv2,2,t)	\
   tl ^= tv1;						\
   tu ^= tv2;   					\
   tv1 = T[2*(uint32_t)x[4*c7+3]];			\
   tv2 = T[2*(uint32_t)x[4*c7+3]+1];			\
   ROTATE_COLUMN_DOWN(tv1,tv2,3,t)	\
   tl ^= tv1;						\
   tu ^= tv2;						\
   y[i] = tu;						\
   y[i+1] = tl;


 
static void RND512P(uint8_t *x, uint32_t *y, uint32_t r) {
  uint32_t temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp;
  uint32_t* x32 = (uint32_t*)x;
  x32[ 0] ^= SWAP32LE(0x00000000)^r;
  x32[ 2] ^= SWAP32LE(0x00000010)^r;
  x32[ 4] ^= SWAP32LE(0x00000020)^r;
  x32[ 6] ^= SWAP32LE(0x00000030)^r;
  x32[ 8] ^= SWAP32LE(0x00000040)^r;
  x32[10] ^= SWAP32LE(0x00000050)^r;
  x32[12] ^= SWAP32LE(0x00000060)^r;
  x32[14] ^= SWAP32LE(0x00000070)^r;
  COLUMN(x,y, 0,  0,  2,  4,  6,  9, 11, 13, 15, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 2,  2,  4,  6,  8, 11, 13, 15,  1, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 4,  4,  6,  8, 10, 13, 15,  1,  3, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 6,  6,  8, 10, 12, 15,  1,  3,  5, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 8,  8, 10, 12, 14,  1,  3,  5,  7, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y,10, 10, 12, 14,  0,  3,  5,  7,  9, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y,12, 12, 14,  0,  2,  5,  7,  9, 11, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y,14, 14,  0,  2,  4,  7,  9, 11, 13, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
}

 
static void RND512Q(uint8_t *x, uint32_t *y, uint32_t r) {
  uint32_t temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp;
  uint32_t* x32 = (uint32_t*)x;
  x32[ 0] = ~x32[ 0];
  x32[ 1] ^= SWAP32LE(0xffffffff)^r;
  x32[ 2] = ~x32[ 2];
  x32[ 3] ^= SWAP32LE(0xefffffff)^r;
  x32[ 4] = ~x32[ 4];
  x32[ 5] ^= SWAP32LE(0xdfffffff)^r;
  x32[ 6] = ~x32[ 6];
  x32[ 7] ^= SWAP32LE(0xcfffffff)^r;
  x32[ 8] = ~x32[ 8];
  x32[ 9] ^= SWAP32LE(0xbfffffff)^r;
  x32[10] = ~x32[10];
  x32[11] ^= SWAP32LE(0xafffffff)^r;
  x32[12] = ~x32[12];
  x32[13] ^= SWAP32LE(0x9fffffff)^r;
  x32[14] = ~x32[14];
  x32[15] ^= SWAP32LE(0x8fffffff)^r;

  COLUMN(x,y, 0,  2,  6, 10, 14,  1,  5,  9, 13, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 2,  4,  8, 12,  0,  3,  7, 11, 15, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 4,  6, 10, 14,  2,  5,  9, 13,  1, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 6,  8, 12,  0,  4,  7, 11, 15,  3, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y, 8, 10, 14,  2,  6,  9, 13,  1,  5, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y,10, 12,  0,  4,  8, 11, 15,  3,  7, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y,12, 14,  2,  6, 10, 13,  1,  5,  9, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
  COLUMN(x,y,14,  0,  4,  8, 12, 15,  3,  7, 11, temp_v1, temp_v2, temp_upper_value, temp_lower_value, temp);
}

 
static void F512(uint32_t *h, const uint32_t *m) {
  int i;
  uint32_t Ptmp[2*COLS512];
  uint32_t Qtmp[2*COLS512];
  uint32_t y[2*COLS512];
  uint32_t z[2*COLS512];

  for (i = 0; i < 2*COLS512; i++) {
    z[i] = m[i];
    Ptmp[i] = h[i]^m[i];
  }

   
  RND512Q((uint8_t*)z, y, SWAP32LE(0x00000000));
  RND512Q((uint8_t*)y, z, SWAP32LE(0x01000000));
  RND512Q((uint8_t*)z, y, SWAP32LE(0x02000000));
  RND512Q((uint8_t*)y, z, SWAP32LE(0x03000000));
  RND512Q((uint8_t*)z, y, SWAP32LE(0x04000000));
  RND512Q((uint8_t*)y, z, SWAP32LE(0x05000000));
  RND512Q((uint8_t*)z, y, SWAP32LE(0x06000000));
  RND512Q((uint8_t*)y, z, SWAP32LE(0x07000000));
  RND512Q((uint8_t*)z, y, SWAP32LE(0x08000000));
  RND512Q((uint8_t*)y, Qtmp, SWAP32LE(0x09000000));

   
  RND512P((uint8_t*)Ptmp, y, SWAP32LE(0x00000000));
  RND512P((uint8_t*)y, z, SWAP32LE(0x00000001));
  RND512P((uint8_t*)z, y, SWAP32LE(0x00000002));
  RND512P((uint8_t*)y, z, SWAP32LE(0x00000003));
  RND512P((uint8_t*)z, y, SWAP32LE(0x00000004));
  RND512P((uint8_t*)y, z, SWAP32LE(0x00000005));
  RND512P((uint8_t*)z, y, SWAP32LE(0x00000006));
  RND512P((uint8_t*)y, z, SWAP32LE(0x00000007));
  RND512P((uint8_t*)z, y, SWAP32LE(0x00000008));
  RND512P((uint8_t*)y, Ptmp, SWAP32LE(0x00000009));

   
  for (i = 0; i < 2*COLS512; i++) {
    h[i] ^= Ptmp[i]^Qtmp[i];
  }
}


 
static void Transform(hashState *ctx, 
	       const uint8_t *input, 
	       int msglen) {

   
  for (; msglen >= SIZE512; 
       msglen -= SIZE512, input += SIZE512) {
    F512(ctx->chaining,(uint32_t*)input);

     
    ctx->block_counter1++;
    if (ctx->block_counter1 == 0) ctx->block_counter2++;
  }
}

 
static void OutputTransformation(hashState *ctx) {
  int j;
  uint32_t temp[2*COLS512];
  uint32_t y[2*COLS512];
  uint32_t z[2*COLS512];



	for (j = 0; j < 2*COLS512; j++) {
	  temp[j] = ctx->chaining[j];
	}
	RND512P((uint8_t*)temp, y, SWAP32LE(0x00000000));
	RND512P((uint8_t*)y, z, SWAP32LE(0x00000001));
	RND512P((uint8_t*)z, y, SWAP32LE(0x00000002));
	RND512P((uint8_t*)y, z, SWAP32LE(0x00000003));
	RND512P((uint8_t*)z, y, SWAP32LE(0x00000004));
	RND512P((uint8_t*)y, z, SWAP32LE(0x00000005));
	RND512P((uint8_t*)z, y, SWAP32LE(0x00000006));
	RND512P((uint8_t*)y, z, SWAP32LE(0x00000007));
	RND512P((uint8_t*)z, y, SWAP32LE(0x00000008));
	RND512P((uint8_t*)y, temp, SWAP32LE(0x00000009));
	for (j = 0; j < 2*COLS512; j++) {
	  ctx->chaining[j] ^= temp[j];
	}									  
}

 
static void Init(hashState* ctx) {
   

  for(size_t i = 0; i < (SIZE512/sizeof(uint32_t)); i++)
  {
	ctx->chaining[i] = 0;
  }

   
  ctx->chaining[2*COLS512-1] = SWAP32LE(u32BIG((uint32_t)HASH_BIT_LEN));

   
  ctx->buf_ptr = 0;
  ctx->block_counter1 = 0;
  ctx->block_counter2 = 0;
  ctx->bits_in_last_byte = 0;
}

 
static void Update(hashState* ctx,
		  const BitSequence* input,
		  DataLength databitlen) {
  int index = 0;
  int msglen = (int)(databitlen/8);
  int rem = (int)(databitlen%8);

   
 
  if (ctx->buf_ptr) {
    while (ctx->buf_ptr < SIZE512 && index < msglen) {
      ctx->buffer[(int)ctx->buf_ptr++] = input[index++];
    }
    if (ctx->buf_ptr < SIZE512) {
       
      if (rem) {
	ctx->bits_in_last_byte = rem;
	ctx->buffer[(int)ctx->buf_ptr++] = input[index];
      }
      return;
    }

     
    ctx->buf_ptr = 0;
    Transform(ctx, ctx->buffer, SIZE512);
  }

   
  Transform(ctx, input+index, msglen-index);
  index += ((msglen-index)/SIZE512)*SIZE512;

   
  while (index < msglen) {
    ctx->buffer[(int)ctx->buf_ptr++] = input[index++];
  }

   
 
 
  if (rem) {
    ctx->bits_in_last_byte = rem;
    ctx->buffer[(int)ctx->buf_ptr++] = input[index];
  }
}

#define BILB ctx->bits_in_last_byte

 
 
static void Final(hashState* ctx,
		 BitSequence* output) {
  int i, j = 0, hashbytelen = HASH_BIT_LEN/8;
  uint8_t *s = (BitSequence*)ctx->chaining;

   
  if (BILB) {
    ctx->buffer[(int)ctx->buf_ptr-1] &= ((1<<BILB)-1)<<(8-BILB);
    ctx->buffer[(int)ctx->buf_ptr-1] ^= 0x1<<(7-BILB);
    BILB = 0;
  }
  else ctx->buffer[(int)ctx->buf_ptr++] = 0x80;

   
  if (ctx->buf_ptr > SIZE512-LENGTHFIELDLEN) {
     
    while (ctx->buf_ptr < SIZE512) {
      ctx->buffer[(int)ctx->buf_ptr++] = 0;
    }
     
    Transform(ctx, ctx->buffer, SIZE512);
    ctx->buf_ptr = 0;
  }
  while (ctx->buf_ptr < SIZE512-LENGTHFIELDLEN) {
    ctx->buffer[(int)ctx->buf_ptr++] = 0;
  }

   
  ctx->block_counter1++;
  if (ctx->block_counter1 == 0) ctx->block_counter2++;
  ctx->buf_ptr = SIZE512;

  while (ctx->buf_ptr > SIZE512-(int)sizeof(uint32_t)) {
    ctx->buffer[(int)--ctx->buf_ptr] = (uint8_t)ctx->block_counter1;
    ctx->block_counter1 >>= 8;
  }
  while (ctx->buf_ptr > SIZE512-LENGTHFIELDLEN) {
    ctx->buffer[(int)--ctx->buf_ptr] = (uint8_t)ctx->block_counter2;
    ctx->block_counter2 >>= 8;
  }
   
  Transform(ctx, ctx->buffer, SIZE512); 
   
  OutputTransformation(ctx);

   
  for (i = SIZE512-hashbytelen; i < SIZE512; i++,j++) {
    output[j] = s[i];
  }

   
  for (i = 0; i < COLS512; i++) {
    ctx->chaining[i] = 0;
  }
  for (i = 0; i < SIZE512; i++) {
    ctx->buffer[i] = 0;
  }
}

 
void groestl(const BitSequence* data, 
		DataLength databitlen,
		BitSequence* hashval) {

  hashState context;

   
    Init(&context);


   
  Update(&context, data, databitlen);

   
  Final(&context, hashval);
}
 
 
 
 
 
 
 
 

 
