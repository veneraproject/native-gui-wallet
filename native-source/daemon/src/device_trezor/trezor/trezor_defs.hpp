                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  

#ifndef USE_DEVICE_TREZOR
#define USE_DEVICE_TREZOR 1
#endif

 
 
#ifndef DEVICE_TREZOR_READY
#undef  USE_DEVICE_TREZOR
#define USE_DEVICE_TREZOR 0
#endif

#if USE_DEVICE_TREZOR
#define WITH_DEVICE_TREZOR 1
#endif

#ifndef WITH_DEVICE_TREZOR
#undef WITH_DEVICE_TREZOR_LITE
#endif

#if defined(HAVE_TREZOR_LIBUSB) && USE_DEVICE_TREZOR
#define WITH_DEVICE_TREZOR_WEBUSB 1
#endif

 
#ifndef USE_DEVICE_TREZOR_UDP
#define USE_DEVICE_TREZOR_UDP 1
#endif

 
#ifndef USE_DEVICE_TREZOR_UDP_RELEASE
#define USE_DEVICE_TREZOR_UDP_RELEASE 0
#endif

#if USE_DEVICE_TREZOR_UDP && (USE_DEVICE_TREZOR_UDP_RELEASE || defined(TREZOR_DEBUG))
#define WITH_DEVICE_TREZOR_UDP 1
#endif

 
#ifndef PROTOBUF_INLINE_NOT_IN_HEADERS
#define PROTOBUF_INLINE_NOT_IN_HEADERS 0
#endif

 
#ifdef minor
#undef minor
#endif
