  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#ifndef RANDOMX_H
#define RANDOMX_H

#include <stddef.h>
#include <stdint.h>

#define RANDOMX_HASH_SIZE 32
#define RANDOMX_DATASET_ITEM_SIZE 64

#ifndef RANDOMX_EXPORT
#define RANDOMX_EXPORT
#endif

typedef enum {
  RANDOMX_FLAG_DEFAULT = 0,
  RANDOMX_FLAG_LARGE_PAGES = 1,
  RANDOMX_FLAG_HARD_AES = 2,
  RANDOMX_FLAG_FULL_MEM = 4,
  RANDOMX_FLAG_JIT = 8,
  RANDOMX_FLAG_SECURE = 16,
  RANDOMX_FLAG_ARGON2_SSSE3 = 32,
  RANDOMX_FLAG_ARGON2_AVX2 = 64,
  RANDOMX_FLAG_ARGON2 = 96
} randomx_flags;

typedef struct randomx_dataset randomx_dataset;
typedef struct randomx_cache randomx_cache;
typedef struct randomx_vm randomx_vm;


#if defined(__cplusplus)

#ifdef __cpp_constexpr
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

inline CONSTEXPR randomx_flags operator |(randomx_flags a, randomx_flags b) {
	return static_cast<randomx_flags>(static_cast<int>(a) | static_cast<int>(b));
}
inline CONSTEXPR randomx_flags operator &(randomx_flags a, randomx_flags b) {
	return static_cast<randomx_flags>(static_cast<int>(a) & static_cast<int>(b));
}
inline randomx_flags& operator |=(randomx_flags& a, randomx_flags b) {
	return a = a | b;
}

extern "C" {
#endif

 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT randomx_flags randomx_get_flags(void);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT randomx_cache *randomx_alloc_cache(randomx_flags flags);

 
 
 
 
 
 
 
 
RANDOMX_EXPORT void randomx_init_cache(randomx_cache *cache, const void *key, size_t keySize);

 
 
 
 
 
RANDOMX_EXPORT void randomx_release_cache(randomx_cache* cache);

 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT randomx_dataset *randomx_alloc_dataset(randomx_flags flags);

 
 
 
 
 
RANDOMX_EXPORT unsigned long randomx_dataset_item_count(void);

 
 
 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT void randomx_init_dataset(randomx_dataset *dataset, randomx_cache *cache, unsigned long startItem, unsigned long itemCount);

 
 
 
 
 
 
 
 
RANDOMX_EXPORT void *randomx_get_dataset_memory(randomx_dataset *dataset);

 
 
 
 
 
RANDOMX_EXPORT void randomx_release_dataset(randomx_dataset *dataset);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT randomx_vm *randomx_create_vm(randomx_flags flags, randomx_cache *cache, randomx_dataset *dataset);

 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT void randomx_vm_set_cache(randomx_vm *machine, randomx_cache* cache);

 
 
 
 
 
 
 
RANDOMX_EXPORT void randomx_vm_set_dataset(randomx_vm *machine, randomx_dataset *dataset);

 
 
 
 
 
RANDOMX_EXPORT void randomx_destroy_vm(randomx_vm *machine);

 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT void randomx_calculate_hash(randomx_vm *machine, const void *input, size_t inputSize, void *output);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT void randomx_calculate_hash_first(randomx_vm* machine, const void* input, size_t inputSize);
RANDOMX_EXPORT void randomx_calculate_hash_next(randomx_vm* machine, const void* nextInput, size_t nextInputSize, void* output);
RANDOMX_EXPORT void randomx_calculate_hash_last(randomx_vm* machine, void* output);

 
 
 
 
 
 
 
 
 
RANDOMX_EXPORT void randomx_calculate_commitment(const void* input, size_t inputSize, const void* hash_in, void* com_out);

#if defined(__cplusplus)
}
#endif

#endif
