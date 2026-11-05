                                       
 
 
 

                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include <string>
#include <vector>

#include <memory>

#include "syncobj.h"

#include "net/net_utils_base.h" 
#include "misc_log_ex.h" 
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include "misc_language.h"
#include <algorithm>


#include "cryptonote_protocol_handler.h"
#include "net/network_throttle.hpp"

#include "cryptonote_core/cryptonote_core.h"  

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "net.cn"

 
 
 
 
 
 
 

namespace cryptonote {

class cryptonote_protocol_handler_base_pimpl {  
	public:

};

}  

 
 
 
 

namespace cryptonote { 

double cryptonote_protocol_handler_base::estimate_one_block_size() noexcept {  
 const double size_min = 500;  
  

 double avg=0;
 try {
 avg = get_avg_block_size( );
 } catch (...) { }
 avg = std::max( size_min , avg);
 return avg;
}

cryptonote_protocol_handler_base::cryptonote_protocol_handler_base() {
}

cryptonote_protocol_handler_base::~cryptonote_protocol_handler_base() {
}

void cryptonote_protocol_handler_base::handler_request_blocks_history(std::list<crypto::hash>& ids) {
}

void cryptonote_protocol_handler_base::handler_response_blocks_now(size_t packet_size) {
	using namespace epee::net_utils;
	double delay=0;  
	MDEBUG("Packet size: " << packet_size);
	do
	{  
		 
		 
 
 
 
		 
 
 
 

		{ 
	  	CRITICAL_REGION_LOCAL(	network_throttle_manager::m_lock_get_global_throttle_out );
			delay = network_throttle_manager::get_global_throttle_out().get_sleep_time_after_tick( packet_size );
		}

		
		delay *= 0.50;
		 
		if (delay > 0) {
			 
            		long int ms = (long int)(delay * 1000);
			MDEBUG("Sleeping for " << ms << " ms before packet_size="<<packet_size);  
			boost::this_thread::sleep(boost::posix_time::milliseconds( ms ) );  
		}
	} while(delay > 0);

 
	{
	  CRITICAL_REGION_LOCAL(	network_throttle_manager::m_lock_get_global_throttle_out );
		network_throttle_manager::get_global_throttle_out().handle_trafic_tcp( packet_size );  
		 
		 
	}
}

}  


