#ifndef VARIANT2_INT_SQRT_H
#define VARIANT2_INT_SQRT_H

#include <math.h>
#include <float.h>

#define VARIANT2_INTEGER_MATH_SQRT_STEP_SSE2() \
  do { \
    const __m128i exp_double_bias = _mm_set_epi64x(0, 1023ULL << 52); \
    __m128d x = _mm_castsi128_pd(_mm_add_epi64(_mm_cvtsi64_si128(sqrt_input >> 12), exp_double_bias)); \
    x = _mm_sqrt_sd(_mm_setzero_pd(), x); \
    sqrt_result = (uint64_t)(_mm_cvtsi128_si64(_mm_sub_epi64(_mm_castpd_si128(x), exp_double_bias))) >> 19; \
  } while(0)

#define VARIANT2_INTEGER_MATH_SQRT_STEP_FP64() \
  do { \
    sqrt_result = sqrt(sqrt_input + 18446744073709551616.0) * 2.0 - 8589934592.0; \
  } while(0)

#define VARIANT2_INTEGER_MATH_SQRT_STEP_REF() \
  sqrt_result = integer_square_root_v2(sqrt_input)

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
static inline uint32_t integer_square_root_v2(uint64_t n)
{
  uint64_t r = 1ULL << 63;

  for (uint64_t bit = 1ULL << 60; bit; bit >>= 2)
  {
    const bool b = (n < r + bit);
    const uint64_t n_next = n - (r + bit);
    const uint64_t r_next = r + bit * 2;
    n = b ? n : n_next;
    r = b ? r : r_next;
    r >>= 1;
  }

  return r * 2 + ((n > r) ? 1 : 0);
}

 
 
 

 
 
 

 

 
 
 
 
 
 

 

 
 
 
 
 
 
 

 
 

 

 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 

 

 
 
 
 
 
 
 
 

 

 
 

 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 

#define VARIANT2_INTEGER_MATH_SQRT_FIXUP(r) \
  do { \
    const uint64_t s = r >> 1; \
    const uint64_t b = r & 1; \
    const uint64_t r2 = (uint64_t)(s) * (s + b) + (r << 32); \
    r += ((r2 + b > sqrt_input) ? -1 : 0) + ((r2 + (1ULL << 32) < sqrt_input - s) ? 1 : 0); \
  } while(0)

#endif
