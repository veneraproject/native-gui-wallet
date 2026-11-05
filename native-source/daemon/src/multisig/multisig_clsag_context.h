                                       
                                         
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 
 
 
 
 


#pragma once

#include "ringct/rctTypes.h"

#include <vector>


namespace multisig {

namespace signing {

class CLSAG_context_t final {
private:
   
  bool initialized;
   
  rct::keyV c_params;
   
  std::size_t c_params_L_offset;
  std::size_t c_params_R_offset;
   
   
   
   
   
   
   
   
   
   
   
   
   
   
  rct::keyV b_params;
   
  std::size_t b_params_L_offset;
  std::size_t b_params_R_offset;
   
   
   
  rct::key mu_P;
  rct::key mu_C;
   
  std::size_t n;
   
  rct::geDsmp wH_l_precomp;
   
  std::vector<rct::geDsmp> W_precomp;
   
  std::vector<rct::geDsmp> H_precomp;
   
  rct::geDsmp G_precomp;
   
  std::size_t l;
   
  rct::keyV s;
   
  std::size_t num_alpha_components;
public:
  CLSAG_context_t() : initialized{false} {}

   
  bool init(
    const rct::keyV& P,
    const rct::keyV& C_nonzero,
    const rct::key& C_offset,
    const rct::key& message,
    const rct::key& I,
    const rct::key& D,
    const unsigned int l,
    const rct::keyV& s,
    const std::size_t num_alpha_components
  );

   
  bool combine_alpha_and_compute_challenge(
     
    const rct::keyV& total_alpha_G,
     
    const rct::keyV& total_alpha_H,
     
    const rct::keyV& alpha,
     
     
    rct::key& alpha_combined,
     
    rct::key& c_0,
     
    rct::key& c
  );

   
  bool get_mu(
    rct::key& mu_P,
    rct::key& mu_C
  ) const;
};

}  

}  
