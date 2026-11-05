#ifndef PORTABLE_BINARY_OARCHIVE_HPP
#define PORTABLE_BINARY_OARCHIVE_HPP

 
#if defined(_MSC_VER)
# pragma once
#endif

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

 
 

                                                           
                                                                      
                                                                          
                                        

 

#include <ostream>
#include <boost/version.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/basic_binary_oprimitive.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>

#include <boost/archive/portable_binary_archive.hpp>
#include <boost/archive/impl/basic_binary_oprimitive.ipp>

namespace boost { namespace archive {

 
 
 
class portable_binary_oarchive_exception : 
    public boost::archive::archive_exception
{
public:
    enum exception_code {
        invalid_flags 
    } m_exception_code ;
    portable_binary_oarchive_exception(exception_code c = invalid_flags ) :
        boost::archive::archive_exception(boost::archive::archive_exception::other_exception),
        m_exception_code(c)
    {}
    virtual const char *what( ) const throw( )
    {
        const char *msg = "programmer error";
        switch(m_exception_code){
        case invalid_flags:
            msg = "cannot be both big and little endian";
            break;
        default:
            msg = boost::archive::archive_exception::what();
            assert(false);
            break;
        }
        return msg;
    }
};

 
 
 
 

class portable_binary_oarchive :
    public boost::archive::basic_binary_oprimitive<
        portable_binary_oarchive,
        std::ostream::char_type, 
        std::ostream::traits_type
    >,
    public boost::archive::detail::common_oarchive<
        portable_binary_oarchive
    >
{
    typedef boost::archive::basic_binary_oprimitive<
        portable_binary_oarchive,
        std::ostream::char_type, 
        std::ostream::traits_type
    > primitive_base_t;
    typedef boost::archive::detail::common_oarchive<
        portable_binary_oarchive
    > archive_base_t;
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend archive_base_t;
    friend primitive_base_t;  
    friend class boost::archive::detail::interface_oarchive<
        portable_binary_oarchive
    >;
    friend class boost::archive::save_access;
protected:
#endif
    unsigned int m_flags;
    void save_impl(const boost::intmax_t l, const char maxsize);
     
     
     
     

     
    template<class T>
    void save(const T & t){
        save_impl(t, sizeof(T));
    }
    void save(const std::string & t){
        this->primitive_base_t::save(t);
    }
    #ifndef BOOST_NO_STD_WSTRING
    void save(const std::wstring & t){
        this->primitive_base_t::save(t);
    }
    #endif
    void save(const float & t){
        this->primitive_base_t::save(t);
         
         
    }
    void save(const double & t){
        this->primitive_base_t::save(t);
         
         
    }
    void save(const char & t){
        this->primitive_base_t::save(t);
    }
    void save(const unsigned char & t){
        this->primitive_base_t::save(t);
    }

     
     
    typedef boost::archive::detail::common_oarchive<portable_binary_oarchive> 
        detail_common_oarchive;
#if BOOST_VERSION > 105800
    template<class T>
    void save_override(T & t){
        this->detail_common_oarchive::save_override(t);
    }
     
    void save_override(const boost::archive::class_name_type & t){
        const std::string s(t);
        * this << s;
    }
     
    void save_override(
        const boost::archive::class_id_optional_type &  
    ){}
#else
    template<class T>
    void save_override(T & t, int){
        this->detail_common_oarchive::save_override(t, 0);
    }
     
    void save_override(const boost::archive::class_name_type & t, int){
        const std::string s(t);
        * this << s;
    }
     
    void save_override(
        const boost::archive::class_id_optional_type &  , int
    ){}
#endif

    void init(unsigned int flags);
public:
    portable_binary_oarchive(std::ostream & os, unsigned flags = endian_little) :
        primitive_base_t(
            * os.rdbuf(),
            0 != (flags & boost::archive::no_codecvt)
        ),
        archive_base_t(flags),
        m_flags(flags & (endian_big | endian_little))
    {
        init(flags);
    }

    portable_binary_oarchive(
        std::basic_streambuf<
            std::ostream::char_type, 
            std::ostream::traits_type
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
    BOOST_SERIALIZATION_REGISTER_ARCHIVE(portable_binary_oarchive)
#endif

 
#define BOOST_ARCHIVE_CUSTOM_OARCHIVE_TYPES portable_binary_oarchive

 
 

                                                             
                                                                      
                                                                          
                                        

 

#include <ostream>
#include <boost/predef/other/endian.h>

namespace boost { namespace archive {

inline void 
portable_binary_oarchive::save_impl(
    const boost::intmax_t l,
    const char maxsize
){
    signed char size = 0;

    if(l == 0){
        this->primitive_base_t::save(size);
        return;
    }

    boost::intmax_t ll;
    bool negative = (l < 0);
    if(negative)
        ll = -l;
    else
        ll = l;

    do{
        ll >>= CHAR_BIT;
        ++size;
    }while(ll != 0);

    this->primitive_base_t::save(
        static_cast<signed char>(negative ? -size : size)
    );

    if(negative)
        ll = -l;
    else
        ll = l;
    char * cptr = reinterpret_cast<char *>(& ll);
    #if BOOST_ENDIAN_BIG_BYTE
        cptr += (sizeof(boost::intmax_t) - size);
        if(m_flags & endian_little)
            reverse_bytes(size, cptr);
    #else
        if(m_flags & endian_big)
            reverse_bytes(size, cptr);
    #endif
    this->primitive_base_t::save_binary(cptr, size);
}

inline void 
portable_binary_oarchive::init(unsigned int flags) {
    if(m_flags == (endian_big | endian_little)){
        boost::serialization::throw_exception(
            portable_binary_oarchive_exception()
        );
    }
    if(0 == (flags & boost::archive::no_header)){
         
        const std::string file_signature(
            boost::archive::BOOST_ARCHIVE_SIGNATURE()
        );
        * this << file_signature;
         
        const boost::archive::library_version_type v{};
         
 
 
 
 
 
        * this << v;
    }
    save(static_cast<unsigned char>(m_flags >> CHAR_BIT));
}

} }

namespace boost {
namespace archive {

namespace detail {
    template class archive_serializer_map<portable_binary_oarchive>;
}

 
 
 
 
 

}  
}  

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#endif  
