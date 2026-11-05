                                       
                                              

                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#pragma once

#include <boost/range/iterator_range.hpp>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <lmdb.h>
#include <utility>

#include "span.h"

namespace lmdb
{
    namespace stream
    {
         
 
 
 
        mdb_size_t count(MDB_cursor* cur);

         
 

 
 
 
 

 
 
 
 

 
 
        std::pair<epee::span<const std::uint8_t>, epee::span<const std::uint8_t>>
            get(MDB_cursor& cur, MDB_cursor_op op, std::size_t key, std::size_t value);
    }

     
 

 
 
 

 
 
 
 
 
 
 
    template<typename T, typename F = T, std::size_t offset = 0>
    class value_iterator
    {
        MDB_cursor* cur;
        epee::span<const std::uint8_t> values;
        
        void increment()
        {
            values.remove_prefix(sizeof(T));
            if (values.empty() && cur)
                values = lmdb::stream::get(*cur, MDB_NEXT_DUP, 0, sizeof(T)).second;
        }

    public:
        using value_type = F;
        using reference = value_type;
        using pointer = void;
        using difference_type = std::size_t;
        using iterator_category = std::input_iterator_tag;

         
        value_iterator() noexcept
          : cur(nullptr), values()
        {}

         
 
 
 
 
        value_iterator(MDB_cursor* cur)
          : cur(cur), values()
        {
            if (cur)
                values = lmdb::stream::get(*cur, MDB_GET_CURRENT, 0, sizeof(T)).second;
        }

        value_iterator(value_iterator const&) = default;
        ~value_iterator() = default;
        value_iterator& operator=(value_iterator const&) = default;

         
        bool is_end() const noexcept { return values.empty(); }

         
        bool equal(value_iterator const& rhs) const noexcept
        {
            return
                (values.empty() && rhs.values.empty()) ||
                values.data() == rhs.values.data();
        }

         
        value_iterator& operator++()
        {
            increment();
            return *this;
        }

         
        value_iterator operator++(int)
        {
            value_iterator out{*this};
            increment();
            return out;
        }

         
 
 

 

 
 
 

 
 
        template<typename U, typename G = U, std::size_t uoffset = 0>
        G get_value() const noexcept
        {
            static_assert(std::is_same<U, T>(), "bad MONERO_FIELD usage?");
            static_assert(std::is_pod<U>(), "value type must be pod");
            static_assert(std::is_pod<G>(), "field type must be pod");
            static_assert(sizeof(G) + uoffset <= sizeof(U), "bad field and/or offset");
            assert(sizeof(G) + uoffset <= values.size());
            assert(!is_end());

            G value;
            std::memcpy(std::addressof(value), values.data() + uoffset, sizeof(value));
            return value;
        }

         
        value_type operator*() const noexcept { return get_value<T, F, offset>(); }
    };
 
     
 

 
 
 
    template<typename T, typename D>
    class value_stream
    {
        std::unique_ptr<MDB_cursor, D> cur;
    public:

         
        explicit value_stream(std::unique_ptr<MDB_cursor, D> cur)
          : cur(std::move(cur))
        {}

        value_stream(value_stream&&) = default;
        value_stream(value_stream const&) = delete;
        ~value_stream() = default;
        value_stream& operator=(value_stream&&) = default;
        value_stream& operator=(value_stream const&) = delete;

         
 
 

 
 
        std::unique_ptr<MDB_cursor, D> give_cursor() noexcept
        {
            return {std::move(cur)};
        }

         
 
 

 
 
 
        void reset()
        {
            if (cur)
                lmdb::stream::get(*cur, MDB_FIRST_DUP, 0, 0);
        }

         
 
 
 
        std::size_t count() const
        {
            return lmdb::stream::count(cur.get());
        }

         
 
 
 
 
 
 
 
 

 
 
 
        template<typename U = T, typename F = U, std::size_t offset = 0>
        value_iterator<U, F, offset> make_iterator() const
        {
            static_assert(std::is_same<U, T>(), "was MONERO_FIELD used with wrong type?");
            return {cur.get()};
        }

         
 
 
 
 
 
 

 
 
 
        template<typename U = T, typename F = U, std::size_t offset = 0>
        boost::iterator_range<value_iterator<U, F, offset>> make_range() const
        {
            return {make_iterator<U, F, offset>(), value_iterator<U, F, offset>{}};
        }
    };

    template<typename T, typename F, std::size_t offset>
    inline
    bool operator==(value_iterator<T, F, offset> const& lhs, value_iterator<T, F, offset> const& rhs) noexcept
    {
        return lhs.equal(rhs);
    }

    template<typename T, typename F, std::size_t offset>
    inline
    bool operator!=(value_iterator<T, F, offset> const& lhs, value_iterator<T, F, offset> const& rhs) noexcept
    {
        return !lhs.equal(rhs);
    }
}  

