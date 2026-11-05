                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#pragma once

#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "p2p/net_node.h"
#include "daemon/protocol.h"
#include "daemon/command_line_args.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{

class t_p2p final
{
private:
  typedef cryptonote::t_cryptonote_protocol_handler<cryptonote::core> t_protocol_raw;
  typedef nodetool::node_server<t_protocol_raw> t_node_server;
public:
  static void init_options(boost::program_options::options_description & option_spec)
  {
    t_node_server::init_options(option_spec);
  }
private:
  t_node_server m_server;
public:
  t_p2p(
      boost::program_options::variables_map const & vm
    , t_protocol & protocol
    )
    : m_server{protocol.get()}
  {
     
    MGINFO_CYAN("Launching P2P server");
    if (!m_server.init(vm, command_line::get_arg(vm, daemon_args::arg_proxy), command_line::get_arg(vm, daemon_args::arg_proxy_allow_dns_leaks)))
    {
      throw std::runtime_error("Failed to initialize p2p server.");
    }
    MGINFO_CYAN("P2P server successfully launched");
  }

  t_node_server & get()
  {
    return m_server;
  }

  void run()
  {
     
    m_server.run();
    MGINFO("p2p net loop stopped");
  }

  void stop()
  {
    m_server.send_stop_signal();
  }

  ~t_p2p()
  {
    MGINFO("Deinitializing p2p...");
    try {
      m_server.deinit();
    } catch (...) {
      MERROR("Failed to deinitialize p2p...");
    }
  }
};

}
