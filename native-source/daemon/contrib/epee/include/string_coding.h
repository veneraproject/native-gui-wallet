                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   


#ifndef _STRING_CODING_H_
#define _STRING_CODING_H_

#include <string>

namespace epee
{
namespace string_encoding
{
	inline std::string convert_to_ansii(const std::wstring& str_from)
	{
		
		std::string res(str_from.begin(), str_from.end());
		return res;
		 
 
 
 
 
 
 
 
 
		
		 
		 
 
 
 
 
 
 
 
 
 
 
	}

	inline std::string convert_to_ansii(const std::string& str_from)
	{
		return str_from;
	}

	inline std::wstring convert_to_unicode(const std::string& str_from)
	{
		std::wstring result;
		std::locale loc;
		for(unsigned int i= 0; i < str_from.size(); ++i)
		{
			result += std::use_facet<std::ctype<wchar_t> >(loc).widen(str_from[i]);
		}
		return result;
		
		 
		 
 
 
 

 
 
 

 
 
 
 
	}
	inline std::wstring convert_to_unicode(const std::wstring& str_from)
	{
		return str_from;
	}

	template<class target_string>
	inline target_string convert_to_t(const std::wstring& str_from);
	
	template<>
	inline std::string convert_to_t<std::string>(const std::wstring& str_from)
	{
		return convert_to_ansii(str_from);
	}

	template<>
	inline std::wstring convert_to_t<std::wstring>(const std::wstring& str_from)
	{
		return str_from;
	}

	template<class target_string>
	inline target_string convert_to_t(const std::string& str_from);

	template<>
	inline std::string convert_to_t<std::string>(const std::string& str_from)
	{
		return str_from;
	}

	template<>
	inline std::wstring convert_to_t<std::wstring>(const std::string& str_from)
	{
		return convert_to_unicode(str_from);
	}

	inline 
	std::string& base64_chars()
	{

		static std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";

		return chars;

	}

	inline
	std::string base64_encode(unsigned char const* bytes_to_encode, size_t in_len) {
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (in_len--) {
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for(i = 0; (i <4) ; i++)
					ret += base64_chars()[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for(j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars()[char_array_4[j]];

			while((i++ < 3))
				ret += '=';

		}

		return ret;

	}

	inline
		std::string base64_encode(const std::string& str) 
	{
		return base64_encode((unsigned char const* )str.data(), str.size());
	}

	inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}


	inline
	std::string base64_decode(std::string const& encoded_string) {
		size_t in_len = encoded_string.size();
		size_t i = 0;
		size_t j = 0;
		size_t in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i ==4) {
				for (i = 0; i <4; i++)
					char_array_4[i] = (unsigned char)base64_chars().find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret += char_array_3[i];
				i = 0;
			}
		}

		if (i) {
			for (j = i; j <4; j++)
				char_array_4[j] = 0;

			for (j = 0; j <4; j++)
				char_array_4[j] = (unsigned char)base64_chars().find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
		}

		return ret;
	}

}
}

#endif  
