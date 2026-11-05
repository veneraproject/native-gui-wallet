                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>

#include "byte_slice.h"
#include "span.h"

namespace epee
{
   

 
 

 
 
 
 
 
 
 
 
 
 
 
  class byte_stream
  {
    byte_buffer buffer_;         
    std::uint8_t* next_write_;   
    const std::uint8_t* end_;    

     
    void overflow(const std::size_t requested);

     
    void check(const std::size_t requested)
    {
      const std::size_t remaining = available();
      if (remaining < requested)
        overflow(requested);
    }

  public:
    using char_type = std::uint8_t;
    using Ch = char_type;

     
    byte_stream() noexcept
      : buffer_(nullptr),
        next_write_(nullptr),
        end_(nullptr)
    {}

    byte_stream(byte_stream&& rhs) noexcept;
    ~byte_stream() noexcept = default;
    byte_stream& operator=(byte_stream&& rhs) noexcept;

    const std::uint8_t* data() const noexcept { return buffer_.get(); }
    std::uint8_t* tellp() const noexcept { return next_write_; }
    std::size_t available() const noexcept { return end_ - next_write_; }
    std::size_t size() const noexcept { return next_write_ - buffer_.get(); }
    std::size_t capacity() const noexcept { return end_ - buffer_.get(); }

     
    void Flush() const noexcept
    {}

     
 
 
 
    void reserve(const std::size_t more)
    {
      check(more);
    }

     
    void clear() noexcept { next_write_ = buffer_.get(); }

     
 
 
    void write(const std::uint8_t* ptr, const std::size_t length)
    {
      check(length);
      std::memcpy(tellp(), ptr, length);
      next_write_ += length;
    }

     
 
 
    void write(const char* ptr, const std::size_t length)
    {
      write(reinterpret_cast<const std::uint8_t*>(ptr), length);
    }

     
 
 
    void write(const epee::span<const std::uint8_t> source)
    {
      write(source.data(), source.size());
    }

     
 
 
    void write(const epee::span<const char> source)
    {
      write(source.data(), source.size());
    }

     
 
 
    void put(const std::uint8_t ch)
    {
      check(1);
      put_unsafe(ch);
    }

     
 
 
    void Put(const std::uint8_t ch)
    {
      put(ch);
    }

     
 
 
 
    void put_unsafe(const std::uint8_t ch) noexcept
    {
      assert(1 <= available());
      *(tellp()) = ch;
      ++next_write_;
    }

     
 
 
    void put_n(const std::uint8_t ch, const std::size_t count)
    {
      check(count);
      std::memset(tellp(), ch, count);
      next_write_ += count;
    }

     
 
 
    void push_back(const std::uint8_t ch)
    {
      put(ch);
    }

     
    byte_buffer take_buffer() noexcept;
  };

   

  inline void PutReserve(byte_stream& dest, const std::size_t length)
  {
    dest.reserve(length);
  }

   

  inline void PutUnsafe(byte_stream& dest, const std::uint8_t ch)
  {
    dest.put_unsafe(ch);
  }

   
  inline void PutN(byte_stream& dest, const std::uint8_t ch, const std::size_t count)
  {
    dest.put_n(ch, count);
  }
}  

