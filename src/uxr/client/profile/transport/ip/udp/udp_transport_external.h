#ifndef UXR_CLIENT_PROFILE_TRANSPORT_IP_UDP_EXTERNAL_H_
#define UXR_CLIENT_PROFILE_TRANSPORT_IP_UDP_EXTERNAL_H_

#include <uxr/client/config.h>

#if defined(UCLIENT_EXTERNAL_UDP)

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct uxrUDPPlatform
{
  int socket_fd;
  void* udp_instance;
} uxrUDPPlatform;


#ifdef __cplusplus
}
#endif

#endif //UCLIENT_EXTERNAL_UDP

#endif // UXR_CLIENT_PROFILE_TRANSPORT_IP_UDP_EXTERNAL_H_