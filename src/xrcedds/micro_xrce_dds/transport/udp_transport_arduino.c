#include "udp_transport_arduino.h"
#include "../lib/src/c/profile/transport/udp/udp_transport_internal.h"

#include "transport_arduino_internal.h"
//#include <unistd.h>
//#include <sys/types.h>
//#include <string.h>
//#include <errno.h>

bool uxr_init_udp_platform(uxrUDPPlatform* platform, const char* ip, uint16_t port)
{
  (void)(platform);

  return uxr_initUdpArduino(ip, port);
}

bool uxr_close_udp_platform(uxrUDPPlatform* platform)
{
  (void)(platform);

  return uxr_closeUdpArduino();
}

size_t uxr_write_udp_data_platform(uxrUDPPlatform* platform, const uint8_t* buf,
    size_t len, uint8_t* errcode)
{
  size_t rv = 0;

  (void)(platform);
  (void)(errcode);
  rv = uxr_writeUdpDataArduino(buf, len);

  return rv;
}

size_t uxr_read_udp_data_platform(uxrUDPPlatform* platform, uint8_t* buf,
    size_t len, int timeout, uint8_t* errcode)
{
  size_t rv = 0;

  (void)(platform);
  (void)(errcode);
  rv = uxr_readUdpDataArduino(buf, len, timeout);

  return rv;
}
