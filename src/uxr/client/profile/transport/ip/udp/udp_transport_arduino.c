#include "udp_transport_external.h"
#include <ros2/xrcedds/micro_xrce_dds/lib/src/c/profile/transport/ip/udp/udp_transport_internal.h>

#include "../../transport_arduino_internal.h"
#include <stdlib.h>

bool uxr_init_udp_platform(struct uxrUDPPlatform* platform, uxrIpProtocol ip_protocol,
  const char* ip, const char* port)
{
  char* str_ptr;
  if(ip_protocol != UXR_IPv4){
    return false;
  }
  return uxr_initUdpArduino(platform->udp_instance, ip, (uint16_t)strtol(port, &str_ptr, 10));
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
