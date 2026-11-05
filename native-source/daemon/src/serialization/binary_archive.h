                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

 
 
 
#pragma once

#include <cassert>
#include <iostream>
#include <iterator>
#include <boost/endian/conversion.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include "common/varint.h"
#include "span.h"
#include "warnings.h"

 
PUSH_WARNINGS
DISABLE_VS_WARNINGS(4244)

 

 
 
 
 
 
 
 
 
template <bool IsSaving>
struct binary_archive_base
{
  typedef binary_archive_base<IsSaving> base_type;
  typedef boost::mpl::bool_<IsSaving> is_saving;

  typedef uint8_t variant_tag_type;

  explicit binary_archive_base() { }
  
   
  void tag(const char *) { }
  void begin_object() { }
  void end_object() { }
  void begin_variant() { }
  void end_variant() { }
};

 
 
 
 
 
 
 
 
 
 
template <bool W>
struct binary_archive;


template <>
struct binary_archive<false> : public binary_archive_base<false>
{
  explicit binary_archive(epee::span<const std::uint8_t> s)
    : base_type(), bytes_(s), begin_(s.begin()), good_(true), varint_bug_backward_compatibility_(false)
  {}

  bool good() const noexcept { return good_; }
  void set_fail() noexcept { good_ = false; }

   
  bool eof() const noexcept { return bytes_.empty(); }
  std::size_t getpos() const noexcept { return bytes_.begin() - begin_; }

  template <class T>
  void serialize_int(T &v)
  {
    serialize_uint(*(typename boost::make_unsigned<T>::type *)&v);
  }

   
 
 
 
  template <class T>
  void serialize_uint(T &v)
  {
    const std::size_t actual = bytes_.remove_prefix(sizeof(T));
    good_ &= (actual == sizeof(T));
    if (actual == sizeof(T))
    {
      std::memcpy(std::addressof(v), bytes_.data() - sizeof(T), sizeof(T));
      boost::endian::little_to_native_inplace(v);  
    }
    else
      v = 0;  
  }
  
  void serialize_blob(void *buf, size_t len, const char *delimiter="")
  {
    const std::size_t actual = bytes_.remove_prefix(len);
    good_ &= (len == actual);
    std::memcpy(buf, bytes_.data() - actual, actual);
  }
  
  template <class T>
  void serialize_varint(T &v)
  {
    serialize_uvarint(*(typename boost::make_unsigned<T>::type *)(&v));
  }

  template <class T>
  void serialize_uvarint(T &v)
  {
    auto current = bytes_.cbegin();
    auto end = bytes_.cend();
    good_ &= (1 <= tools::read_varint(current, end, v));
    current = std::min(current, bytes_.cend());
    bytes_ = {current, std::size_t(bytes_.cend() - current)};
  }

  void begin_array(size_t &s)
  {
    serialize_varint(s);
  }

  void begin_array() { }
  void delimit_array() { }
  void end_array() { }

  void begin_string(const char *delimiter  ) { }
  void end_string(const char *delimiter    ) { }

  void read_variant_tag(variant_tag_type &t) {
    serialize_int(t);
  }

  size_t remaining_bytes() const noexcept { return good() ? bytes_.size() : 0; }
  void enable_varint_bug_backward_compatibility() { varint_bug_backward_compatibility_ = true; }
  bool varint_bug_backward_compatibility_enabled() const { return varint_bug_backward_compatibility_; }
protected:
  epee::span<const std::uint8_t> bytes_;
  std::uint8_t const* const begin_;
  bool good_;
  bool varint_bug_backward_compatibility_;
};

template <>
struct binary_archive<true> : public binary_archive_base<true>
{
  typedef std::ostream stream_type;
  explicit binary_archive(stream_type &s) : base_type(), stream_(s) { }

  bool good() const { return stream_.good(); }
  void set_fail() { stream_.setstate(std::ios::failbit); }

  std::streampos getpos() const { return stream_.tellp(); }

  template <class T>
  void serialize_int(T v)
  {
    serialize_uint(static_cast<typename boost::make_unsigned<T>::type>(v));
  }
  template <class T>
  void serialize_uint(T v)
  {
    for (size_t i = 0; i < sizeof(T); i++) {
      stream_.put((char)(v & 0xff));
      if (1 < sizeof(T)) v >>= 8;
    }
  }

  void serialize_blob(void *buf, size_t len, const char *delimiter="")
  {
    stream_.write((char *)buf, len);
  }

  template <class T>
  void serialize_varint(T &v)
  {
    serialize_uvarint(*(typename boost::make_unsigned<T>::type *)(&v));
  }

  template <class T>
  void serialize_uvarint(T &v)
  {
    typedef std::ostreambuf_iterator<char> it;
    tools::write_varint(it(stream_), v);
  }
  void begin_array(size_t s)
  {
    serialize_varint(s);
  }
  void begin_array() { }
  void delimit_array() { }
  void end_array() { }

  void begin_string(const char *delimiter="\"") { }
  void end_string(const char *delimiter="\"") { }

  void write_variant_tag(variant_tag_type t) {
    serialize_int(t);
  }

  bool varint_bug_backward_compatibility_enabled() const { return false; }
protected:
  stream_type& stream_;
};

POP_WARNINGS
