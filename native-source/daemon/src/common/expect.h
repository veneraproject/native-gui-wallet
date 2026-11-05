                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <cassert>
#include <system_error>
#include <type_traits>
#include <utility>

#include "common/error.h"

 
#define MONERO_PRECOND(...)                            \
    do                                                 \
    {                                                  \
        if (!( __VA_ARGS__ ))                          \
            return {::common_error::kInvalidArgument}; \
    } while (0)

 
#define MONERO_CHECK(...)                           \
    do                                              \
    {                                               \
        const ::expect<void> result = __VA_ARGS__ ; \
        if (!result)                                \
            return result.error();                  \
    } while (0)

 
 

 
 
#define MONERO_UNWRAP(...)                                        \
    ::detail::expect::unwrap( __VA_ARGS__ , nullptr, __FILE__ , __LINE__ )

 
 
 
#define MONERO_THROW(code, msg) \
    ::detail::expect::throw_( code , msg , __FILE__ , __LINE__ )


template<typename> class expect;

namespace detail
{
     
    template<bool C>
    using enable_if = typename std::enable_if<C>::type;
 
    struct expect
    {
         
        static void throw_(std::error_code ec, const char* msg, const char* file, unsigned line);

         
        template<typename T>
        static T unwrap(::expect<T>&& result, const char* error_msg, const char* file, unsigned line)
        {
            if (!result)
                throw_(result.error(), error_msg, file, line);
            return std::move(*result);
        }

         
        static void unwrap(::expect<void>&& result, const char* error_msg, const char* file, unsigned line);
    };
}

 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 

 
 
template<typename T>
class expect
{
    static_assert(std::is_nothrow_destructible<T>(), "T must have a nothrow destructor");

    template<typename U>
    static constexpr bool is_convertible() noexcept
    {
        return std::is_constructible<T, U>() &&
            std::is_convertible<U, T>();
    }

     
    std::error_code code_;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
     

    T& get() noexcept
    {
        assert(has_value());
        return *reinterpret_cast<T*>(std::addressof(storage_));
    }

    T const& get() const noexcept
    {
        assert(has_value());
        return *reinterpret_cast<T const*>(std::addressof(storage_));
    }

    template<typename U>
    void store(U&& value) noexcept(std::is_nothrow_constructible<T, U>())
    {
        new (std::addressof(storage_)) T{std::forward<U>(value)};
        code_ = std::error_code{};
    }

    void maybe_throw() const
    {
        if (has_error())
            ::detail::expect::throw_(error(), nullptr, nullptr, 0);
    }

public:
    using value_type = T;
    using error_type = std::error_code;

    expect() = delete;

     
 
 
    expect(std::error_code const& code) noexcept
      : code_(code), storage_()
    {
        if (!has_error())
            code_ = ::common_error::kInvalidErrorCode;
    }

     
    expect(T val) noexcept(std::is_nothrow_move_constructible<T>())
      : code_(), storage_()
    {
        store(std::move(val));
    }

    expect(expect const& src) noexcept(std::is_nothrow_copy_constructible<T>())
      : code_(src.error()), storage_()
    {
        if (src.has_value())
            store(src.get());
    }

     
    template<typename U, typename = detail::enable_if<is_convertible<U const&>()>>
    expect(expect<U> const& src) noexcept(std::is_nothrow_constructible<T, U const&>())
      : code_(src.error()), storage_()
    {
        if (src.has_value())
            store(*src);
    }

    expect(expect&& src) noexcept(std::is_nothrow_move_constructible<T>())
      : code_(src.error()), storage_()
    {
        if (src.has_value())
            store(std::move(src.get()));
    }

     
    template<typename U, typename = detail::enable_if<is_convertible<U>()>>
    expect(expect<U>&& src) noexcept(std::is_nothrow_constructible<T, U>())
      : code_(src.error()), storage_()
    {
        if (src.has_value())
            store(std::move(*src));
    }

    ~expect() noexcept
    {
        if (has_value())
            get().~T();
    }

    expect& operator=(expect const& src) noexcept(std::is_nothrow_copy_constructible<T>() && std::is_nothrow_copy_assignable<T>())
    {
        if (this != std::addressof(src))
        {
            if (has_value() && src.has_value())
                get() = src.get();
            else if (has_value())
                get().~T();
            else if (src.has_value())
                store(src.get());
            code_ = src.error();
        }
        return *this;
    }

     
 
    expect& operator=(expect&& src) noexcept(std::is_nothrow_move_constructible<T>() && std::is_nothrow_move_assignable<T>())
    {
        if (this != std::addressof(src))
        {
            if (has_value() && src.has_value())
                get() = std::move(src.get());
            else if (has_value())
                get().~T();
            else if (src.has_value())
                store(std::move(src.get()));
            code_ = src.error();
        }
        return *this;
    }

     
    explicit operator bool() const noexcept { return has_value(); }

     
    bool has_error() const noexcept { return bool(code_); }

     
    bool has_value() const noexcept { return !has_error(); }

     
    std::error_code error() const noexcept { return code_; }

     
    T& value() &
    {
        maybe_throw();
        return get();
    }

     
    T const& value() const &
    {
        maybe_throw();
        return get();
    }

     
 
    T&& value() &&
    {
        maybe_throw();
        return std::move(get());
    }

     
    T* operator->() noexcept { return std::addressof(get()); }
     
    T const* operator->() const noexcept { return std::addressof(get()); }
     
    T& operator*() noexcept { return get(); }
     
    T const& operator*() const noexcept { return get(); }

     
 
 
 
    template<typename U>
    bool equal(expect<U> const& rhs) const noexcept(noexcept(*std::declval<expect<T>>() == *rhs))
    {
        return has_value() && rhs.has_value() ?
            get() == *rhs : error() == rhs.error();
    }

     
    bool equal(std::error_code const& rhs) const noexcept
    {
        return has_error() && error() == rhs;
    }

     
 
 
 
    template<typename U, typename = detail::enable_if<!std::is_constructible<std::error_code, U>::value>>
    bool equal(U const& rhs) const noexcept(noexcept(*std::declval<expect<T>>() == rhs))
    {
        return has_value() && get() == rhs;
    }

     
    bool matches(std::error_condition const& rhs) const noexcept
    {
        return has_error() && error() == rhs;
    }
};

template<>
class expect<void>
{
    std::error_code code_;

public:
    using value_type = void;
    using error_type = std::error_code;

     
    expect() noexcept
      : code_()
    {}

    expect(std::error_code const& code) noexcept
      : code_(code)
    {
        if (!has_error())
            code_ = ::common_error::kInvalidErrorCode;
    }

    expect(expect const&) = default;
    ~expect() = default;
    expect& operator=(expect const&) = default;

     
    explicit operator bool() const noexcept { return !has_error(); }

     
    bool has_error() const noexcept { return bool(code_); }

     
    std::error_code error() const noexcept { return code_; }

     
    bool equal(expect const& rhs) const noexcept
    {
        return error() == rhs.error();
    }

     
    bool equal(std::error_code const& rhs) const noexcept
    {
        return has_error() && error() == rhs;
    }

     
    bool matches(std::error_condition const& rhs) const noexcept
    {
        return has_error() && error() == rhs;
    }
};

 
inline expect<void> success() noexcept { return expect<void>{}; }

template<typename T, typename U>
inline
bool operator==(expect<T> const& lhs, expect<U> const& rhs) noexcept(noexcept(lhs.equal(rhs)))
{
    return lhs.equal(rhs);
}

template<typename T, typename U>
inline
bool operator==(expect<T> const& lhs, U const& rhs) noexcept(noexcept(lhs.equal(rhs)))
{
    return lhs.equal(rhs);
}

template<typename T, typename U>
inline
bool operator==(T const& lhs, expect<U> const& rhs) noexcept(noexcept(rhs.equal(lhs)))
{
    return rhs.equal(lhs);
}

template<typename T, typename U>
inline
bool operator!=(expect<T> const& lhs, expect<U> const& rhs) noexcept(noexcept(lhs.equal(rhs)))
{
    return !lhs.equal(rhs);
}

template<typename T, typename U>
inline
bool operator!=(expect<T> const& lhs, U const& rhs) noexcept(noexcept(lhs.equal(rhs)))
{
    return !lhs.equal(rhs);
}

template<typename T, typename U>
inline
bool operator!=(T const& lhs, expect<U> const& rhs) noexcept(noexcept(rhs.equal(lhs)))
{
    return !rhs.equal(lhs);
}

namespace detail
{
    inline void expect::unwrap(::expect<void>&& result, const char* error_msg, const char* file, unsigned line)
    {
        if (!result)
            throw_(result.error(), error_msg, file, line);
    }
}

