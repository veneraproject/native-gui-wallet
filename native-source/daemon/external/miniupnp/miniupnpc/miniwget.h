                                                             
                     
                          
                                         
                                                             
                                              
     
#ifndef MINIWGET_H_INCLUDED
#define MINIWGET_H_INCLUDED

#include "miniupnpc_declspec.h"

#ifdef __cplusplus
extern "C" {
#endif

MINIUPNP_LIBSPEC void * miniwget(const char *, int *, unsigned int, int *);

MINIUPNP_LIBSPEC void * miniwget_getaddr(const char *, int *, char *, int, unsigned int, int *);

int parseURL(const char *, char *, unsigned short *, char * *, unsigned int *);

#ifdef __cplusplus
}
#endif

#endif
