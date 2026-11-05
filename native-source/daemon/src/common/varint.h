                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#pragma once

#include <limits>
#include <type_traits>
#include <utility>
#include <sstream>
#include <string>
 
 
 
 
 
 
 
 
 
 
 
 
 
 

namespace tools {

   
 
  enum {
     
    EVARINT_OVERFLOW = -1,
     
    EVARINT_REPRESENT = -2,
  };

   
 
  template<typename OutputIt, typename T>
   
  typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value, void>::type 
  write_varint(OutputIt &&dest, T i) {
     
    while (i >= 0x80) {
      *dest = (static_cast<char>(i) & 0x7f) | 0x80; 
      ++dest;
      i >>= 7;			 
    }
     
    *dest = static_cast<char>(i);
    dest++;			 
  }

   
 
  template<typename T>
    std::string get_varint_data(const T& v)
    {
      std::stringstream ss;
      write_varint(std::ostreambuf_iterator<char>(ss), v);
      return ss.str();
    }
   
  
  template<int bits, typename InputIt, typename T>
    typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value && 0 <= bits && bits <= std::numeric_limits<T>::digits, int>::type
    read_varint(InputIt &&first, InputIt &&last, T &write) {
    int read = 0;
    write = 0;
    for (int shift = 0;; shift += 7) {
      if (first == last) {
	return read; 
      }
      unsigned char byte = *first;
      ++first;
      ++read;
      if (shift + 7 >= bits && byte >= 1 << (bits - shift)) {
	return EVARINT_OVERFLOW;
      }
      if (byte == 0 && shift != 0) {
	return EVARINT_REPRESENT;
      }

      write |= static_cast<T>(byte & 0x7f) << shift;  

       
      if ((byte & 0x80) == 0) {
	break;
      }
    }
    return read;
  }

   
 
 
  template<typename InputIt, typename T>
    int read_varint(InputIt &&first, InputIt &&last, T &i) {
    return read_varint<std::numeric_limits<T>::digits>(std::forward<InputIt>(first), std::forward<InputIt>(last), i);
  }
}
