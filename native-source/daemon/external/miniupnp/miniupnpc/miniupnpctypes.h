                                                                  
                                                                               
                          
                                    
                                                             
                                                    
#ifndef MINIUPNPCTYPES_H_INCLUDED
#define MINIUPNPCTYPES_H_INCLUDED

#if (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L)
#define UNSIGNED_INTEGER unsigned long long
#define STRTOUI	strtoull
#else
#define UNSIGNED_INTEGER unsigned int
#define STRTOUI	strtoul
#endif

#endif

