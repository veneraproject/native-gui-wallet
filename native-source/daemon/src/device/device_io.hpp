                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  




#pragma once


namespace hw {
  namespace io {

    class device_io {

    public:

      device_io()   {};
      ~device_io() {};

      virtual void init()  = 0;
      virtual void release() = 0;

      virtual void connect(void *parms) = 0;
      virtual void disconnect() = 0;
      virtual bool connected() const = 0;

      virtual int  exchange(unsigned char *command, unsigned int cmd_len, unsigned char *response, unsigned int max_resp_len, bool user_input) = 0;
    };
  };
};
