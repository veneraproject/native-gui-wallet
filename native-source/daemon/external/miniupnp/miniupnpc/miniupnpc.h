                                                              
                                          
                    
                           
                         
                                         
                                                       
                                                           
#ifndef MINIUPNPC_H_INCLUDED
#define MINIUPNPC_H_INCLUDED

#include "miniupnpc_declspec.h"
#include "igd_desc_parse.h"
#include "upnpdev.h"

 
#define UPNPDISCOVER_SUCCESS (0)
#define UPNPDISCOVER_UNKNOWN_ERROR (-1)
#define UPNPDISCOVER_SOCKET_ERROR (-101)
#define UPNPDISCOVER_MEMORY_ERROR (-102)

 
#define MINIUPNPC_VERSION	"2.2.1"
#define MINIUPNPC_API_VERSION	17

 
 
 
#define UPNP_LOCAL_PORT_ANY     0
#define UPNP_LOCAL_PORT_SAME    1

#ifdef __cplusplus
extern "C" {
#endif

 
struct UPNParg { const char * elt; const char * val; };

char *
simpleUPnPcommand(int, const char *, const char *,
                  const char *, struct UPNParg *,
                  int *);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscover(int delay, const char * multicastif,
             const char * minissdpdsock, int localport,
             int ipv6, unsigned char ttl,
             int * error);

MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscoverAll(int delay, const char * multicastif,
                const char * minissdpdsock, int localport,
                int ipv6, unsigned char ttl,
                int * error);

MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscoverDevice(const char * device, int delay, const char * multicastif,
                const char * minissdpdsock, int localport,
                int ipv6, unsigned char ttl,
                int * error);

MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscoverDevices(const char * const deviceTypes[],
                    int delay, const char * multicastif,
                    const char * minissdpdsock, int localport,
                    int ipv6, unsigned char ttl,
                    int * error,
                    int searchalltypes);

 
 
 
MINIUPNP_LIBSPEC void parserootdesc(const char *, int, struct IGDdatas *);

 
 
 
 
 
 
struct UPNPUrls {
	char * controlURL;
	char * ipcondescURL;
	char * controlURL_CIF;
	char * controlURL_6FC;
	char * rootdescURL;
};

 
 
 
 
 
 
 
 
 
 
 
 
MINIUPNP_LIBSPEC int
UPNP_GetValidIGD(struct UPNPDev * devlist,
                 struct UPNPUrls * urls,
				 struct IGDdatas * data,
				 char * lanaddr, int lanaddrlen);

 
 
 
 
 
 
MINIUPNP_LIBSPEC int
UPNP_GetIGDFromUrl(const char * rootdescurl,
                   struct UPNPUrls * urls,
                   struct IGDdatas * data,
                   char * lanaddr, int lanaddrlen);

MINIUPNP_LIBSPEC void
GetUPNPUrls(struct UPNPUrls *, struct IGDdatas *,
            const char *, unsigned int);

MINIUPNP_LIBSPEC void
FreeUPNPUrls(struct UPNPUrls *);

 
MINIUPNP_LIBSPEC int UPNPIGD_IsConnected(struct UPNPUrls *, struct IGDdatas *);


#ifdef __cplusplus
}
#endif

#endif

