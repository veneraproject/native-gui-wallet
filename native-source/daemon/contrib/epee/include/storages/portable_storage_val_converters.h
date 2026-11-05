                                                                    
                       
  
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
  
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
  



#pragma once

#include <boost/regex.hpp>

#include "misc_language.h"
#include "portable_storage_base.h"
#include "parserse_base_utils.h"
#include "warnings.h"
#include "misc_log_ex.h"

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <typeinfo>
#include <iomanip>

namespace epee
{
  namespace serialization
  {
#define ASSERT_AND_THROW_WRONG_CONVERSION() ASSERT_MES_AND_THROW("WRONG DATA CONVERSION: from type=" << typeid(from).name() << " to type " << typeid(to).name())

    template<typename from_type, typename to_type>
    void convert_int_to_uint(const from_type& from, to_type& to)
    {
PUSH_WARNINGS
DISABLE_VS_WARNINGS(4018)
      CHECK_AND_ASSERT_THROW_MES(from >=0, "unexpected int value with signed storage value less than 0, and unsigned receiver value");
DISABLE_GCC_AND_CLANG_WARNING(sign-compare)
      CHECK_AND_ASSERT_THROW_MES(from <= std::numeric_limits<to_type>::max(), "int value overhead: try to set value " << from << " to type " << typeid(to_type).name() << " with max possible value = " << std::numeric_limits<to_type>::max());
      to = static_cast<to_type>(from);
POP_WARNINGS
    }
    template<typename from_type, typename to_type>
    void convert_int_to_int(const from_type& from, to_type& to)
    {
      CHECK_AND_ASSERT_THROW_MES(from >= boost::numeric::bounds<to_type>::lowest(), "int value overhead: try to set value " << from << " to type " << typeid(to_type).name() << " with lowest possible value = " << boost::numeric::bounds<to_type>::lowest());
PUSH_WARNINGS
DISABLE_CLANG_WARNING(tautological-constant-out-of-range-compare)
      CHECK_AND_ASSERT_THROW_MES(from <= std::numeric_limits<to_type>::max(), "int value overhead: try to set value " << from << " to type " << typeid(to_type).name() << " with max possible value = " << std::numeric_limits<to_type>::max());
POP_WARNINGS
      to = static_cast<to_type>(from);
    }
    template<typename from_type, typename to_type>
    void convert_uint_to_any_int(const from_type& from, to_type& to)
    {
PUSH_WARNINGS
DISABLE_VS_WARNINGS(4018)
DISABLE_CLANG_WARNING(tautological-constant-out-of-range-compare)
        CHECK_AND_ASSERT_THROW_MES(from <= std::numeric_limits<to_type>::max(), "uint value overhead: try to set value " << from << " to type " << typeid(to_type).name() << " with max possible value = " << std::numeric_limits<to_type>::max());
      to = static_cast<to_type>(from);
POP_WARNINGS
    }

    template<typename from_type, typename to_type, bool, bool>  
    struct convert_to_signed_unsigned;

    template<typename from_type, typename to_type>
    struct convert_to_signed_unsigned<from_type, to_type, true, true>
    {
      static void convert(const from_type& from, to_type& to)
      { 
        convert_int_to_int(from, to);
      }
    };

    template<typename from_type, typename to_type>
    struct convert_to_signed_unsigned<from_type, to_type, true, false>
    {
      static void convert(const from_type& from, to_type& to)
      { 
        convert_int_to_uint(from, to);
      }
    };

    template<typename from_type, typename to_type>
    struct convert_to_signed_unsigned<from_type, to_type, false, true>
    {
      static void convert(const from_type& from, to_type& to)
      { 
        convert_uint_to_any_int(from, to);
      }
    };

    template<typename from_type, typename to_type>
    struct convert_to_signed_unsigned<from_type, to_type, false, false>
    {
      static void convert(const from_type& from, to_type& to)
      {
         
        convert_uint_to_any_int(from, to);
      }
    };

    template<typename from_type, typename to_type, bool>
    struct convert_to_integral;

    template<typename from_type, typename to_type>
    struct convert_to_integral<from_type, to_type, true>
    {
      static void convert(const from_type& from, to_type& to)
      {
        convert_to_signed_unsigned<from_type, to_type, std::is_signed<from_type>::value, std::is_signed<to_type>::value>::convert(from, to);
      }
    };

    template<typename from_type, typename to_type>
    struct convert_to_integral<from_type, to_type, false>
    {
      static void convert(const from_type& from, to_type& to)
      {
        ASSERT_AND_THROW_WRONG_CONVERSION();
      }
    };

     
     
     
    template<>
    struct convert_to_integral<std::string, uint64_t, false>
    {
      static void convert(const std::string& from, uint64_t& to)
      {
        MTRACE("Converting std::string to uint64_t. Source: " << from);
         
        if(std::all_of(from.begin(), from.end(), epee::misc_utils::parse::isdigit))
          to = boost::lexical_cast<uint64_t>(from);
         
        else if (boost::regex_match (from, boost::regex("\\d{4}-[01]\\d-[0-3]\\dT[0-2]\\d:[0-5]\\d:[0-5]\\dZ")))
        {
           
#ifdef HAVE_STRPTIME
          struct tm tm;
          if (strptime(from.c_str(), "%Y-%m-%dT%H:%M:%S", &tm))
#else
          std::tm tm = {};
          std::istringstream ss(from);
          if (ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S"))
#endif
            to = std::mktime(&tm);
        } else
          ASSERT_AND_THROW_WRONG_CONVERSION();
      }
    };

    template<class from_type, class to_type>
    struct is_convertable: std::integral_constant<bool,
                            std::is_integral<to_type>::value &&
                            std::is_integral<from_type>::value &&
                            !std::is_same<from_type, bool>::value &&
                            !std::is_same<to_type, bool>::value > {};

    template<typename from_type, typename to_type, bool>
    struct convert_to_same;

    template<typename from_type, typename to_type>
    struct convert_to_same<from_type, to_type, true>
    {
      static void convert(const from_type& from, to_type& to)
      {
        to = from;
      }
    };

    template<typename from_type, typename to_type>
    struct convert_to_same<from_type, to_type, false>
    {
      static void convert(const from_type& from, to_type& to)
      {
        convert_to_integral<from_type, to_type, is_convertable<from_type, to_type>::value>::convert(from, to); 
      }
    };


    template<class from_type, class to_type>
    void convert_t(const from_type& from, to_type& to)
    {
      convert_to_same<from_type, to_type, std::is_same<to_type, from_type>::value>::convert(from, to);
    }
  }
}
