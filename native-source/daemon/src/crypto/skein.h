#ifndef _SKEIN_H_
#define _SKEIN_H_     1
                                                                           
  
                                                                     
  
                                          
  
                                                                   
  
                                                                           
   
                                                                     
                                                                    
  
                                                                           
  
                                                                  
                                                                     
                                                                  
  
                                                                        
                                                                        
                                                                        
                                                                 
                                                                     
                                                                     
  
                                                                            
#include "skein_port.h"                       

typedef enum
{
  SKEIN_SUCCESS         =      0,           
  SKEIN_FAIL            =      1,
  SKEIN_BAD_HASHLEN     =      2
}
HashReturn;

typedef size_t   DataLength;                 
typedef u08b_t   BitSequence;                

 
HashReturn skein_hash(int hashbitlen,   const BitSequence *data, 
                      DataLength databitlen,  BitSequence *hashval);

#endif   
