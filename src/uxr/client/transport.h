#include <ros2/xrcedds/micro_xrce_dds/lib/include/uxr/client/transport.h>


#ifdef PROFILE_UDP_TRANSPORT
#if defined(PLATFORM_NAME_ARDUINO)
#include <uxr/client/profile/transport/udp/udp_transport_arduino.h>
#elif defined(PLATFORM_NAME_BAREMETAL)
#include <uxr/client/profile/transport/udp/udp_transport_baremetal.h>
#endif
#endif //PROFILE_UDP_TRANSPORT

#ifdef PROFILE_TCP_TRANSPORT
#if defined(PLATFORM_NAME_ARDUINO)
#include <uxr/client/profile/transport/tcp/tcp_transport_arduino.h>
#elif defined(PLATFORM_NAME_BAREMETAL)
#include <uxr/client/profile/transport/tcp/tcp_transport_baremetal.h>
#endif
#endif //PROFILE_TCP_TRANSPORT

#ifdef PROFILE_SERIAL_TRANSPORT
#if defined(PLATFORM_NAME_ARDUINO)
#include <uxr/client/profile/transport/serial/serial_transport_arduino.h>
#elif defined(PLATFORM_NAME_BAREMETAL)
#include <uxr/client/profile/transport/serial/serial_transport_baremetal.h>
#endif
#endif //PROFILE_SERIAL_TRANSPORT