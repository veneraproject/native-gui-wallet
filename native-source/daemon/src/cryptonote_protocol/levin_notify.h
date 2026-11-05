                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <vector>

#include "byte_slice.h"
#include "cryptonote_basic/blobdatatype.h"
#include "cryptonote_protocol/enums.h"
#include "cryptonote_protocol/fwd.h"
#include "net/enums.h"
#include "span.h"

namespace epee
{
namespace levin
{
    template<typename> class async_protocol_handler_config;
}
}

namespace nodetool
{
  template<typename> struct p2p_connection_context_t;
}

namespace cryptonote
{
namespace levin
{
  namespace detail
  {
    using p2p_context = nodetool::p2p_connection_context_t<cryptonote::cryptonote_connection_context>;
    struct zone;  
  }  

  using connections = epee::levin::async_protocol_handler_config<detail::p2p_context>;

   
  class notify
  {
    std::shared_ptr<detail::zone> zone_;
    i_core_events* core_;

  public:
    struct status
    {
      bool has_noise;
      bool connections_filled;  
      bool has_outgoing;  
    };

     
    notify() noexcept
      : zone_(nullptr)
      , core_(nullptr)
    {}

     
    explicit notify(boost::asio::io_service& service, std::shared_ptr<connections> p2p, epee::byte_slice noise, epee::net_utils::zone zone, bool pad_txs, i_core_events& core);

    notify(const notify&) = delete;
    notify(notify&&) = default;

    ~notify() noexcept;

    notify& operator=(const notify&) = delete;
    notify& operator=(notify&&) = default;

     
    status get_status() const noexcept;

     
    void new_out_connection();

    void on_handshake_complete(const boost::uuids::uuid &id, bool is_income);
    void on_connection_close(const boost::uuids::uuid &id);

     
    void run_epoch();

     
    void run_stems();

     
    void run_fluff();

     
 
 
 
 

 
 

 
 
 
 

 
    bool send_txs(std::vector<blobdata> txs, const boost::uuids::uuid& source, relay_method tx_relay);
  };
}  
}  
