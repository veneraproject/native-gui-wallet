                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

#include <boost/thread/thread.hpp>
#include <boost/utility/string_ref.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include "common/command_line.h"
#include "cryptonote_basic/fwd.h"
#include "net/zmq.h"
#include "rpc/fwd.h"
#include "rpc/rpc_handler.h"
#include "span.h"

namespace cryptonote
{

namespace rpc
{

class ZmqServer final
{
  public:

    ZmqServer(RpcHandler& h, bool restricted);

    ~ZmqServer();

    void serve();

     
    void* init_rpc(boost::string_ref address, boost::string_ref port);

     
    std::shared_ptr<listener::zmq_pub> init_pub(epee::span<const std::string> addresses);

    void run();
    void stop();

  private:
    RpcHandler& handler;
    const bool restricted;

    net::zmq::context context;

    boost::thread run_thread;

    net::zmq::socket rep_socket;
    net::zmq::socket pub_socket;
    net::zmq::socket relay_socket;
    std::shared_ptr<listener::zmq_pub> shared_state;
};

}   

}   
