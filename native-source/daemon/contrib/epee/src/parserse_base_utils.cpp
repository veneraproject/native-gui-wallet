                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   

#include "storages/parserse_base_utils.h"

#include "misc_log_ex.h"
#include <boost/utility/string_ref.hpp>
#include <algorithm>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "serialization"

namespace epee 
{
namespace misc_utils
{
  namespace parse
  {
    std::string transform_to_escape_sequence(const std::string& src)
    {
      static const char escaped[] = "\b\f\n\r\t\v\"\\/";
      std::string::const_iterator it = std::find_first_of(src.begin(), src.end(), escaped, escaped + sizeof(escaped));
      if (it == src.end())
        return src;

      std::string res;
      res.reserve(2 * src.size());
      res.assign(src.begin(), it);
      for(; it!=src.end(); ++it)
      {
        switch(*it)
        {
        case '\b':   
          res+="\\b"; break;
        case '\f':   
          res+="\\f"; break;
        case '\n':   
          res+="\\n"; break;
        case '\r':   
          res+="\\r"; break;
        case '\t':   
          res+="\\t"; break;
        case '\v':   
          res+="\\v"; break;
         
         
        case '"':   
          res+="\\\""; break;
        case '\\':   
          res+="\\\\"; break;
        case '/':   
          res+="\\/"; break;
        default:
          res.push_back(*it);
        }
      }
      return res;
    }
     
 
 
 
 
 
 
 
 
 
 

 
      void match_string2(std::string::const_iterator& star_end_string, std::string::const_iterator buf_end, std::string& val)
      {
        bool escape_mode = false;
        std::string::const_iterator it = star_end_string;
        ++it;
        std::string::const_iterator fi = it;
        while (fi != buf_end && ((lut[(uint8_t)*fi] & 32)) == 0)
          ++fi;
        val.assign(it, fi);
        it = fi;
        for(;it != buf_end;++it)
        {
          if(escape_mode )
          {
            switch(*it)
            {
            case 'b':   
              val.push_back(0x08);break;
            case 'f':   
              val.push_back(0x0C);break;
            case 'n':   
              val.push_back('\n');break;
            case 'r':   
              val.push_back('\r');break;
            case 't':   
              val.push_back('\t');break;
            case 'v':   
              val.push_back('\v');break;
            case '\'':   
              val.push_back('\'');break;
            case '"':   
              val.push_back('"');break;
            case '\\':   
              val.push_back('\\');break;
            case '/':   
              val.push_back('/');break;
            case 'u':   
              if (buf_end - it < 4)
              {
                ASSERT_MES_AND_THROW("Invalid Unicode escape sequence");
              }
              else
              {
                uint32_t dst = 0;
                for (int i = 0; i < 4; ++i)
                {
                  const unsigned char tmp = isx[(unsigned char)*++it];
                  CHECK_AND_ASSERT_THROW_MES(tmp != 0xff, "Bad Unicode encoding");
                  dst = dst << 4 | tmp;
                }
                 
                if (dst <= 0x7f)
                {
                  val.push_back(dst);
                }
                else if (dst <= 0x7ff)
                {
                  val.push_back(0xc0 | (dst >> 6));
                  val.push_back(0x80 | (dst & 0x3f));
                }
                else if (dst <= 0xffff)
                {
                  val.push_back(0xe0 | (dst >> 12));
                  val.push_back(0x80 | ((dst >> 6) & 0x3f));
                  val.push_back(0x80 | (dst & 0x3f));
                }
                else
                {
                  ASSERT_MES_AND_THROW("Unicode code point is out or range");
                }
              }
              break;
            default:
              val.push_back(*it);
              LOG_PRINT_L0("Unknown escape sequence :\"\\" << *it << "\"");
            }
            escape_mode = false;
          }else if(*it == '"')
          {
            star_end_string = it;
            return;
          }else if(*it == '\\')
          {
            escape_mode = true;
          }          
          else
          {
            val.push_back(*it); 
          }
        }
        ASSERT_MES_AND_THROW("Failed to match string in json entry: " << std::string(star_end_string, buf_end));
      }
      void match_number2(std::string::const_iterator& star_end_string, std::string::const_iterator buf_end, boost::string_ref& val, bool& is_float_val, bool& is_signed_val)
      {
        val.clear();
        uint8_t float_flag = 0;
        is_signed_val = false;
        size_t chars = 0;
        std::string::const_iterator it = star_end_string;
        if (it != buf_end && *it == '-')
        {
          is_signed_val = true;
          ++chars;
          ++it;
        }
        for(;it != buf_end;++it)
        {
          const uint8_t flags = lut[(uint8_t)*it];
          if (flags & 16)
          {
            float_flag |= flags;
            ++chars;
          }
          else
          {
            val = boost::string_ref(&*star_end_string, chars);
            if(val.size())
            {
              star_end_string = --it;
              is_float_val = !!(float_flag & 2);
              return;
            }
            else 
              ASSERT_MES_AND_THROW("wrong number in json entry: " << std::string(star_end_string, buf_end));
          }
        }
        ASSERT_MES_AND_THROW("wrong number in json entry: " << std::string(star_end_string, buf_end));
      }
      void match_word2(std::string::const_iterator& star_end_string, std::string::const_iterator buf_end, boost::string_ref& val)
      {
        val.clear();

        for(std::string::const_iterator it = star_end_string;it != buf_end;++it)
        {
          if (!(lut[(uint8_t)*it] & 4))
          {
            val = boost::string_ref(&*star_end_string, std::distance(star_end_string, it));
            if(val.size())
            {
              star_end_string = --it;
              return;
            }else 
              ASSERT_MES_AND_THROW("failed to match word number in json entry: " << std::string(star_end_string, buf_end));
          }
        }
        ASSERT_MES_AND_THROW("failed to match word number in json entry: " << std::string(star_end_string, buf_end));
      }
  }
}
}
