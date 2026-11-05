                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <chrono>
#include "crypto/crypto.h"

namespace crypto
{
   
  template<typename D>
  struct random_poisson_duration
  {
    using result_type = D;                  
    using rep = typename result_type::rep;  

     
    explicit random_poisson_duration(result_type average)
      : dist(average.count() < 0 ? 0 : average.count())
    {}

     
    result_type operator()()
    {
      crypto::random_device rand{};
      return result_type{dist(rand)};
    }

  private:
    std::poisson_distribution<rep> dist;
  };

     
 
 
 
 

   
  using random_poisson_seconds = random_poisson_duration<std::chrono::seconds>;
   
  using random_poisson_subseconds =
    random_poisson_duration<std::chrono::duration<std::chrono::milliseconds::rep, std::ratio<1, 4>>>;
}
