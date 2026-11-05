                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "common/venera_branding.h"
#include <boost/algorithm/string.hpp>
#include "cryptonote_config.h"
#include "version.h"
#include "string_tools.h"
#include "daemon/command_server.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize {

namespace p = std::placeholders;

t_command_server::t_command_server(
    uint32_t ip
  , uint16_t port
  , const boost::optional<tools::login>& login
  , const epee::net_utils::ssl_options_t& ssl_options
  , bool is_rpc
  , cryptonote::core_rpc_server* rpc_server
  )
  : m_parser(ip, port, login, ssl_options, is_rpc, rpc_server)
  , m_command_lookup()
  , m_is_rpc(is_rpc)
{
  m_command_lookup.set_handler(
      "help"
    , std::bind(&t_command_server::help, this, p::_1)
    , "help [<command>]"
    , "Show the help section or the documentation about a <command>."
    );
  m_command_lookup.set_handler(
      "print_height"
    , std::bind(&t_command_parser_executor::print_height, &m_parser, p::_1)
    , "Print the local blockchain height."
    );
  m_command_lookup.set_handler(
      "print_cn"
    , std::bind(&t_command_parser_executor::print_connections, &m_parser, p::_1)
    , "Print the current connections."
    );
  m_command_lookup.set_handler(
      "print_net_stats"
    , std::bind(&t_command_parser_executor::print_net_stats, &m_parser, p::_1)
    , "Print network statistics."
    );
  m_command_lookup.set_handler(
      "print_block"
    , std::bind(&t_command_parser_executor::print_block, &m_parser, p::_1)
    , "print_block <block_hash> | <block_height>"
    , "Print a given block."
    );
  m_command_lookup.set_handler(
      "print_tx"
    , std::bind(&t_command_parser_executor::print_transaction, &m_parser, p::_1)
    , "print_tx <transaction_hash> [+hex] [+json]"
    , "Print a given transaction."
    );
  m_command_lookup.set_handler(
      "is_key_image_spent"
    , std::bind(&t_command_parser_executor::is_key_image_spent, &m_parser, p::_1)
    , "is_key_image_spent <key_image>"
    , "Print whether a given key image is in the spent key images set."
    );
  m_command_lookup.set_handler(
      "print_pool_stats"
    , std::bind(&t_command_parser_executor::print_transaction_pool_stats, &m_parser, p::_1)
    , "Print the transaction pool's statistics."
    );
  m_command_lookup.set_handler(
      "stop_node"
    , std::bind(&t_command_parser_executor::stop_daemon, &m_parser, p::_1)
    , "Stop the Venera node."
    );
  m_command_lookup.set_handler(
      "limit"
    , std::bind(&t_command_parser_executor::set_limit, &m_parser, p::_1)
    , "limit [<kB/s>]"
    , "Get or set the download and upload limit."
    );
  m_command_lookup.set_handler(
      "limit_up"
    , std::bind(&t_command_parser_executor::set_limit_up, &m_parser, p::_1)
    , "limit_up [<kB/s>]"
    , "Get or set the upload limit."
    );
  m_command_lookup.set_handler(
      "limit_down"
    , std::bind(&t_command_parser_executor::set_limit_down, &m_parser, p::_1)
    , "limit_down [<kB/s>]"
    , "Get or set the download limit."
    );
  m_command_lookup.set_handler(
      "out_peers"
    , std::bind(&t_command_parser_executor::out_peers, &m_parser, p::_1)
    , "out_peers <max_number>"
    , "Set the <max_number> of out peers."
    );
  m_command_lookup.set_handler(
      "in_peers"
    , std::bind(&t_command_parser_executor::in_peers, &m_parser, p::_1)
    , "in_peers <max_number>"
    , "Set the <max_number> of in peers."
    );
  m_command_lookup.set_handler(
      "bans"
    , std::bind(&t_command_parser_executor::show_bans, &m_parser, p::_1)
    , "Show the currently banned IPs."
    );
  m_command_lookup.set_handler(
      "ban"
    , std::bind(&t_command_parser_executor::ban, &m_parser, p::_1)
    , "ban [<IP>|@<filename>] [<seconds>]"
    , "Ban a given <IP> or list of IPs from a file for a given amount of <seconds>."
    );
  m_command_lookup.set_handler(
      "unban"
    , std::bind(&t_command_parser_executor::unban, &m_parser, p::_1)
    , "unban <address>"
    , "Unban a given <IP>."
    );
  m_command_lookup.set_handler(
      "flush_txpool"
    , std::bind(&t_command_parser_executor::flush_txpool, &m_parser, p::_1)
    , "flush_txpool [<txid>]"
    , "Flush a transaction from the tx pool by its <txid>, or the whole tx pool."
    );
  m_command_lookup.set_handler(
      "print_coinbase_tx_sum"
    , std::bind(&t_command_parser_executor::print_coinbase_tx_sum, &m_parser, p::_1)
    , "print_coinbase_tx_sum <start_height> [<block_count>]"
    , "Print the sum of coinbase transactions."
    );
  m_command_lookup.set_handler(
      "sync_info"
    , std::bind(&t_command_parser_executor::sync_info, &m_parser, p::_1)
    , "Print information about the blockchain sync state."
    );
  m_command_lookup.set_handler(
      "flush_cache"
    , std::bind(&t_command_parser_executor::flush_cache, &m_parser, p::_1)
    , "flush_cache [bad-txs] [bad-blocks]"
    , "Flush the specified cache(s)."
    );

}

bool t_command_server::process_command_str(const std::string& cmd)
{
  return m_command_lookup.process_command_str(cmd);
}

bool t_command_server::process_command_vec(const std::vector<std::string>& cmd)
{
  bool result = m_command_lookup.process_command_vec(cmd);
  if (!result)
  {
    help(std::vector<std::string>());
  }
  return result;
}

bool t_command_server::start_handling(std::function<void(void)> exit_handler)
{
  if (m_is_rpc) return false;

  m_command_lookup.start_handling("", "", exit_handler);

  return true;
}

void t_command_server::stop_handling()
{
  if (m_is_rpc) return;

  m_command_lookup.stop_handling();
}

bool t_command_server::help(const std::vector<std::string>& args)
{
  if(args.empty())
  {
    std::cout << "\033[36m" << get_commands_str() << "\033[0m" << std::endl;
  }
  else
  {
    std::cout << "\033[36m" << get_command_usage(args) << "\033[0m" << std::endl;
  }
  return true;
}

bool t_command_server::apropos(const std::vector<std::string>& args)
{
  if (args.empty())
  {
    std::cout << "Missing keyword" << std::endl;
    return true;
  }
  const std::vector<std::string>& command_list = m_command_lookup.get_command_list(args);
  if (command_list.empty())
  {
    std::cout << "Nothing found" << std::endl;
    return true;
  }

  std::cout << std::endl;
  for(auto const& command:command_list)
  {
    std::vector<std::string> cmd;
    cmd.push_back(command);
    std::pair<std::string, std::string> documentation = m_command_lookup.get_documentation(cmd);
    std::cout << "  " << documentation.first << std::endl;
  }
  std::cout << std::endl;

  return true;
}

std::string t_command_server::get_commands_str()
{
  std::stringstream ss;
  ss << venera::node_title() << std::endl;
  ss << "Commands: " << std::endl;
  std::string usage = m_command_lookup.get_usage();
  boost::replace_all(usage, "\n", "\n  ");
  usage.insert(0, "  ");
  ss << usage;
  return ss.str();
}

 std::string t_command_server::get_command_usage(const std::vector<std::string> &args)
 {
   std::pair<std::string, std::string> documentation = m_command_lookup.get_documentation(args);
   std::stringstream ss;
   if(documentation.first.empty())
   {
     ss << "Unknown command: " << args.front() << std::endl;
   }
   else
   {
     std::string usage = documentation.second.empty() ? args.front() : documentation.first;
     std::string description = documentation.second.empty() ? documentation.first : documentation.second;
     usage.insert(0, "  ");
     ss << "Command usage: " << std::endl << usage << std::endl << std::endl;
     boost::replace_all(description, "\n", "\n  ");
     description.insert(0, "  ");
     ss << "Command description: " << std::endl << description << std::endl;
   }
   return ss.str();
 }

}  
