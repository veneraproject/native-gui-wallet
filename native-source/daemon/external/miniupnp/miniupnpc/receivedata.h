                                                               
                    
                                                             
                         
                                         
                                                       
                                                           
#ifndef RECEIVEDATA_H_INCLUDED
#define RECEIVEDATA_H_INCLUDED

#include "miniupnpc_socketdef.h"

 
 
 
int receivedata(SOCKET socket,
                char * data, int length,
                int timeout, unsigned int * scope_id);

#endif

