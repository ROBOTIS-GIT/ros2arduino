#include <uxr/client/config.h>


#ifdef PROFILE_UDP_TRANSPORT
#if defined(UCLIENT_EXTERNAL_UDP)
#include <uxr/client/profile/transport/ip/udp/udp_transport_external.h>
#endif
#endif //PROFILE_UDP_TRANSPORT

#ifdef PROFILE_TCP_TRANSPORT
#if defined(UCLIENT_EXTERNAL_TCP)
#include <uxr/client/profile/transport/ip/tcp/tcp_transport_external.h>
#endif
#endif //PROFILE_TCP_TRANSPORT

#ifdef PROFILE_SERIAL_TRANSPORT
#if defined(UCLIENT_EXTERNAL_SERIAL)
#include <uxr/client/profile/transport/serial/serial_transport_external.h>
#endif
#endif //PROFILE_SERIAL_TRANSPORT