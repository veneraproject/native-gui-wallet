                                       
 
 
 
 
 

 
 
 
 

 

 
 
 
 
 
 
 
 
 
 
 


 

                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "net/network_throttle-detail.hpp"

namespace epee
{
namespace net_utils
{

 
 
 

 
 
boost::mutex network_throttle_manager::m_lock_get_global_throttle_in;
boost::mutex network_throttle_manager::m_lock_get_global_throttle_inreq;
boost::mutex network_throttle_manager::m_lock_get_global_throttle_out;

 
 
i_network_throttle & network_throttle_manager::get_global_throttle_in() { 
	static network_throttle obj_get_global_throttle_in("in/all","<<< global-IN",10);
	return obj_get_global_throttle_in;
}



i_network_throttle & network_throttle_manager::get_global_throttle_inreq() { 
	static network_throttle obj_get_global_throttle_inreq("inreq/all", "<== global-IN-REQ",10);
	return obj_get_global_throttle_inreq;
}


i_network_throttle & network_throttle_manager::get_global_throttle_out() { 
	static network_throttle obj_get_global_throttle_out("out/all", ">>> global-OUT",10);
	return obj_get_global_throttle_out;
}




network_throttle_bw::network_throttle_bw(const std::string &name1) 
	: m_in("in/"+name1, name1+"-DOWNLOAD"), m_inreq("inreq/"+name1, name1+"-DOWNLOAD-REQUESTS"), m_out("out/"+name1, name1+"-UPLOAD")
{ }




}  
}  





