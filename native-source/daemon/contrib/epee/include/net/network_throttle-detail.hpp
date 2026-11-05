                                       
 
 
 

                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 


#ifndef INCLUDED_throttle_detail_hpp
#define INCLUDED_throttle_detail_hpp

#include <boost/circular_buffer.hpp>
#include "network_throttle.hpp"

namespace epee
{
namespace net_utils
{


class network_throttle : public i_network_throttle {
	private:
		struct packet_info {
			size_t m_size;  
			packet_info();
		};


		network_speed_bps m_target_speed;
		size_t m_network_add_cost;  
		size_t m_network_minimal_segment;  
		size_t m_network_max_segment;  

		const size_t m_window_size;  
		network_time_seconds m_slot_size;  
		 

		boost::circular_buffer< packet_info > m_history;  
		network_time_seconds m_last_sample_time;  
		network_time_seconds m_start_time;  
		bool m_any_packet_yet;  
		uint64_t m_total_packets;
		uint64_t m_total_bytes;

		std::string m_name;  
		std::string m_nameshort;  

	 
	public:
		network_throttle(const std::string &nameshort, const std::string &name, int window_size=-1);
		virtual ~network_throttle();
		virtual void set_name(const std::string &name);
		virtual void set_target_speed( network_speed_kbps target );
		virtual network_speed_kbps get_target_speed();

		 
		virtual void handle_trafic_exact(size_t packet_size);  
		virtual void handle_trafic_tcp(size_t packet_size);  

		virtual void tick();  

		virtual double get_time_seconds() const ;  

		 
		virtual void calculate_times(size_t packet_size, calculate_times_struct &cts, bool dbg, double force_window) const;  

		virtual network_time_seconds get_sleep_time_after_tick(size_t packet_size);  
		virtual network_time_seconds get_sleep_time(size_t packet_size) const;  

		virtual size_t get_recommended_size_of_planned_transport() const;  
		virtual size_t get_recommended_size_of_planned_transport_window(double force_window) const;   
		virtual double get_current_speed() const;
		virtual void get_stats(uint64_t &total_packets, uint64_t &total_bytes) const;

	private:
		virtual network_time_seconds time_to_slot(network_time_seconds t) const { return std::floor( t ); }  
        virtual void _handle_trafic_exact(size_t packet_size, size_t orginal_size);
        virtual void logger_handle_net(const std::string &filename, double time, size_t size);
};

 
 
 
struct network_throttle_bw {
	public:
		network_throttle m_in;  
		network_throttle m_inreq;  
		network_throttle m_out;  

	public:
		network_throttle_bw(const std::string &name1);
};



}  
}  


#endif


