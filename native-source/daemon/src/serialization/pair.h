                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#pragma once
#include <memory>
#include <boost/type_traits/make_unsigned.hpp>
#include "serialization.h"

namespace serialization
{
  namespace detail
  {
    template<typename T>
    inline constexpr bool use_pair_varint() noexcept
    {
      return std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) > 1;
    }

    template <typename Archive, class T>
    typename std::enable_if<!use_pair_varint<T>(), bool>::type
    serialize_pair_element(Archive& ar, T& e)
    {
      return do_serialize(ar, e);
    }

    template<typename Archive, typename T>
    typename std::enable_if<use_pair_varint<T>(), bool>::type
    serialize_pair_element(Archive& ar, T& e)
    {
      static constexpr const bool previously_varint = std::is_same<uint64_t, T>();

      if (!previously_varint && ar.varint_bug_backward_compatibility_enabled() && !typename Archive::is_saving())
        return do_serialize(ar, e);
      ar.serialize_varint(e);
      return true;
    }
  }
}

template <template <bool> class Archive, class F, class S>
inline bool do_serialize(Archive<false>& ar, std::pair<F,S>& p)
{
  size_t cnt;
  ar.begin_array(cnt);
  if (!ar.good())
    return false;
  if (cnt != 2)
    return false;

  if (!::serialization::detail::serialize_pair_element(ar, p.first))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if (!::serialization::detail::serialize_pair_element(ar, p.second))
    return false;
  if (!ar.good())
    return false;

  ar.end_array();
  return true;
}

template <template <bool> class Archive, class F, class S>
inline bool do_serialize(Archive<true>& ar, std::pair<F,S>& p)
{
  ar.begin_array(2);
  if (!ar.good())
    return false;
  if(!::serialization::detail::serialize_pair_element(ar, p.first))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if(!::serialization::detail::serialize_pair_element(ar, p.second))
    return false;
  if (!ar.good())
    return false;
  ar.end_array();
  return true;
}

