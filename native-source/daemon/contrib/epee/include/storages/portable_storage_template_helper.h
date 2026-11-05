                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   

#pragma once

#include <string>

#include "byte_slice.h"
#include "parserse_base_utils.h"  
#include "portable_storage.h"
#include "file_io_utils.h"
#include "span.h"

namespace epee
{
  class byte_stream;

  namespace serialization
  {
     
    template<class t_struct>
    bool load_t_from_json(t_struct& out, const std::string& json_buff)
    {
      portable_storage ps;
      bool rs = ps.load_from_json(json_buff);
      if(!rs)
        return false;

      return out.load(ps);
    }
     
    template<class t_struct>
    bool load_t_from_json_file(t_struct& out, const std::string& json_file)
    {
      std::string f_buff;
      if(!file_io_utils::load_file_to_string(json_file, f_buff))
        return false;

      return load_t_from_json(out, f_buff);
    }
     
    template<class t_struct>
    bool store_t_to_json(t_struct& str_in, std::string& json_buff, size_t indent = 0, bool insert_newlines = true)
    {
      portable_storage ps;
      str_in.store(ps);
      ps.dump_as_json(json_buff, indent, insert_newlines);
      return true;
    }
     
    template<class t_struct>
    std::string store_t_to_json(t_struct& str_in, size_t indent = 0, bool insert_newlines = true)
    {
      std::string json_buff;
      store_t_to_json(str_in, json_buff, indent, insert_newlines);
      return json_buff;
    }
     
    template<class t_struct>
    bool store_t_to_json_file(t_struct& str_in, const std::string& fpath)
    {
      std::string json_buff;
      store_t_to_json(str_in, json_buff);
      return file_io_utils::save_string_to_file(fpath, json_buff);
    }
     
    template<class t_struct>
    bool load_t_from_binary(t_struct& out, const epee::span<const uint8_t> binary_buff, const epee::serialization::portable_storage::limits_t *limits = NULL)
    {
      portable_storage ps;
      bool rs = ps.load_from_binary(binary_buff, limits);
      if(!rs)
        return false;

      return out.load(ps);
    }
     
    template<class t_struct>
    bool load_t_from_binary(t_struct& out, const std::string& binary_buff)
    {
      return load_t_from_binary(out, epee::strspan<uint8_t>(binary_buff));
    }
     
    template<class t_struct>
    bool load_t_from_binary_file(t_struct& out, const std::string& binary_file)
    {
      std::string f_buff;
      if(!file_io_utils::load_file_to_string(binary_file, f_buff))
        return false;

      return load_t_from_binary(out, f_buff);
    }
     
    template<class t_struct>
    bool store_t_to_binary(t_struct& str_in, byte_slice& binary_buff, size_t initial_buffer_size = 8192)
    {
      portable_storage ps;
      str_in.store(ps);
      return ps.store_to_binary(binary_buff, initial_buffer_size);
    }
     
    template<class t_struct>
    byte_slice store_t_to_binary(t_struct& str_in, size_t initial_buffer_size = 8192)
    {
      byte_slice binary_buff;
      store_t_to_binary(str_in, binary_buff, initial_buffer_size);
      return binary_buff;
    }
     
    template<class t_struct>
    bool store_t_to_binary(t_struct& str_in, byte_stream& binary_buff)
    {
      portable_storage ps;
      str_in.store(ps);
      return ps.store_to_binary(binary_buff);
    }

  }
}
