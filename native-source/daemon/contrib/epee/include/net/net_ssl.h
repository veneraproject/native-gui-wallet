                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   



#ifndef _NET_SSL_H
#define _NET_SSL_H

#include <chrono>
#include <functional>
#include <stdint.h>
#include <string>
#include <vector>
#include <boost/utility/string_ref.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/system/error_code.hpp>

#define SSL_FINGERPRINT_SIZE 32

namespace epee
{
namespace net_utils
{
	enum class ssl_support_t: uint8_t {
		e_ssl_support_disabled,
		e_ssl_support_enabled,
		e_ssl_support_autodetect,
	};

  enum class ssl_verification_t : uint8_t
  {
    none = 0,          
    system_ca,         
    user_certificates, 
    user_ca            
  };

  struct ssl_authentication_t
  {
    std::string private_key_path;  
    std::string certificate_path;  

     
    void use_ssl_certificate(boost::asio::ssl::context &ssl_context) const;
  };

   
 
 
 
  class ssl_options_t
  {
     
    std::vector<std::vector<std::uint8_t>> fingerprints_;

  public:
    std::string ca_path;
    ssl_authentication_t auth;
    ssl_support_t support;
    ssl_verification_t verification;

     
    ssl_options_t(ssl_support_t support)
      : fingerprints_(),
        ca_path(),
        auth(),
        support(support),
        verification(support == ssl_support_t::e_ssl_support_disabled ? ssl_verification_t::none : ssl_verification_t::system_ca)
    {}

     
    ssl_options_t(std::vector<std::vector<std::uint8_t>> fingerprints, std::string ca_path);

    ssl_options_t(const ssl_options_t&) = default;
    ssl_options_t(ssl_options_t&&) = default;

    ssl_options_t& operator=(const ssl_options_t&) = default;
    ssl_options_t& operator=(ssl_options_t&&) = default;

     
    explicit operator bool() const noexcept { return support != ssl_support_t::e_ssl_support_disabled; }

     
    bool has_strong_verification(boost::string_ref host) const noexcept;

     
    bool has_fingerprint(boost::asio::ssl::verify_context &ctx) const;

     
    void configure(
      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> &socket,
      boost::asio::ssl::stream_base::handshake_type type,
      const std::string& host = {}) const;
    boost::asio::ssl::context create_context() const;

     
 
 
 

 
 

 
 
 
 
 
 
 
 
 
 

 
 
    bool handshake(
      boost::asio::io_context& io_context,
      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> &socket,
      boost::asio::ssl::stream_base::handshake_type type,
      boost::asio::const_buffer buffer = {},
      const std::string& host = {},
      std::chrono::milliseconds timeout = std::chrono::seconds(15),
      const std::function<bool()>& abort_requested = nullptr) const;
  };

         
	constexpr size_t get_ssl_magic_size() { return 9; }
	bool is_ssl(const unsigned char *data, size_t len);
	bool ssl_support_from_string(ssl_support_t &ssl, boost::string_ref s);

	bool create_ec_ssl_certificate(EVP_PKEY *&pkey, X509 *&cert);
	bool create_rsa_ssl_certificate(EVP_PKEY *&pkey, X509 *&cert);

   
  boost::system::error_code store_ssl_keys(boost::asio::ssl::context& ssl, const boost::filesystem::path& base);
}
}

#endif  
