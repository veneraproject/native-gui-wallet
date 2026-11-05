                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   

#include "file_io_utils.h"

#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#ifdef _WIN32
#include <windows.h>
#include "string_tools.h"
#endif

 
 

 
 
 
 

 
 
 

 
 
 
 
 

 
 
 

namespace epee
{
namespace file_io_utils
{
 
	bool is_file_exist(const std::string& path)
	{
		boost::filesystem::path p(path);
		return boost::filesystem::exists(p);
	}


	bool save_string_to_file(const std::string& path_to_file, const std::string& str)
	{
#ifdef _WIN32
                std::wstring wide_path;
                try { wide_path = string_tools::utf8_to_utf16(path_to_file); } catch (...) { return false; }
                HANDLE file_handle = CreateFileW(wide_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (file_handle == INVALID_HANDLE_VALUE)
                    return false;
                DWORD bytes_written;
                DWORD bytes_to_write = (DWORD)str.size();
                BOOL result = WriteFile(file_handle, str.data(), bytes_to_write, &bytes_written, NULL);
                CloseHandle(file_handle);
                if (bytes_written != bytes_to_write)
                    result = FALSE;
                return result;
#else
		try
		{
			std::ofstream fstream;
			fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fstream.open(path_to_file, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
			fstream << str;
			fstream.close();
			return true;
		}

		catch(...)
		{
			return false;
		}
#endif
	}


	bool load_file_to_string(const std::string& path_to_file, std::string& target_str, size_t max_size)
	{
#ifdef _WIN32
                std::wstring wide_path;
                try { wide_path = string_tools::utf8_to_utf16(path_to_file); } catch (...) { return false; }
                HANDLE file_handle = CreateFileW(wide_path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (file_handle == INVALID_HANDLE_VALUE)
                    return false;
                DWORD file_size = GetFileSize(file_handle, NULL);
                if ((file_size == INVALID_FILE_SIZE) || (uint64_t)file_size > (uint64_t)max_size) {
                    CloseHandle(file_handle);
                    return false;
                }
                target_str.resize(file_size);
                DWORD bytes_read;
                BOOL result = ReadFile(file_handle, &target_str[0], file_size, &bytes_read, NULL);
                CloseHandle(file_handle);
                if (bytes_read != file_size)
                    result = FALSE;
                return result;
#else
		try
		{
			std::ifstream fstream;
			fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fstream.open(path_to_file, std::ios_base::binary | std::ios_base::in | std::ios::ate);

			std::ifstream::pos_type file_size = fstream.tellg();
			
			if((uint64_t)file_size > (uint64_t)max_size)  
				return false; 
			size_t file_size_t = static_cast<size_t>(file_size);

			target_str.resize(file_size_t);

			fstream.seekg (0, std::ios::beg);
			fstream.read((char*)target_str.data(), target_str.size());
			fstream.close();
			return true;
		}

		catch(...)
		{
			return false;
		}
#endif
	}


	bool get_file_size(const std::string& path_to_file, uint64_t &size)
	{
#ifdef _WIN32
                std::wstring wide_path;
                try { wide_path = string_tools::utf8_to_utf16(path_to_file); } catch (...) { return false; }
                HANDLE file_handle = CreateFileW(wide_path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (file_handle == INVALID_HANDLE_VALUE)
                    return false;
                LARGE_INTEGER file_size;
                BOOL result = GetFileSizeEx(file_handle, &file_size);
                CloseHandle(file_handle);
                if (result) {
                    size = file_size.QuadPart;
                }
                return size;
#else
		try
		{
			std::ifstream fstream;
			fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fstream.open(path_to_file, std::ios_base::binary | std::ios_base::in | std::ios::ate);
			size = fstream.tellg();
			fstream.close();
			return true;
		}

		catch(...)
		{
			return false;
		}
#endif
	}

}
}
