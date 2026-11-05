                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                         

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#ifdef HAVE_EXPLICIT_BZERO
#include <strings.h>
#endif
#include "memwipe.h"

#if defined(_MSC_VER)
#define SCARECROW \
    __asm;
#else
#define SCARECROW \
    __asm__ __volatile__("" : : "r"(ptr) : "memory");
#endif

#ifdef HAVE_MEMSET_S

void *memwipe(void *ptr, size_t n)
{
  if (n > 0 && memset_s(ptr, n, 0, n))
  {
#ifdef NDEBUG
    fprintf(stderr, "Error: memset_s failed\n");
    _exit(1);
#else
    abort();
#endif
  }
  SCARECROW  
  return ptr;
}

#elif defined HAVE_EXPLICIT_BZERO

void *memwipe(void *ptr, size_t n)
{
  if (n > 0)
    explicit_bzero(ptr, n);
  SCARECROW
  return ptr;
}

#else

 

                                                                         
                                                                        
                                                                  
                                        
  
                                                                         
                                                                        
                       
  
                                                                        
                                                                           
                                         
  
                                
                                                   
                           
   
static void memory_cleanse(void *ptr, size_t len)
{
    memset(ptr, 0, len);

     
 
 
    SCARECROW
}

void *memwipe(void *ptr, size_t n)
{
  if (n > 0)
    memory_cleanse(ptr, n);
  SCARECROW
  return ptr;
}

#endif
