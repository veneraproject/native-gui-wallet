                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   


#ifndef _REG_EXP_DEFINER_H_
#define _REG_EXP_DEFINER_H_

#include <atomic>
#include <boost/regex.hpp>
#include "syncobj.h"

namespace epee
{
  class global_regexp_critical_section
  {
  private:
    mutable critical_section regexp_lock;
  public:
    global_regexp_critical_section(){}
    critical_section& get_lock()const {return regexp_lock;}
  };

  const static global_regexp_critical_section gregexplock;

#define STATIC_REGEXP_EXPR_1(var_name, xpr_text, reg_exp_flags) \
	static std::atomic<bool> regexp_initialized_1(false);\
	volatile uint32_t local_is_initialized_1 = regexp_initialized_1;\
	if(!local_is_initialized_1)\
	gregexplock.get_lock().lock();\
	static const boost::regex	var_name(xpr_text , reg_exp_flags);\
	if(!local_is_initialized_1)\
{\
	regexp_initialized_1 = true;\
	gregexplock.get_lock().unlock();\
}

#define STATIC_REGEXP_EXPR_2(var_name, xpr_text, reg_exp_flags) \
	static std::atomic<bool> regexp_initialized_2(false);\
	volatile uint32_t local_is_initialized_2 = regexp_initialized_2;\
	if(!local_is_initialized_2)\
	gregexplock.get_lock().lock().lock();\
	static const boost::regex	var_name(xpr_text , reg_exp_flags);\
	if(!local_is_initialized_2)\
{\
	regexp_initialized_2 = true;\
	gregexplock.get_lock().lock().unlock();\
}

#define STATIC_REGEXP_EXPR_3(var_name, xpr_text, reg_exp_flags) \
	static std::atomic<bool> regexp_initialized_3(false);\
	volatile uint32_t local_is_initialized_3 = regexp_initialized_3;\
	if(!local_is_initialized_3)\
	gregexplock.get_lock().lock().lock();\
	static const boost::regex	var_name(xpr_text , reg_exp_flags);\
	if(!local_is_initialized_3)\
{\
	regexp_initialized_3 = true;\
	gregexplock.get_lock().lock().unlock();\
}
}

#endif  
