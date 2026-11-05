                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <boost/uuid/uuid.hpp>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "span.h"

namespace net
{
namespace dandelionpp
{
     
    class connection_map
    {
         
        std::vector<boost::uuids::uuid> out_mapping_;  
        std::vector<std::pair<boost::uuids::uuid, std::size_t>> in_mapping_;  
        std::vector<std::size_t> usage_count_;

         
        connection_map(const connection_map&) = default;

    public:
        using value_type = boost::uuids::uuid;
        using size_type = std::vector<boost::uuids::uuid>::size_type;
        using difference_type = std::vector<boost::uuids::uuid>::difference_type;
        using reference = const boost::uuids::uuid&;
        using const_reference = reference;
        using iterator = std::vector<boost::uuids::uuid>::const_iterator;
        using const_iterator = iterator;

         
        explicit connection_map()
          : connection_map(std::vector<boost::uuids::uuid>{}, 0)
        {}

         
        explicit connection_map(std::vector<boost::uuids::uuid> out_connections, std::size_t stems);

        connection_map(connection_map&&) = default;
        ~connection_map() noexcept;
        connection_map& operator=(connection_map&&) = default;
        connection_map& operator=(const connection_map&) = delete;

         
        connection_map clone() const;

         
        const_iterator begin() const noexcept
        {
            return out_mapping_.begin();
        }

         
        const_iterator end() const noexcept
        {
            return out_mapping_.end();
        }

         
 
 

 
 
        bool update(std::vector<boost::uuids::uuid> current);

         
        std::size_t size() const noexcept;

         
        boost::uuids::uuid get_stem(const boost::uuids::uuid& source);
    };
}  
}  
