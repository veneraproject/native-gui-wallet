                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#pragma once

#include <string>
#include <atomic>
#include <boost/optional/optional.hpp>
#include "wipeable_string.h"

namespace tools
{
  class password_container
  {
  public:
    static constexpr const size_t max_password_size = 1024;

     
    password_container() noexcept;

     
    password_container(std::string&& password) noexcept;
    password_container(const epee::wipeable_string& password) noexcept;

     
    static boost::optional<password_container> prompt(bool verify, const char *mesage = "Password", bool hide_input = true);
    static std::atomic<bool> is_prompting;

    password_container(const password_container&) = delete;
    password_container(password_container&& rhs) = default;

     
    ~password_container() noexcept;

    password_container& operator=(const password_container&) = delete;
    password_container& operator=(password_container&&) = default;

    const epee::wipeable_string &password() const noexcept { return m_password; }

  private:
    epee::wipeable_string m_password;
  };

  struct login
  {
    login() = default;

     
 
 
 
 

 
 
 

 
 
 
    static boost::optional<login> parse(std::string&& userpass, bool verify, const std::function<boost::optional<password_container>(bool)> &prompt);

    login(const login&) = delete;
    login(login&&) = default;
    ~login() = default;
    login& operator=(const login&) = delete;
    login& operator=(login&&) = default;

    std::string username;
    password_container password;
  };
}
