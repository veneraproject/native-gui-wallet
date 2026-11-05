                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "net/zmq.h"

#include <cassert>
#include <cerrno>
#include <limits>
#include <utility>

#include "byte_slice.h"

namespace net
{
namespace zmq
{
    const std::error_category& error_category() noexcept
    {
        struct category final : std::error_category
        {
            virtual const char* name() const noexcept override final
            {
                return "error::error_category()";
            }

            virtual std::string message(int value) const override final
            {
                char const* const msg = zmq_strerror(value);
                if (msg)
                    return msg;
                return "zmq_strerror failure";
            }

            virtual std::error_condition default_error_condition(int value) const noexcept override final
            {
                 
                switch (value)
                {
                case EFSM:
                case ETERM:
                    break;
                default:
                     
 
 
 
 
                    return std::errc(value);
                }
                return std::error_condition{value, *this};
            }

        };
        static const category instance{};
        return instance;
    }

    void terminate::call(void* ptr) noexcept
    {
        assert(ptr != nullptr);  
        while (zmq_term(ptr))
        {
            if (zmq_errno() != EINTR)
                break;
        }
    }

    namespace
    {
         
        class message
        {
            zmq_msg_t handle_;

        public:
            message() noexcept
              : handle_()
            {
                zmq_msg_init(handle());
            }

            message(message&& rhs) = delete;
            message(const message& rhs) = delete;
            message& operator=(message&& rhs) = delete;
            message& operator=(const message& rhs) = delete;

            ~message() noexcept
            {
                zmq_msg_close(handle());
            }

            zmq_msg_t* handle() noexcept
            {
                return std::addressof(handle_);
            }

            const char* data() noexcept
            {
                return static_cast<const char*>(zmq_msg_data(handle()));
            }

            std::size_t size() noexcept
            {
                return zmq_msg_size(handle());
            }
        };

        struct do_receive
        {
             
 
 
 
 
 
 
            int operator()(std::string& payload, void* const socket, const int flags) const
            {
                static constexpr const int max_out = std::numeric_limits<int>::max();
                const std::string::size_type initial = payload.size();
                message part{};
                bool too_large = false;
                for (;;)
                {
                    int last = 0;
                    if ((last = zmq_msg_recv(part.handle(), socket, flags)) < 0)
                        return last;

                    if (!too_large)
                        too_large =
                            max_message_size < payload.size() ||
                            max_message_size - payload.size() < part.size();
                    if (!too_large)
                        payload.append(part.data(), part.size());
                    if (!zmq_msg_more(part.handle()))
                        break;
                }
                if (too_large)
                {
                    errno = EMSGSIZE;
                    return -1;
                }
                const std::string::size_type added = payload.size() - initial;
                return unsigned(max_out) < added ? max_out : int(added);
            }
        };
    }  

    expect<std::string> receive(void* const socket, const int flags)
    {
        std::string payload{};
        MONERO_CHECK(retry_op(do_receive{}, payload, socket, flags));
        return {std::move(payload)};
    }

    expect<void> send(const epee::span<const std::uint8_t> payload, void* const socket, const int flags) noexcept
    {
        return retry_op(zmq_send, socket, payload.data(), payload.size(), flags);
    }

    expect<void> send(epee::byte_slice&& payload, void* socket, int flags) noexcept
    {
        void* const data = const_cast<std::uint8_t*>(payload.data());
        const std::size_t size = payload.size();
        auto buffer = payload.take_buffer();  

        zmq_msg_t msg{};
        MONERO_ZMQ_CHECK(zmq_msg_init_data(std::addressof(msg), data, size, epee::release_byte_slice::call, buffer.get()));
        buffer.release();  

        expect<void> sent = retry_op(zmq_msg_send, std::addressof(msg), socket, flags);
        if (!sent)  
            zmq_msg_close(std::addressof(msg));
        return sent;
    }
}  
}  

