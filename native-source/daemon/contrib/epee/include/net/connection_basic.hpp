                                       
 
 
 

                                                                                   
                                                               
                                                                          
                                                                                                                                                             
                                                                                                               

                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 

#ifndef INCLUDED_p2p_connection_basic_hpp
#define INCLUDED_p2p_connection_basic_hpp


#include <string>
#include <atomic>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "byte_slice.h"
#include "net/net_utils_base.h"
#include "net/net_ssl.h"
#include "syncobj.h"

namespace epee
{
namespace net_utils
{

	class connection_basic_shared_state
	{
		ssl_options_t ssl_options_;
	public:
		boost::asio::ssl::context ssl_context;
		std::atomic<long> sock_count;
		std::atomic<long> sock_number;

		connection_basic_shared_state()
		  : ssl_options_(ssl_support_t::e_ssl_support_disabled),
		    ssl_context(boost::asio::ssl::context::tlsv12),
		    sock_count(0),
		    sock_number(0)
		{}

		void configure_ssl(ssl_options_t src)
		{
			ssl_options_ = std::move(src);
			ssl_context = ssl_options_.create_context();
		}

		const ssl_options_t& ssl_options() const noexcept { return ssl_options_; }
	};

   
   
   
   

class connection_basic_pimpl;  

  enum t_connection_type {  
	  e_connection_type_NET = 0,  
	  e_connection_type_RPC = 1,  
	  e_connection_type_P2P = 2   
  };
  
  std::string to_string(t_connection_type type);

class connection_basic {  
		 
		const std::shared_ptr<connection_basic_shared_state> m_state;
	public:

		std::unique_ptr< connection_basic_pimpl > mI;  

		 
    std::atomic<bool> m_want_close_connection;
    std::atomic<bool> m_was_shutdown;
    critical_section m_send_que_lock;
    std::deque<byte_slice> m_send_que;
    volatile bool m_is_multithreaded;
     
    boost::asio::io_service::strand strand_;
     
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    ssl_support_t m_ssl_support;

	public:
		 
		connection_basic(boost::asio::ip::tcp::socket&& socket, std::shared_ptr<connection_basic_shared_state> state, ssl_support_t ssl_support);
		connection_basic(boost::asio::io_service &io_service, std::shared_ptr<connection_basic_shared_state> state, ssl_support_t ssl_support);

		virtual ~connection_basic() noexcept(false);

                 
		connection_basic_shared_state& get_state() noexcept { return *m_state;   }
		connection_basic(boost::asio::io_service& io_service, std::atomic<long> &ref_sock_count, std::atomic<long> &sock_number, ssl_support_t ssl);

		boost::asio::ip::tcp::socket& socket() { return socket_.next_layer(); }
		ssl_support_t get_ssl_support() const { return m_ssl_support; }
		void disable_ssl() { m_ssl_support = epee::net_utils::ssl_support_t::e_ssl_support_disabled; }

		bool handshake(boost::asio::ssl::stream_base::handshake_type type, boost::asio::const_buffer buffer = {})
		{
			 
			return m_state->ssl_options().handshake(strand_.context(), socket_, type, buffer);
		}

		template<typename MutableBufferSequence, typename ReadHandler>
		void async_read_some(const MutableBufferSequence &buffers, ReadHandler &&handler)
		{
			if (m_ssl_support == epee::net_utils::ssl_support_t::e_ssl_support_enabled)
				socket_.async_read_some(buffers, std::forward<ReadHandler>(handler));
			else
				socket().async_read_some(buffers, std::forward<ReadHandler>(handler));
		}

		template<typename ConstBufferSequence, typename WriteHandler>
		void async_write_some(const ConstBufferSequence &buffers, WriteHandler &&handler)
		{
			if (m_ssl_support == epee::net_utils::ssl_support_t::e_ssl_support_enabled)
				socket_.async_write_some(buffers, std::forward<WriteHandler>(handler));
			else
				socket().async_write_some(buffers, std::forward<WriteHandler>(handler));
		}

		template<typename ConstBufferSequence, typename WriteHandler>
		void async_write(const ConstBufferSequence &buffers, WriteHandler &&handler)
		{
			if (m_ssl_support == epee::net_utils::ssl_support_t::e_ssl_support_enabled)
				boost::asio::async_write(socket_, buffers, std::forward<WriteHandler>(handler));
			else
				boost::asio::async_write(socket(), buffers, std::forward<WriteHandler>(handler));
		}

		 
		void do_send_handler_write(const void * ptr , size_t cb);
		void do_send_handler_write_from_queue(const boost::system::error_code& e, size_t cb , int q_len);  

		void logger_handle_net_write(size_t size);  
		void logger_handle_net_read(size_t size);  

		 
		
		static void set_rate_up_limit(uint64_t limit);
		static void set_rate_down_limit(uint64_t limit);
		static uint64_t get_rate_up_limit();
		static uint64_t get_rate_down_limit();

		 
		static void set_tos_flag(int tos);  
		static int get_tos_flag();

		 
		void sleep_before_packet(size_t packet_size, int phase, int q_len);  
		static void save_limit_to_file(int limit);  
		static double get_sleep_time(size_t cb);
};

}  
}  

#endif


