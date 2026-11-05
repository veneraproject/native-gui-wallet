                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace serialization
{
  namespace detail
  {
    template<typename T>
    inline constexpr bool use_container_varint() noexcept
    {
      return std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) > 1;
    }

    template <typename Archive, class T>
    typename std::enable_if<!use_container_varint<T>(), bool>::type
    serialize_container_element(Archive& ar, T& e)
    {
      return do_serialize(ar, e);
    }

    template<typename Archive, typename T>
    typename std::enable_if<use_container_varint<T>(), bool>::type
    serialize_container_element(Archive& ar, T& e)
    {
      static constexpr const bool previously_varint = std::is_same<uint64_t, T>() || std::is_same<uint32_t, T>();

      if (!previously_varint && ar.varint_bug_backward_compatibility_enabled() && !typename Archive::is_saving())
        return do_serialize(ar, e);
      ar.serialize_varint(e);
      return true;
    }

     
    template<typename... C>
    void do_reserve(const C&...) {}
    template<typename C>
    auto do_reserve(C &c, std::size_t N, std::size_t B) -> decltype(c.reserve(N))
    {
      using T = typename C::value_type;

      static constexpr std::size_t max_compression_ratio =
        is_blob_type<T>::type::value ? 1 :
        use_container_varint<T>() ? sizeof(T) :
        (std::is_same<T, char>::value || std::is_same<T, unsigned char>::value) ? 1:
        4;  

       
      B /= sizeof(T);
      B = std::max(std::size_t(1), B);
      if (std::numeric_limits<std::size_t>::max() / max_compression_ratio <= B)
        B = std::numeric_limits<std::size_t>::max();
      else
        B *= max_compression_ratio;

      return c.reserve(std::min(N, B));
    }
  }
}

template <template <bool> class Archive, typename C>
bool do_serialize_container(Archive<false> &ar, C &v, size_t max_cnt = std::numeric_limits<size_t>::max())
{
  size_t cnt;
  ar.begin_array(cnt);
  if (!ar.good())
    return false;
  v.clear();

   
  if (max_cnt < cnt || ar.remaining_bytes() < cnt) {
    ar.set_fail();
    return false;
  }

  ::serialization::detail::do_reserve(v, cnt, ar.remaining_bytes());

  for (size_t i = 0; i < cnt; i++) {
    if (i > 0)
      ar.delimit_array();
    typename C::value_type e;
    if (!::serialization::detail::serialize_container_element(ar, e))
      return false;
    ::serialization::detail::do_add(v, std::move(e));
    if (!ar.good())
      return false;
  }
  ar.end_array();
  return true;
}

template <template <bool> class Archive, typename C>
bool do_serialize_container(Archive<true> &ar, C &v, size_t max_cnt = std::numeric_limits<size_t>::max())
{
  size_t cnt = v.size();
  if (cnt > max_cnt) {
    ar.set_fail();
    return false;
  }
  ar.begin_array(cnt);
  for (auto i = v.begin(); i != v.end(); ++i)
  {
    if (!ar.good())
      return false;
    if (i != v.begin())
      ar.delimit_array();
    if(!::serialization::detail::serialize_container_element(ar, (typename C::value_type&)*i))
      return false;
    if (!ar.good())
      return false;
  }
  ar.end_array();
  return true;
}
