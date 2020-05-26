#ifndef UXR_CLIENT_PROFILE_TRANSPORT_IP_TCP_EXTERNAL_H_
#define UXR_CLIENT_PROFILE_TRANSPORT_IP_TCP_EXTERNAL_H_

#include <uxr/client/config.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct uxrTCPPlatform
{
  int socket_fd;
  void* client_instance;
}uxrTCPPlatform;

#ifdef __cplusplus
}
#endif

#endif // UXR_CLIENT_PROFILE_TRANSPORT_IP_TCP_EXTERNAL_H_