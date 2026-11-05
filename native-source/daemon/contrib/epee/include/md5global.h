  
                                    
  
                                           
                       
  
                                                                     
                                                                     
           
                                                                    
                                                                   
                                                                       
                                                                         
                                                                          
                                                                    
                                                                     
                                                                   
  
                                                                          
                                                                        
                                                                             
                                                                           
                                                                             
                                                                          
                                                                        
                                                                             
                                                                            
                                                                         
               
   

 
 
 

 
 

#ifndef MD5GLOBAL_H
#define MD5GLOBAL_H

namespace md5
{


	 
 
 
 
 
#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

	 
	typedef unsigned char *POINTER;

	 
	typedef unsigned short int UINT2;

	 
	 
	typedef unsigned int UINT4;

	 
 
 
 
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

}

#endif
