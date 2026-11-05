  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define alignSize(pos, align) (((pos - 1) / align + 1) * align)

void* allocMemoryPages(size_t);
void setPagesRW(void*, size_t);
void setPagesRX(void*, size_t);
void setPagesRWX(void*, size_t);
void* allocLargePagesMemory(size_t);
void freePagedMemory(void*, size_t);

#ifdef __cplusplus
}
#endif
