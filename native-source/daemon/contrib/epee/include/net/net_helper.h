                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   




#pragma once

 
 
#include <atomic>
#include <memory>
#include <string>
#include <boost/version.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread/future.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/system/error_code.hpp>
#include <boost/utility/string_ref.hpp>
#include <functional>
#ifdef BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR
#include <boost/asio/posix/stream_descriptor.hpp>
#include <cerrno>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#endif
#include "net/net_utils_base.h"
#include "net/net_ssl.h"
#include "misc_language.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "net"

#ifndef MAKE_IP
#define MAKE_IP( a1, a2, a3, a4 )	(a1|(a2<<8)|(a3<<16)|(a4<<24))
#endif


namespace epee
{
namespace net_utils
{
	struct direct_connect
	{
		boost::unique_future<boost::asio::ip::tcp::socket>
			operator()(const std::string& addr, const std::string& port, boost::asio::steady_timer&) const;
	};


  class blocked_mode_client
	{
		enum try_connect_result_t
		{
			CONNECT_SUCCESS,
			CONNECT_FAILURE,
			CONNECT_NO_SSL,
		};

		
		
				struct handler_obj
				{
					handler_obj(boost::system::error_code& error,	size_t& bytes_transferred):ref_error(error), ref_bytes_transferred(bytes_transferred)
					{}
					handler_obj(const handler_obj& other_obj):ref_error(other_obj.ref_error), ref_bytes_transferred(other_obj.ref_bytes_transferred)
					{}

					boost::system::error_code& ref_error;
					size_t& ref_bytes_transferred;

					void operator()(const boost::system::error_code& error,  
						std::size_t bytes_transferred            
						)
					{
						ref_error = error;
						ref_bytes_transferred = bytes_transferred;
					}
				};

	public:
		inline
			blocked_mode_client() :
				m_io_service(),
				m_ctx(boost::asio::ssl::context::tlsv12),
				m_ssl_socket(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(m_io_service, m_ctx)),
				m_connector(direct_connect{}),
				m_ssl_options(epee::net_utils::ssl_support_t::e_ssl_support_autodetect),
				m_initialized(true),
				m_connected(false),
				m_deadline(m_io_service, std::chrono::steady_clock::time_point::max()),
#ifdef BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR
				m_wake_reader(m_io_service),
				m_wake_writer(-1),
#endif
				m_aborted(false),
				m_bytes_sent(0),
				m_bytes_received(0)
		{
			check_deadline();
#ifdef BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR
			 
			 
			int fds[2];
			if (0 != ::pipe(fds))
				throw std::runtime_error("failed to create shutdown wake pipe");
			bool flags_set = true;
			for (const int fd : fds)
			{
				if (-1 == ::fcntl(fd, F_SETFL, O_NONBLOCK) || -1 == ::fcntl(fd, F_SETFD, FD_CLOEXEC))
					flags_set = false;
			}
			boost::system::error_code ec;
			if (flags_set)
				m_wake_reader.assign(fds[0], ec);
			if (!flags_set || ec)
			{
				::close(fds[0]);
				::close(fds[1]);
				throw std::runtime_error("failed to set up shutdown wake pipe");
			}
			m_wake_writer = fds[1];
			wait_wake();
#endif
		}

		 
 
 
 

 
 
 

 
 

 
 
		using connect_func = boost::unique_future<boost::asio::ip::tcp::socket>(const std::string&, const std::string&, boost::asio::steady_timer&);

		inline
			~blocked_mode_client()
		{
			 
			try { disconnect(); }
			catch(...) {   }
#ifdef BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR
			if (m_wake_writer != -1)
				::close(m_wake_writer);  
#endif
		}

		inline void set_ssl(ssl_options_t ssl_options)
		{
			if (ssl_options)
				m_ctx = ssl_options.create_context();
			else
				m_ctx = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12);
			m_ssl_options = std::move(ssl_options);
		}

    inline
      bool connect(const std::string& addr, int port, std::chrono::milliseconds timeout)
    {
      return connect(addr, std::to_string(port), timeout);
    }

    inline
			try_connect_result_t try_connect(const std::string& addr, const std::string& port, std::chrono::milliseconds timeout)
		{
				const auto deadline = std::chrono::steady_clock::now() + timeout;
				m_deadline.expires_after(timeout);
				boost::unique_future<boost::asio::ip::tcp::socket> connection = m_connector(addr, port, m_deadline);
				for (;;)
				{
					 
					 
					if (shutdown_requested() || std::chrono::steady_clock::now() >= deadline)
						return CONNECT_FAILURE;

					m_io_service.restart();
					m_io_service.run_one();

					if (connection.is_ready())
						break;
				}

				m_ssl_socket->next_layer() = connection.get();
				m_deadline.cancel();
				if (shutdown_requested())
				{
					boost::system::error_code ignored_ec;
					m_ssl_socket->next_layer().close(ignored_ec);
					return CONNECT_FAILURE;  
				}
				if (m_ssl_socket->next_layer().is_open())
				{
					m_connected = true;
					m_deadline.expires_at(std::chrono::steady_clock::time_point::max());
					 
					if (m_ssl_options.support == epee::net_utils::ssl_support_t::e_ssl_support_enabled || m_ssl_options.support == epee::net_utils::ssl_support_t::e_ssl_support_autodetect)
					{
						if (!m_ssl_options.handshake(m_io_service, *m_ssl_socket, boost::asio::ssl::stream_base::client, {}, addr, timeout, [this] { return shutdown_requested(); }))
						{
							if (shutdown_requested())
							{
								m_connected = false;
								return CONNECT_FAILURE;  
							}
							if (m_ssl_options.support == epee::net_utils::ssl_support_t::e_ssl_support_autodetect)
							{
								boost::system::error_code ignored_ec;
								m_ssl_socket->next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
								m_ssl_socket->next_layer().close();
								m_connected = false;
								return CONNECT_NO_SSL;
							}
							else
							{
								MWARNING("Failed to establish SSL connection");
								m_connected = false;
								return CONNECT_FAILURE;
							}
						}
					}
					 
					 
					if (shutdown_requested())
					{
						m_connected = false;
						boost::system::error_code ignored_ec;
						m_ssl_socket->next_layer().close(ignored_ec);
						return CONNECT_FAILURE;
					}
					return CONNECT_SUCCESS;
				}else
				{
					MWARNING("Some problems at connect, expected open socket");
					return CONNECT_FAILURE;
				}

		}

    inline
			bool connect(const std::string& addr, const std::string& port, std::chrono::milliseconds timeout)
		{
			if (shutdown_requested())
				return false;  
			m_connected = false;
			try
			{
				m_ssl_socket->next_layer().close();
				m_pending_read.reset();

				 
				 
				m_ssl_socket.reset(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(m_io_service, m_ctx));

				 

				try_connect_result_t try_connect_result = try_connect(addr, port, timeout);
				if (try_connect_result == CONNECT_FAILURE)
					return false;
				if (m_ssl_options.support == epee::net_utils::ssl_support_t::e_ssl_support_autodetect)
				{
					if (try_connect_result == CONNECT_NO_SSL)
					{
						MERROR("SSL handshake failed on an autodetect connection, reconnecting without SSL");
						m_ssl_options.support = epee::net_utils::ssl_support_t::e_ssl_support_disabled;
						if (try_connect(addr, port, timeout) != CONNECT_SUCCESS)
							return false;
					}
				}
			}
			catch(const boost::system::system_error& er)
			{
				MDEBUG("Some problems at connect, message: " << er.what());
				return false;
			}
			catch(...)
			{
				MDEBUG("Some fatal problems.");
				return false;
			}

			return true;
		}
		 
		void set_connector(std::function<connect_func> connector)
		{
			m_connector = std::move(connector);
		}

		inline 
		bool disconnect()
		{
			m_connected = false;
			try
			{	
				if(!shutdown_requested() && m_ssl_socket->next_layer().is_open())
				{
					if(m_ssl_options)
						shutdown_ssl();
					 
					boost::system::error_code ignored_ec;
					m_ssl_socket->next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
					 
					m_ssl_socket->next_layer().close();
				}
			}
			catch(const boost::system::system_error&  )
			{
				 
				return false;
			}
			catch(...)
			{
				 
				return false;
			}
			return true;
		}


		inline 
		bool send(const boost::string_ref buff, std::chrono::milliseconds timeout)
		{
			try
			{
				m_deadline.expires_after(timeout);

				 
				 
				 
				 
				 
				boost::system::error_code ec = boost::asio::error::would_block;

				 
				 
				 
				 
				async_write(buff.data(), buff.size(), ec);

				 
				while (ec == boost::asio::error::would_block && !shutdown_requested())
				{
					m_io_service.restart();
					m_io_service.run_one(); 
				}

				 
				 
				if (ec == boost::asio::error::would_block)
				{
					boost::system::error_code ignored_ec;
					m_ssl_socket->next_layer().close(ignored_ec);
					while (ec == boost::asio::error::would_block)
					{
						m_io_service.restart();
						m_io_service.run_one();
					}
				}

				if (ec)
				{
					LOG_PRINT_L3("Problems at write: " << ec.message());
          m_connected = false;
					return false;
				}else
				{
					m_deadline.expires_at(std::chrono::steady_clock::time_point::max());
					m_bytes_sent += buff.size();
				}
			}

			catch(const boost::system::system_error& er)
			{
				LOG_ERROR("Some problems at connect, message: " << er.what());
				return false;
			}
			catch(...)
			{
				LOG_ERROR("Some fatal problems.");
				return false;
			}

			return true;
		}

		inline 
			bool send(const void* data, size_t sz)
		{
			try
			{
				 
 

 
 
 
 
 
 

 
 
 
 
 

 
 
 
 
 
 
				boost::system::error_code ec;

				size_t writen = write(data, sz, ec);

				if (!writen || ec)
				{
					LOG_PRINT_L3("Problems at write: " << ec.message());
          m_connected = false;
					return false;
				}else
				{
					m_deadline.expires_at(std::chrono::steady_clock::time_point::max());
					m_bytes_sent += sz;
				}
			}

			catch(const boost::system::system_error& er)
			{
				LOG_ERROR("Some problems at send, message: " << er.what());
        m_connected = false;
				return false;
			}
			catch(...)
			{
				LOG_ERROR("Some fatal problems.");
				return false;
			}

			return true;
		}

		bool is_connected(bool *ssl = NULL)
		{
			if (!m_connected || !m_ssl_socket->next_layer().is_open())
				return false;

			boost::system::error_code ec;
			if (m_ssl_options)
			{
				 
				if (!m_pending_read)
				{
					const auto pending = std::make_shared<pending_read>();
					 
					const auto socket = m_ssl_socket;
					socket->async_read_some(boost::asio::buffer(&pending->byte, 1),
						[socket, pending](const boost::system::error_code& error, size_t) {
							pending->error = error;
							if (pending->handler)
							{
								auto handler = std::move(pending->handler);
								handler(error, pending->byte);
							}
						});
					m_pending_read = pending;
				}
				m_io_service.restart();
				m_io_service.poll();
				ec = m_pending_read->error;
			}
			else
			{
				 
				auto& socket = m_ssl_socket->next_layer();
				const bool non_blocking = socket.non_blocking();
				socket.non_blocking(true, ec);
				if (!ec)
				{
					char byte;
					socket.receive(boost::asio::buffer(&byte, 1), boost::asio::ip::tcp::socket::message_peek, ec);
					boost::system::error_code restore_error;
					socket.non_blocking(non_blocking, restore_error);
					if (restore_error)
					{
						m_connected = false;
						return false;
					}
				}
			}
			if (ec && ec != boost::asio::error::would_block &&
				ec != boost::asio::error::try_again && ec != boost::asio::error::interrupted)
			{
				MDEBUG("Peer closed idle connection, marking disconnected: " << ec.message());
				m_connected = false;
				return false;
			}

			if (ssl)
				*ssl = m_ssl_options.support != ssl_support_t::e_ssl_support_disabled;
			return m_connected;
		}

		inline 
		bool recv(std::string& buff, std::chrono::milliseconds timeout)
		{
			try
			{
				 
				 
				 
				m_deadline.expires_after(timeout);

				 
				 
				 
				 
				 
				 

				 
				 
				 
				 

				boost::system::error_code ec = boost::asio::error::would_block;
				size_t bytes_transfered = 0;
			
				handler_obj hndlr(ec, bytes_transfered);

				static const size_t max_size = 16384;
				buff.resize(max_size);
				
				async_read(&buff[0], max_size, boost::asio::transfer_at_least(1), hndlr);

				 
				while (ec == boost::asio::error::would_block && !shutdown_requested())
				{
					m_io_service.restart();
					m_io_service.run_one(); 
				}

				 
				 
				if (ec == boost::asio::error::would_block)
				{
					boost::system::error_code ignored_ec;
					m_ssl_socket->next_layer().close(ignored_ec);
					while (ec == boost::asio::error::would_block)
					{
						m_io_service.restart();
						m_io_service.run_one();
					}
				}

				if (ec)
				{
					m_connected = false;
                    MTRACE("READ ENDS: Connection err_code " << ec.value());
                    if(ec == boost::asio::error::eof)
                    {
                      MTRACE("Connection err_code eof.");
                       
                      buff.clear();
                      return true;
                    }

					MDEBUG("Problems at read: " << ec.message());
					return false;
				}else
				{
                    MTRACE("READ ENDS: Success. bytes_tr: " << bytes_transfered);
					m_deadline.expires_at(std::chrono::steady_clock::time_point::max());
				}

				 
 

				m_bytes_received += bytes_transfered;
				buff.resize(bytes_transfered);
				return true;
			}

			catch(const boost::system::system_error& er)
			{
				LOG_ERROR("Some problems at read, message: " << er.what());
        m_connected = false;
				return false;
			}
			catch(...)
			{
				LOG_ERROR("Some fatal problems at read.");
				return false;
			}



			return false;

		}

		inline bool recv_n(std::string& buff, int64_t sz, std::chrono::milliseconds timeout)
		{

			try
			{
				 
				 
				 
				m_deadline.expires_after(timeout);

				 
				 
				 
				 
				 
				 

				 
				 
				 
				 

				buff.resize(static_cast<size_t>(sz));
				boost::system::error_code ec = boost::asio::error::would_block;
				size_t bytes_transfered = 0;

				
				handler_obj hndlr(ec, bytes_transfered);
				async_read((char*)buff.data(), buff.size(), boost::asio::transfer_at_least(buff.size()), hndlr);
				
				 
				while (ec == boost::asio::error::would_block && !shutdown_requested())
				{
					m_io_service.run_one(); 
				}

				 
				 
				if (ec == boost::asio::error::would_block)
				{
					boost::system::error_code ignored_ec;
					m_ssl_socket->next_layer().close(ignored_ec);
					while (ec == boost::asio::error::would_block)
					{
						m_io_service.restart();
						m_io_service.run_one();
					}
				}

				if (ec)
				{
					LOG_PRINT_L3("Problems at read: " << ec.message());
          m_connected = false;
					return false;
				}else
				{
					m_deadline.expires_at(std::chrono::steady_clock::time_point::max());
				}

				m_bytes_received += bytes_transfered;
				if(bytes_transfered != buff.size())
				{
					LOG_ERROR("Transferred mismatch with transfer_at_least value: m_bytes_transferred=" << bytes_transfered << " at_least value=" << buff.size());
					return false;
				}

				return true;
			}

			catch(const boost::system::system_error& er)
			{
				LOG_ERROR("Some problems at read, message: " << er.what());
        m_connected = false;
				return false;
			}
			catch(...)
			{
				LOG_ERROR("Some fatal problems at read.");
				return false;
			}



			return false;
		}
		
		bool shutdown()
		{
			 
			 
			m_aborted = true;
			m_connected = false;
#ifdef BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR
			 
			const int saved_errno = errno;
			const char wake = 0;
			while (::write(m_wake_writer, &wake, 1) == -1 && errno == EINTR) {}
			errno = saved_errno;  
#else
			 
			boost::asio::post(m_io_service, []{});
#endif
			return true;
		}
		
		boost::asio::io_context& get_io_service()
		{
			return m_io_service;
		}

		boost::asio::ip::tcp::socket& get_socket()
		{
			return m_ssl_socket->next_layer();
		}

		uint64_t get_bytes_sent() const
		{
			return m_bytes_sent;
		}

		uint64_t get_bytes_received() const
		{
			return m_bytes_received;
		}

	private:

		struct pending_read
		{
			char byte = 0;
			boost::system::error_code error = boost::asio::error::would_block;
			std::function<void(const boost::system::error_code&, char)> handler;
		};

		bool shutdown_requested() const
		{
			return m_aborted.load();
		}

#ifdef BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR
		 
		void wait_wake()
		{
			m_wake_reader.async_read_some(boost::asio::buffer(m_wake_buf),
				[this] (const boost::system::error_code& ec, std::size_t) { if (!ec) wait_wake(); });
		}
#endif

		void check_deadline()
		{
			 
			 
			 
			if (m_deadline.expiry() <= std::chrono::steady_clock::now())
			{
				 
				 
				 
				LOG_PRINT_L3("Timed out socket");
        m_connected = false;
				m_ssl_socket->next_layer().close();

				 
				 
				m_deadline.expires_at(std::chrono::steady_clock::time_point::max());
			}

			 
			m_deadline.async_wait(boost::bind(&blocked_mode_client::check_deadline, this));
		}

		void shutdown_ssl() {
			 
			boost::system::error_code ec = boost::asio::error::would_block;
			m_deadline.expires_after(std::chrono::milliseconds(2000));
			m_ssl_socket->async_shutdown(boost::lambda::var(ec) = boost::lambda::_1);
			while (ec == boost::asio::error::would_block)
			{
				m_io_service.restart();
				m_io_service.run_one();
			}
			 
			if (ec.category() == boost::asio::error::get_ssl_category() &&
			    ec.value() !=
#if BOOST_VERSION >= 106200
			    boost::asio::ssl::error::stream_truncated
#else  
			    ERR_PACK(ERR_LIB_SSL, 0, SSL_R_SHORT_READ)
#endif
			    )
				MDEBUG("Problems at ssl shutdown: " << ec.message());
		}
		
	protected:
		bool write(const void* data, size_t sz, boost::system::error_code& ec)
		{
			bool success;
			if(m_ssl_options.support != ssl_support_t::e_ssl_support_disabled)
				success = boost::asio::write(*m_ssl_socket, boost::asio::buffer(data, sz), ec);
			else
				success = boost::asio::write(m_ssl_socket->next_layer(), boost::asio::buffer(data, sz), ec);
			return success;
		}
		
		void async_write(const void* data, size_t sz, boost::system::error_code& ec) 
		{
			if(m_ssl_options.support != ssl_support_t::e_ssl_support_disabled)
				boost::asio::async_write(*m_ssl_socket, boost::asio::buffer(data, sz), boost::lambda::var(ec) = boost::lambda::_1);
			else
				boost::asio::async_write(m_ssl_socket->next_layer(), boost::asio::buffer(data, sz), boost::lambda::var(ec) = boost::lambda::_1);
		}
		
		void async_read(char* buff, size_t sz, boost::asio::detail::transfer_at_least_t transfer_at_least, handler_obj& hndlr)
		{
			 
			if (sz && m_pending_read)
			{
				const auto pending = m_pending_read;
				auto on_read = [socket = m_ssl_socket, buff, sz, transfer_at_least, hndlr]
					(const boost::system::error_code& error, char byte) mutable {
						if (error)
						{
							hndlr(error, 0);
							return;
						}
						buff[0] = byte;
						if (!transfer_at_least(error, 1) || sz == 1)
							hndlr(error, 1);
						else
							boost::asio::async_read(*socket, boost::asio::buffer(buff + 1, sz - 1),
								[transfer_at_least](const boost::system::error_code& ec, size_t bytes) mutable {
									return transfer_at_least(ec, bytes + 1);
								},
								[hndlr](const boost::system::error_code& ec, size_t bytes) mutable { hndlr(ec, bytes + 1); });
					};
				if (pending->error == boost::asio::error::would_block)
					pending->handler = std::move(on_read);
				m_pending_read.reset();
				if (pending->error != boost::asio::error::would_block)
					on_read(pending->error, pending->byte);
				return;
			}
			if(m_ssl_options.support == ssl_support_t::e_ssl_support_disabled)
				boost::asio::async_read(m_ssl_socket->next_layer(), boost::asio::buffer(buff, sz), transfer_at_least, hndlr);
			else
				boost::asio::async_read(*m_ssl_socket, boost::asio::buffer(buff, sz), transfer_at_least, hndlr);
			
		}
		
	protected:
		boost::asio::io_context m_io_service;
		boost::asio::ssl::context m_ctx;
		std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_ssl_socket;
		std::shared_ptr<pending_read> m_pending_read;
		std::function<connect_func> m_connector;
		ssl_options_t m_ssl_options;
		bool m_initialized;
		std::atomic<bool> m_connected;
		boost::asio::steady_timer m_deadline;
#ifdef BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR
		 
		boost::asio::posix::stream_descriptor m_wake_reader;
		int m_wake_writer;
		char m_wake_buf[8];
#endif
		 
		std::atomic<bool> m_aborted;
		std::atomic<uint64_t> m_bytes_sent;
		std::atomic<uint64_t> m_bytes_received;
	};
}
}

