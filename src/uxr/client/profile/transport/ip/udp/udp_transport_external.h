#ifndef UXR_CLIENT_PROFILE_TRANSPORT_IP_UDP_EXTERNAL_H_
#define UXR_CLIENT_PROFILE_TRANSPORT_IP_UDP_EXTERNAL_H_

#include <uxr/client/config.h>

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

#endif // UXR_CLIENT_PROFILE_TRANSPORT_IP_UDP_EXTERNAL_H_