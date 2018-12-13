#include "../../../../../include/uxr/client/profile/transport/udp/udp_transport_arduino.h"
#include "udp_transport_internal.h"
//#include <unistd.h>
//#include <sys/types.h>
//#include <string.h>
//#include <errno.h>

bool uxr_init_udp_platform(uxrUDPPlatform* platform, const char* ip, uint16_t port)
{
  bool rv = false;

  (void)(platform);
  (void)(ip);
  (void)(port);

  return rv;
}

bool uxr_close_udp_platform(uxrUDPPlatform* platform)
{
  bool rv = false;

  (void)(platform);

  return rv;
}

size_t uxr_write_udp_data_platform(uxrUDPPlatform* platform, const uint8_t* buf,
    size_t len, uint8_t* errcode)
{
  size_t rv = 0;

  (void)(platform);
  (void)(buf);
  (void)(len);
  (void)(errcode);

  return rv;
}

size_t uxr_read_udp_data_platform(uxrUDPPlatform* platform, uint8_t* buf,
    size_t len, int timeout, uint8_t* errcode)
{
  size_t rv = 0;

  (void)(platform);
  (void)(buf);
  (void)(len);
  (void)(timeout);
  (void)(errcode);

  return rv;
}
