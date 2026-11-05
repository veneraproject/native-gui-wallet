                                                             
                    
                                                             
                         
                                         
                                                       
                                                           
#ifndef MINISSDPC_H_INCLUDED
#define MINISSDPC_H_INCLUDED

#include "miniupnpc_declspec.h"
#include "upnpdev.h"

 
#define MINISSDPC_SUCCESS (0)
#define MINISSDPC_UNKNOWN_ERROR (-1)
#define MINISSDPC_SOCKET_ERROR (-101)
#define MINISSDPC_MEMORY_ERROR (-102)
#define MINISSDPC_INVALID_INPUT (-103)
#define MINISSDPC_INVALID_SERVER_REPLY (-104)

#ifdef __cplusplus
extern "C" {
#endif

#if !(defined(_WIN32) || defined(__amigaos__) || defined(__amigaos4__))

MINIUPNP_LIBSPEC struct UPNPDev *
getDevicesFromMiniSSDPD(const char * devtype, const char * socketpath, int * error);

MINIUPNP_LIBSPEC int
connectToMiniSSDPD(const char * socketpath);

MINIUPNP_LIBSPEC int
disconnectFromMiniSSDPD(int s);

MINIUPNP_LIBSPEC int
requestDevicesFromMiniSSDPD(int s, const char * devtype);

MINIUPNP_LIBSPEC struct UPNPDev *
receiveDevicesFromMiniSSDPD(int s, int * error);

#endif  

MINIUPNP_LIBSPEC struct UPNPDev *
ssdpDiscoverDevices(const char * const deviceTypes[],
                    int delay, const char * multicastif,
                    int localport,
                    int ipv6, unsigned char ttl,
                    int * error,
                    int searchalltypes);

#ifdef __cplusplus
}
#endif

#endif

