  
                                    
  
                                           
                       
  
                                                                     
                                                                     
           
                                                                    
                                                                   
                                                                       
                                                                         
                                                                          
                                                                    
                                                                     
                                                                   
  
                                                                          
                                                                        
                                                                             
                                                                           
                                                                             
                                                                          
                                                                        
                                                                             
                                                                            
                                                                         
               
   

 
 
 

 
 

                                                                  
                

                                                                 
                                                                
                                                                  
                 

                                                                 
                                                            
                                                            
                                           

                                                                  
                                                               
                                                           
                                                
                                                                
                              
   
#ifndef MD5_H
#define MD5_H


#include "md5global.h"

namespace md5
{
	 
	typedef struct {
		UINT4 state[4];                                    
		UINT4 count[2];         
		unsigned char buffer[64];                          
	} MD5_CTX;

	static void MD5Init(MD5_CTX * context);
	static void MD5Update( MD5_CTX *context, const unsigned char *input, unsigned int inputLen );
	static void MD5Final ( unsigned char digest[16], MD5_CTX *context );

	
	inline bool md5( unsigned char *input, int ilen, unsigned char output[16] )
	{
		MD5_CTX ctx;

		MD5Init( &ctx );
		MD5Update( &ctx, input, ilen );
		MD5Final( output, &ctx);

		memwipe( &ctx, sizeof( MD5_CTX ));
		return true;
	}


}

#include "md5_l.inl"

#endif
