                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   


#pragma once

#include "int-util.h"

 
 
 

namespace epee
{
  namespace net_utils
  {
    inline
    bool is_ip_local(uint32_t ip)
    {
      ip = SWAP32LE(ip);
       
 
 
 
 
 
      if( (ip | 0xffffff00) == 0xffffff0a)
        return true;

      if( (ip | 0xffff0000) == 0xffffa8c0)
        return true;

      if( (ip | 0xffffff00) == 0xffffffac)
      {
        uint32_t second_num = (ip >> 8) & 0xff;
        if(second_num >= 16 && second_num <= 31 )
          return true;
      }
      return false;
    }
    inline
    bool is_ip_loopback(uint32_t ip)
    {
      ip = SWAP32LE(ip);
      if( (ip | 0xffffff00) == 0xffffff7f)
        return true;
       
       
 
 
 
      return false;
    }
   
  }
}

