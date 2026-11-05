                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <limits>
#include <type_traits>

namespace
{
   
  template <typename T>
  struct HasCredits
  {
    template<typename U, uint64_t (U::*)> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, &U::credits>*);
    template<typename U> static int Test(...);
    static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
  };
}

namespace tools
{
  struct rpc_payment_state_t
  {
    uint64_t credits;
    uint64_t expected_spent;
    uint64_t discrepancy;
    std::string top_hash;
    bool stale;

    rpc_payment_state_t(): credits(0), expected_spent(0), discrepancy(0), stale(true) {}
  };

  static inline void check_rpc_cost(rpc_payment_state_t &rpc_payment_state, const char *call, uint64_t post_call_credits, uint64_t pre_call_credits, double expected_cost)
  {
    uint64_t expected_credits = (uint64_t)expected_cost;
    if (expected_credits == 0)
      expected_credits = 1;

    rpc_payment_state.credits = post_call_credits;
    rpc_payment_state.expected_spent += expected_credits;

    if (pre_call_credits <= post_call_credits)
      return;

    uint64_t cost = pre_call_credits - post_call_credits;

    if (cost == expected_credits)
    {
      MDEBUG("Call " << call << " cost " << cost << " credits");
      return;
    }
    MWARNING("Call " << call << " cost " << cost << " credits, expected " << expected_credits);

    if (cost > expected_credits)
    {
      uint64_t d = cost - expected_credits;
      if (rpc_payment_state.discrepancy > std::numeric_limits<uint64_t>::max() - d)
      {
        MERROR("Integer overflow in credit discrepancy calculation, setting to max");
        rpc_payment_state.discrepancy = std::numeric_limits<uint64_t>::max();
      }
      else
      {
        rpc_payment_state.discrepancy += d;
      }
    }
  }
}
