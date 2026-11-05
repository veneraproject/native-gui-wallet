                                       
 
 
 

                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 
 


#ifndef INCLUDED_network_throttle_hpp
#define INCLUDED_network_throttle_hpp

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/thread.hpp>

#include "syncobj.h"

#include "net/net_utils_base.h" 
#include "misc_log_ex.h" 
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/chrono.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "misc_language.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <memory>
#include <mutex>
#include <fstream>

namespace epee
{
namespace net_utils
{

 
typedef double network_speed_kbps;    
typedef double network_speed_bps;     
typedef double network_time_seconds;
typedef double network_MB;

class i_network_throttle;

 
 
 
struct calculate_times_struct {
		double average;
		double window;
		double delay;
		double recomendetDataSize;
};
typedef calculate_times_struct calculate_times_struct;


 
 
 
class network_throttle_manager {
	 

	 
	 
	
	 
	public:  

    static boost::mutex m_lock_get_global_throttle_in;
    static boost::mutex m_lock_get_global_throttle_inreq;
    static boost::mutex m_lock_get_global_throttle_out;

		friend class connection_basic;  
		friend class connection_basic_pimpl;  

	public:
		static i_network_throttle & get_global_throttle_in();  
		static i_network_throttle & get_global_throttle_inreq();  
		static i_network_throttle & get_global_throttle_out();  
};



 
 
 
class i_network_throttle {
	public:
		virtual void set_name(const std::string &name)=0;
		virtual void set_target_speed( network_speed_kbps target )=0;
		virtual network_speed_kbps get_target_speed()=0;

		virtual void handle_trafic_exact(size_t packet_size) =0;  
		virtual void handle_trafic_tcp(size_t packet_size) =0;  
		virtual void tick() =0;  
		
		 
		
		virtual void calculate_times(size_t packet_size, calculate_times_struct &cts, bool dbg, double force_window) const =0;  
		 

		 

		virtual network_time_seconds get_sleep_time(size_t packet_size) const =0;  
		virtual network_time_seconds get_sleep_time_after_tick(size_t packet_size) =0;  

		virtual size_t get_recommended_size_of_planned_transport() const =0;  

		virtual double get_time_seconds() const =0;  
		virtual void logger_handle_net(const std::string &filename, double time, size_t size)=0;
		virtual void get_stats(uint64_t &total_packets, uint64_t &total_bytes) const =0;


};


 


}  
}  


#endif



