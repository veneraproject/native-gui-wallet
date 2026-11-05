#ifndef PORTABLE_BINARY_IARCHIVE_HPP
#define PORTABLE_BINARY_IARCHIVE_HPP

 
#if defined(_MSC_VER)
# pragma once
#endif

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

 
 

                                                             
                                                                      
                                                                          
                                        

 

#include <istream>
#include <boost/version.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/basic_binary_iprimitive.hpp>
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>

#include <boost/archive/portable_binary_archive.hpp>
#include <boost/archive/impl/basic_binary_iprimitive.ipp>

namespace boost { namespace archive {

 
 
 
class portable_binary_iarchive_exception : 
    public boost::archive::archive_exception
{
public:
    enum exception_code {
        incompatible_integer_size
    } m_exception_code ;
    portable_binary_iarchive_exception(exception_code c = incompatible_integer_size ) :
        boost::archive::archive_exception(boost::archive::archive_exception::other_exception),
        m_exception_code(c)
    {}
    virtual const char *what( ) const throw( )
    {
        const char *msg = "programmer error";
        switch(m_exception_code){
        case incompatible_integer_size:
            msg = "integer cannot be represented";
            break;
        default:
            msg = boost::archive::archive_exception::what();
            assert(false);
            break;
        }
        return msg;
    }
};

 
 
 
 
class portable_binary_iarchive :
    public boost::archive::basic_binary_iprimitive<
        portable_binary_iarchive,
        std::istream::char_type, 
        std::istream::traits_type
    >,
    public boost::archive::detail::common_iarchive<
        portable_binary_iarchive
    >
    {
    typedef boost::archive::basic_binary_iprimitive<
        portable_binary_iarchive,
        std::istream::char_type, 
        std::istream::traits_type
    > primitive_base_t;
    typedef boost::archive::detail::common_iarchive<
        portable_binary_iarchive
    > archive_base_t;
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend archive_base_t;
    friend primitive_base_t;  
    friend class boost::archive::detail::interface_iarchive<
        portable_binary_iarchive
    >;
    friend class boost::archive::load_access;
protected:
#endif
    unsigned int m_flags;
    void load_impl(boost::intmax_t & l, char maxsize);

     
    template<class T>
    void load(T & t){
        boost::intmax_t l;
        load_impl(l, sizeof(T));
         
         
        t = T(l);
    }
    void load(boost::serialization::item_version_type & t){
        boost::intmax_t l;
        load_impl(l, sizeof(boost::serialization::item_version_type));
         
        t = boost::serialization::item_version_type(l);
    }
    void load(boost::archive::version_type & t){
        boost::intmax_t l;
        load_impl(l, sizeof(boost::archive::version_type));
         
        t = boost::archive::version_type(l);
    }
    void load(boost::archive::class_id_type & t){
        boost::intmax_t l;
        load_impl(l, sizeof(boost::archive::class_id_type));
         
        t = boost::archive::class_id_type(static_cast<int>(l));
    }
    void load(std::string & t){
        this->primitive_base_t::load(t);
    }
    #ifndef BOOST_NO_STD_WSTRING
    void load(std::wstring & t){
        this->primitive_base_t::load(t);
    }
    #endif
    void load(float & t){
        this->primitive_base_t::load(t);
         
         
    }
    void load(double & t){
        this->primitive_base_t::load(t);
         
         
    }
    void load(char & t){
        this->primitive_base_t::load(t);
    }
    void load(unsigned char & t){
        this->primitive_base_t::load(t);
    }
    typedef boost::archive::detail::common_iarchive<portable_binary_iarchive> 
        detail_common_iarchive;
#if BOOST_VERSION > 105800
    template<class T>
    void load_override(T & t){
        this->detail_common_iarchive::load_override(t);
    }
    void load_override(boost::archive::class_name_type & t);
     
    void load_override(boost::archive::class_id_optional_type &){}
#else
    template<class T>
    void load_override(T & t, int){
        this->detail_common_iarchive::load_override(t, 0);
    }
    void load_override(boost::archive::class_name_type & t, int);
     
    void load_override(boost::archive::class_id_optional_type &, int){}
#endif

    void init(unsigned int flags);
public:
    portable_binary_iarchive(std::istream & is, unsigned flags = 0) :
        primitive_base_t(
            * is.rdbuf(), 
            0 != (flags & boost::archive::no_codecvt)
        ),
        archive_base_t(flags),
        m_flags(0)
    {
        init(flags);
    }

    portable_binary_iarchive(
        std::basic_streambuf<
            std::istream::char_type, 
            std::istream::traits_type
        > & bsb, 
        unsigned int flags
    ) :
        primitive_base_t(
            bsb, 
            0 != (flags & boost::archive::no_codecvt)
        ),
        archive_base_t(flags),
        m_flags(0)
    {
        init(flags);
    }
};

} }

 
#ifdef BOOST_SERIALIZATION_REGISTER_ARCHIVE
    BOOST_SERIALIZATION_REGISTER_ARCHIVE(portable_binary_iarchive)
#endif

 
#define BOOST_ARCHIVE_CUSTOM_IARCHIVE_TYPES portable_binary_iarchive

 
 

                                                           
                                                                      
                                                                          
                                        

 

#include <istream>
#include <string>

#include <boost/predef/other/endian.h>
#include <boost/serialization/throw_exception.hpp>
#include <boost/archive/archive_exception.hpp>

namespace boost { namespace archive {

inline void 
portable_binary_iarchive::load_impl(boost::intmax_t & l, char maxsize){
    signed char size;
    l = 0;
    this->primitive_base_t::load(size);

    if(0 == size){
        return;
    }

    bool negative = (size < 0);
    if(negative)
        size = -size;

    if(size > maxsize)
        boost::serialization::throw_exception(
            portable_binary_iarchive_exception()
        );

    char * cptr = reinterpret_cast<char *>(& l);
    #if BOOST_ENDIAN_BIG_BYTE
        cptr += (sizeof(boost::intmax_t) - size);
    #endif
    this->primitive_base_t::load_binary(cptr, size);

    #if BOOST_ENDIAN_BIG_BYTE
        if((m_flags & endian_little) || (!(m_flags & endian_big)))
    #else
        if(m_flags & endian_big)
    #endif
    reverse_bytes(size, cptr);
    
    if(negative)
        l = -l;
}

#if BOOST_VERSION > 105800
inline void
portable_binary_iarchive::load_override(
    boost::archive::class_name_type & t
){
    std::string cn;
    cn.reserve(BOOST_SERIALIZATION_MAX_KEY_SIZE);
    load_override(cn);
    if(cn.size() > (BOOST_SERIALIZATION_MAX_KEY_SIZE - 1))
        boost::serialization::throw_exception(
            boost::archive::archive_exception(
                boost::archive::archive_exception::invalid_class_name)
            );
    std::memcpy(t, cn.data(), cn.size());
     
    t.t[cn.size()] = '\0';
}
#else
inline void
portable_binary_iarchive::load_override(
    boost::archive::class_name_type & t, int
){
    std::string cn;
    cn.reserve(BOOST_SERIALIZATION_MAX_KEY_SIZE);
    load_override(cn, 0);
    if(cn.size() > (BOOST_SERIALIZATION_MAX_KEY_SIZE - 1))
        boost::serialization::throw_exception(
            boost::archive::archive_exception(
                boost::archive::archive_exception::invalid_class_name)
            );
    std::memcpy(t, cn.data(), cn.size());
     
    t.t[cn.size()] = '\0';
}
#endif

inline void 
portable_binary_iarchive::init(unsigned int flags){
    if(0 == (flags & boost::archive::no_header)){
         
        std::string file_signature;
        * this >> file_signature;
        if(file_signature != boost::archive::BOOST_ARCHIVE_SIGNATURE())
            boost::serialization::throw_exception(
                boost::archive::archive_exception(
                    boost::archive::archive_exception::invalid_signature
                )
            );
         
         
        boost::archive::library_version_type input_library_version;
        * this >> input_library_version;

         
         
 
 
 
 
 
 
 
 
        
        #if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3205))
        this->set_library_version(input_library_version);
         
         
         
         
        boost::archive::detail::basic_iarchive::set_library_version(
            input_library_version
        );
        #endif
    }
    if (!(m_flags & (endian_little | endian_big)))
      m_flags |= endian_little;
    unsigned char x;
    load(x);
    m_flags = x << CHAR_BIT;
}

} }

namespace boost {
namespace archive {

namespace detail {
    template class archive_serializer_map<portable_binary_iarchive>;
}

 
 
 
 
 

}  
}  

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#endif  
