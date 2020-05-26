#include "tcp_transport_external.h"
#include <ros2/xrcedds/micro_xrce_dds/lib/src/c/profile/transport/ip/tcp/tcp_transport_internal.h>

#include "../../transport_arduino_internal.h"
#include <stdlib.h>

bool uxr_init_tcp_platform(struct uxrTCPPlatform* platform, uxrIpProtocol ip_protocol,
  const char* ip, const char* port)
{
  char* str_ptr;
  if(ip_protocol != UXR_IPv4){
    return false;
  }
  return uxr_connectTcpArduino(platform->client_instance, ip, (uint16_t)strtol(port, &str_ptr, 10));
}

bool uxr_close_tcp_platform(struct uxrTCPPlatform* platform)
{
  (void)(platform);

  return uxr_closeTcpArduino();
}

size_t uxr_write_tcp_data_platform(struct uxrTCPPlatform* platform,
    const uint8_t* buf, size_t len, uint8_t* errcode)
{
  size_t rv = 0;

  (void)(platform);
  (void)(errcode);

  rv = uxr_writeTcpDataArduino(buf, len);

  return rv;
}

size_t uxr_read_tcp_data_platform(struct uxrTCPPlatform* platform, uint8_t* buf,
    size_t len, int timeout, uint8_t* errcode)
{
  size_t rv = 0;

  (void)(platform);
  (void)(errcode);

  rv = uxr_readTcpDataArduino(buf, len, timeout);

  return rv;
}

void uxr_disconnect_tcp_platform(struct uxrTCPPlatform* platform)
{
  (void)(platform);
  uxr_disconnectTcpArduino();
}

