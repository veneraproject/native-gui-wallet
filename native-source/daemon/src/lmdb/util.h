                                       
                                              

                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#pragma once

#include <cstddef>
#include <cstring>
#include <lmdb.h>
#include <type_traits>
#include <utility>

#include "span.h"

 
 
 
#define MONERO_FIELD(obj, field)                                     \
    obj , decltype(std::declval<obj>().field) , offsetof(obj, field)

 
#define MONERO_SORT_BY(obj, field)                              \
    &::lmdb::less<                                              \
        lmdb::native_type<decltype(std::declval<obj>().field)>, \
        offsetof(obj, field)                                    \
    >

 
#define MONERO_COMPARE(obj, field)           \
    &::lmdb::compare<                        \
        decltype(std::declval<obj>().field), \
        offsetof(obj, field)                 \
    >

namespace lmdb
{
     
    template<typename T>
    struct identity
    {
        using type = T;
    };

     
 
 
 
 
 
 
 
    template<typename T>
    using native_type = typename std::conditional<
        std::is_enum<T>::value, std::underlying_type<T>, identity<T>
    >::type::type;

     
    template<typename T, typename U = typename std::underlying_type<T>::type>
    inline constexpr U to_native(T value) noexcept
    {
        return U(value);
    }

     
    template<typename T>
    inline MDB_val to_val(T&& value) noexcept
    {
         
        static_assert(!std::is_rvalue_reference<T&&>(), "cannot use temporary value");
        void const* const temp = reinterpret_cast<void const*>(std::addressof(value));
        return MDB_val{sizeof(value), const_cast<void*>(temp)};
    }

     
    inline constexpr epee::span<const std::uint8_t> to_byte_span(MDB_val value) noexcept
    {
        return {static_cast<const std::uint8_t*>(value.mv_data), value.mv_size};
    }

     
 

 
 

 
 
    template<typename T, std::size_t offset = 0>
    inline int less(MDB_val const* left, MDB_val const* right) noexcept
    {
        if (!left || !right || left->mv_size < sizeof(T) + offset || right->mv_size < sizeof(T) + offset)
        {
            assert("invalid use of custom comparison" == 0);
            return -1;
        }

        T left_val;
        T right_val;
        std::memcpy(std::addressof(left_val), static_cast<char*>(left->mv_data) + offset, sizeof(T));
        std::memcpy(std::addressof(right_val), static_cast<char*>(right->mv_data) + offset, sizeof(T));
        return left_val < right_val ? -1 : bool(right_val < left_val);
    }

     
 

 
 

 
 
    template<typename T, std::size_t offset = 0>
    inline int compare(MDB_val const* left, MDB_val const* right) noexcept
    {
        static_assert(!epee::has_padding<T>(), "memcmp will not work");
        if (!left || !right || left->mv_size < sizeof(T) + offset || right->mv_size < sizeof(T) + offset)
        {
            assert("invalid use of custom comparison" == 0);
            return -1;
        }
        return std::memcmp(
            static_cast<char*>(left->mv_data) + offset,
            static_cast<char*>(right->mv_data) + offset,
            sizeof(T)
        );
    }
}  
