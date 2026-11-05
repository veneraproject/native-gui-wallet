                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

 
 
 
 
 
 
#pragma once

#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include "serialization.h"

 
 
 
 
 
 
template <class Archive, class T>
struct variant_serialization_traits
{
};

 
 
 
 
template <class Archive, class Variant, class TBegin, class TEnd>
struct variant_reader
{
  typedef typename Archive::variant_tag_type variant_tag_type;
  typedef typename boost::mpl::next<TBegin>::type TNext;
  typedef typename boost::mpl::deref<TBegin>::type current_type;

   
  static inline bool read(Archive &ar, Variant &v, variant_tag_type t)
  {
    if(variant_serialization_traits<Archive, current_type>::get_tag() == t) {
      current_type x;
      if(!do_serialize(ar, x))
      {
        ar.set_fail();
        return false;
      }
      v = x;
    } else {
       
      return variant_reader<Archive, Variant, TNext, TEnd>::read(ar, v, t);
    }
    return true;
  }
};

 
 
template <class Archive, class Variant, class TBegin>
struct variant_reader<Archive, Variant, TBegin, TBegin>
{
  typedef typename Archive::variant_tag_type variant_tag_type;

  static inline bool read(Archive &ar, Variant &v, variant_tag_type t)
  {
    ar.set_fail();
    return false;
  }
};

template <template <bool> class Archive, typename... T>
static bool do_serialize(Archive<false> &ar, boost::variant<T...> &v) {
    using types = typename boost::variant<T...>::types;
    typename Archive<false>::variant_tag_type t;
    ar.begin_variant();
    ar.read_variant_tag(t);
    if(!variant_reader<Archive<false>, boost::variant<T...>,
       typename boost::mpl::begin<types>::type,
       typename boost::mpl::end<types>::type>::read(ar, v, t))
    {
      ar.set_fail();
      return false;
    }
    ar.end_variant();
    return true;
}

template <template <bool> class Archive>
struct variant_write_visitor : public boost::static_visitor<bool>
{
    Archive<true> &ar;

    variant_write_visitor(Archive<true> &a) : ar(a) { }

    template <class T>
    bool operator ()(T &rv) const
    {
      ar.begin_variant();
      ar.write_variant_tag(variant_serialization_traits<Archive<true>, T>::get_tag());
      if(!do_serialize(ar, rv))
      {
        ar.set_fail();
        return false;
      }
      ar.end_variant();
      return true;
    }
};

template <template <bool> class Archive, typename... T>
static bool do_serialize(Archive<true> &ar, boost::variant<T...> &v)
{
  return boost::apply_visitor(variant_write_visitor<Archive>(ar), v);
}
