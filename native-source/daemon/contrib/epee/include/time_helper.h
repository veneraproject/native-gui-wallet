                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   



#pragma once

#include <chrono>
#include <cstdio>
#include <ctime>
#include <string>

namespace epee
{
namespace misc_utils
{
	inline bool get_gmt_time(time_t t, struct tm &tm)
	{
#ifdef _WIN32
		return gmtime_s(&tm, &t);
#else
		return gmtime_r(&t, &tm);
#endif
	}

	inline std::string get_internet_time_str(const time_t& time_)
	{
		char tmpbuf[200] = {0};
		struct tm pt;
		get_gmt_time(time_, pt);
		strftime(tmpbuf, 199, "%a, %d %b %Y %H:%M:%S GMT", &pt);
		return tmpbuf;
	}

	inline std::string get_time_interval_string(const time_t& time_)
	{
		time_t tail = time_;
		const int days = static_cast<int>(tail/(60*60*24));
		tail = tail%(60*60*24);
		const int hours = static_cast<int>(tail/(60*60));
		tail = tail%(60*60);
		const int minutes = static_cast<int>(tail/60);
		tail = tail%(60);
		const int seconds = static_cast<int>(tail);

		char tmpbuf[64] = {0};
		snprintf(tmpbuf, sizeof(tmpbuf) - 1, "d%d.h%d.m%d.s%d", days, hours, minutes, seconds);

		return tmpbuf;
	}

	inline uint64_t get_ns_count()
	{
		typedef std::chrono::duration<uint64_t, std::nano> ns_duration;
		const ns_duration ns_since_epoch = std::chrono::steady_clock::now().time_since_epoch();
		return ns_since_epoch.count();
	}

	inline uint64_t get_tick_count()
	{
		return get_ns_count() / 1000000;
	}
}
}
