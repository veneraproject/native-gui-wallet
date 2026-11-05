                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   



#pragma once

#include <boost/utility/value_init.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
namespace epee
{
#define AUTO_VAL_INIT(v)   boost::value_initialized<decltype(v)>()

namespace misc_utils
{
  bool sleep_no_w(long ms);

  template <typename T>
  T get_mid(const T &a, const T &b)
  {
     
     
    return (a/2) + (b/2) + ((a - 2*(a/2)) + (b - 2*(b/2)))/2;
  }

  template<class type_vec_type>
  type_vec_type median(std::vector<type_vec_type> &v)
  {
    if(v.empty())
      return boost::value_initialized<type_vec_type>();
    if(v.size() == 1)
      return v[0];

    size_t n = (v.size()) / 2;
    std::sort(v.begin(), v.end());
     
    if(v.size()%2)
    { 
      return v[n];
    }else 
    { 
      return get_mid<type_vec_type>(v[n-1],v[n]);
    }

  }

   
   
   

  struct call_befor_die_base
  {
    virtual ~call_befor_die_base(){}
  };

  typedef boost::shared_ptr<call_befor_die_base> auto_scope_leave_caller;


  template<class t_scope_leave_handler>
  struct call_befor_die: public call_befor_die_base
  {
    t_scope_leave_handler m_func;
    call_befor_die(t_scope_leave_handler f):m_func(f)
    {}
    ~call_befor_die()
    {
      try { m_func(); }
      catch (...) {   }
    }
  };

  template<class t_scope_leave_handler>
  auto_scope_leave_caller create_scope_leave_handler(t_scope_leave_handler f)
  {
    auto_scope_leave_caller slc(new call_befor_die<t_scope_leave_handler>(f));
    return slc;
  }

  template<typename T> struct struct_init: T
  {
    struct_init(): T{} {}
  };

}
}
