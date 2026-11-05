                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#pragma once

#include "cryptonote_protocol/enums.h"

namespace cryptonote
{
   
   
   
  struct tx_verification_context
  {
    static_assert(unsigned(relay_method::none) == 0, "default m_relay initialization is not to relay_method::none");

    relay_method m_relay;  
    bool m_verifivation_failed;  
     
     
     
     
    bool m_no_drop_offense;
    bool m_verifivation_impossible;  
    bool m_added_to_pool; 
    bool m_low_mixin;
    bool m_double_spend;
    bool m_invalid_input;
    bool m_invalid_output;
    bool m_too_big;
    bool m_overspend;
    bool m_fee_too_low;
    bool m_too_few_outputs;
    bool m_tx_extra_too_big;
  };

  struct block_verification_context
  {
    bool m_added_to_main_chain;
    bool m_verifivation_failed;  
    bool m_marked_as_orphaned;
    bool m_already_exists;
    bool m_partial_block_reward;
    bool m_bad_pow;  
  };
}
