                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <array>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <boost/utility/string_ref.hpp>

#include "wipeable_string.h"
#include "span.h"

namespace epee
{
  struct to_hex
  {
     
    static std::string string(const span<const std::uint8_t> src);
     
    static epee::wipeable_string wipeable_string(const span<const std::uint8_t> src);
    template<typename T> static epee::wipeable_string wipeable_string(const T &pod) { return wipeable_string(span<const uint8_t>((const uint8_t*)&pod, sizeof(pod))); }

     
    template<std::size_t N>
    static std::array<char, N * 2> array(const std::array<std::uint8_t, N>& src) noexcept
    {
      std::array<char, N * 2> out;
      static_assert(N <= 128, "keep the stack size down");
      buffer_unchecked(out.data(), {src.data(), src.size()});
      return out;
    }

     
    template<typename T>
    static std::array<char, sizeof(T) * 2> array(const T& src) noexcept
    {
      std::array<char, sizeof(T) * 2> out;
      static_assert(sizeof(T) <= 128, "keep the stack size down");
      buffer_unchecked(out.data(), as_byte_span(src));
      return out;
    }

     
    static void buffer(std::ostream& out, const span<const std::uint8_t> src);

     
    static void formatted(std::ostream& out, const span<const std::uint8_t> src);

  private:
    template<typename T> T static convert(const span<const std::uint8_t> src);

     
    static void buffer_unchecked(char* out, const span<const std::uint8_t> src) noexcept;
  };

   
  struct from_hex
  {
    static bool to_string(std::string& out, boost::string_ref src);

    static bool to_buffer(span<std::uint8_t> out, boost::string_ref src) noexcept;

  private:
    static bool to_buffer_unchecked(std::uint8_t* out, boost::string_ref src) noexcept;
  };

   
  struct from_hex_locale
  {
      static std::vector<uint8_t> to_vector(boost::string_ref src);
  };
}
