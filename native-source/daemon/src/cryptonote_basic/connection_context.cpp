                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "connection_context.h"

#include "cryptonote_protocol/cryptonote_protocol_defs.h"
#include "p2p/p2p_protocol_defs.h"

namespace cryptonote
{
  std::size_t cryptonote_connection_context::get_max_bytes(const int command) noexcept
  {
    switch (command)
    {
    case nodetool::COMMAND_HANDSHAKE_T<cryptonote::CORE_SYNC_DATA>::ID:
      return 65536;
    case nodetool::COMMAND_TIMED_SYNC_T<cryptonote::CORE_SYNC_DATA>::ID:
      return 65536;
    case nodetool::COMMAND_PING::ID:
      return 4096;
    case nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID:
      return 4096;
    case cryptonote::NOTIFY_NEW_BLOCK::ID:
      return 1024 * 1024 * 128;  
    case cryptonote::NOTIFY_NEW_TRANSACTIONS::ID:
      return 1024 * 1024 * 128;  
    case cryptonote::NOTIFY_REQUEST_GET_OBJECTS::ID:
      return 1024 * 1024 * 2;  
    case cryptonote::NOTIFY_RESPONSE_GET_OBJECTS::ID:
      return 1024 * 1024 * 128;  
    case cryptonote::NOTIFY_REQUEST_CHAIN::ID:
      return 512 * 1024;  
    case cryptonote::NOTIFY_RESPONSE_CHAIN_ENTRY::ID:
      return 1024 * 1024 * 4;  
    case cryptonote::NOTIFY_NEW_FLUFFY_BLOCK::ID:
      return 1024 * 1024 * 4;  
    case cryptonote::NOTIFY_REQUEST_FLUFFY_MISSING_TX::ID:
      return 1024 * 1024;  
    case cryptonote::NOTIFY_GET_TXPOOL_COMPLEMENT::ID:
      return 1024 * 1024 * 4;  
    default:
      break;
    };
    return std::numeric_limits<size_t>::max();
  }
}  
