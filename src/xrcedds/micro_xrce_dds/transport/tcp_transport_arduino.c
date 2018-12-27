#include "tcp_transport_arduino.h"
#include "../lib/src/c/profile/transport/tcp/tcp_transport_internal.h"

#include "transport_arduino_internal.h"
// #include <unistd.h>
// #include <sys/types.h>
// #include <string.h>

bool uxr_init_tcp_platform(struct uxrTCPPlatform* platform, const char* ip,
    uint16_t port)
{
  return uxr_connectTcpArduino(platform->client_instance, ip, port);
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

