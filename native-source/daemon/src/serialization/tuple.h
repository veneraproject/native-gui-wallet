                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#pragma once
#include <memory>
#include "serialization.h"

namespace serialization
{
  namespace detail
  {
    template <typename Archive, class T>
    bool serialize_tuple_element(Archive& ar, T& e)
    {
      return do_serialize(ar, e);
    }

    template <typename Archive>
    bool serialize_tuple_element(Archive& ar, uint64_t& e)
    {
      ar.serialize_varint(e);
      return true;
    }
  }
}

template <template <bool> class Archive, class E0, class E1, class E2>
inline bool do_serialize(Archive<false>& ar, std::tuple<E0,E1,E2>& p)
{
  size_t cnt;
  ar.begin_array(cnt);
  if (!ar.good())
    return false;
  if (cnt != 3)
    return false;

  if (!::serialization::detail::serialize_tuple_element(ar, std::get<0>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if (!::serialization::detail::serialize_tuple_element(ar, std::get<1>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if (!::serialization::detail::serialize_tuple_element(ar, std::get<2>(p)))
    return false;
  if (!ar.good())
    return false;

  ar.end_array();
  return true;
}

template <template <bool> class Archive, class E0, class E1, class E2>
inline bool do_serialize(Archive<true>& ar, std::tuple<E0,E1,E2>& p)
{
  ar.begin_array(3);
  if (!ar.good())
    return false;
  if(!::serialization::detail::serialize_tuple_element(ar, std::get<0>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if(!::serialization::detail::serialize_tuple_element(ar, std::get<1>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if(!::serialization::detail::serialize_tuple_element(ar, std::get<2>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.end_array();
  return true;
}

template <template <bool> class Archive, class E0, class E1, class E2, class E3>
inline bool do_serialize(Archive<false>& ar, std::tuple<E0,E1,E2,E3>& p)
{
  size_t cnt;
  ar.begin_array(cnt);
  if (!ar.good())
    return false;
  if (cnt != 4)
    return false;

  if (!::serialization::detail::serialize_tuple_element(ar, std::get<0>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if (!::serialization::detail::serialize_tuple_element(ar, std::get<1>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if (!::serialization::detail::serialize_tuple_element(ar, std::get<2>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if (!::serialization::detail::serialize_tuple_element(ar, std::get<3>(p)))
    return false;
  if (!ar.good())
    return false;

  ar.end_array();
  return true;
}

template <template <bool> class Archive, class E0, class E1, class E2, class E3>
inline bool do_serialize(Archive<true>& ar, std::tuple<E0,E1,E2,E3>& p)
{
  ar.begin_array(4);
  if (!ar.good())
    return false;
  if(!::serialization::detail::serialize_tuple_element(ar, std::get<0>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if(!::serialization::detail::serialize_tuple_element(ar, std::get<1>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if(!::serialization::detail::serialize_tuple_element(ar, std::get<2>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.delimit_array();
  if(!::serialization::detail::serialize_tuple_element(ar, std::get<3>(p)))
    return false;
  if (!ar.good())
    return false;
  ar.end_array();
  return true;
}

