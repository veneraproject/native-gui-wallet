                                       
                                         
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include "common/data_cache.h"
#include "cryptonote_basic/cryptonote_basic.h"

namespace cryptonote
{

 
static constexpr const size_t RCT_VER_CACHE_SIZE = 8192;

using rct_ver_cache_t = ::tools::data_cache<::crypto::hash, RCT_VER_CACHE_SIZE>;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
bool ver_rct_non_semantics_simple_cached
(
    transaction& tx,
    const rct::ctkeyM& mix_ring,
    rct_ver_cache_t& cache,
    std::uint8_t rct_type_to_cache
);

}  
