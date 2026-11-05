                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "span.h"

namespace epee
{
  struct byte_slice_data;
  class byte_stream;

  struct release_byte_slice
  {
     
    static void call(void*, void* ptr) noexcept;
    void operator()(byte_slice_data* ptr) const noexcept
    {
      call(nullptr, ptr);
    }
  };

   
  struct release_byte_buffer
  {
    void operator()(std::uint8_t* buf) const noexcept;
  };

   
 
 

 
 
 
 
  class byte_slice
  {
     
 
 
    std::unique_ptr<byte_slice_data, release_byte_slice> storage_;
    span<const std::uint8_t> portion_;  

     
    byte_slice(byte_slice_data* storage, span<const std::uint8_t> portion) noexcept;

    struct adapt_buffer{};

    template<typename T>
    explicit byte_slice(const adapt_buffer, T&& buffer);

  public:
    using value_type = std::uint8_t;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const std::uint8_t*;
    using const_pointer = const std::uint8_t*;
    using reference = std::uint8_t;
    using const_reference = std::uint8_t;
    using iterator = pointer;
    using const_iterator = const_pointer;

     
    byte_slice() noexcept
      : storage_(nullptr), portion_()
    {}

     
    byte_slice(std::nullptr_t) noexcept
      : byte_slice()
    {}

     
    explicit byte_slice(std::initializer_list<span<const std::uint8_t>> sources);

     
    explicit byte_slice(std::vector<std::uint8_t>&& buffer);

     
    explicit byte_slice(std::string&& buffer);

     
    explicit byte_slice(byte_stream&& stream, bool shrink = true);

    byte_slice(byte_slice&& source) noexcept;
    ~byte_slice() noexcept = default;

     
    byte_slice& operator=(byte_slice&&) noexcept;

     
    byte_slice clone() const noexcept { return {storage_.get(), portion_}; }

    iterator begin() const noexcept { return portion_.begin(); }
    const_iterator cbegin() const noexcept { return portion_.begin(); }

    iterator end() const noexcept { return portion_.end(); }
    const_iterator cend() const noexcept { return portion_.end(); }

    bool empty() const noexcept { return storage_ == nullptr; }
    const std::uint8_t* data() const noexcept { return portion_.data(); }
    std::size_t size() const noexcept { return portion_.size(); }

     

 
 
 
 
    std::size_t remove_prefix(std::size_t max_bytes) noexcept;

     

 
 
 
 
    byte_slice take_slice(std::size_t max_bytes) noexcept;

     

 
 
 
    byte_slice get_slice(std::size_t begin, std::size_t end) const;

     
    std::unique_ptr<byte_slice_data, release_byte_slice> take_buffer() noexcept;
  };

   
  using byte_buffer = std::unique_ptr<std::uint8_t, release_byte_buffer>;

   
 
  byte_buffer byte_buffer_resize(byte_buffer buf, std::size_t length) noexcept;

   

 
 
 
  byte_buffer byte_buffer_increase(byte_buffer buf, std::size_t current, std::size_t more);
}  

