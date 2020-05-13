#include "serial_transport_external.h"
#include <ros2/xrcedds/micro_xrce_dds/lib/src/c/profile/transport/serial/serial_protocol_internal.h>

#include "../transport_arduino_internal.h"

bool uxr_init_serial_platform(struct uxrSerialPlatform* platform, int fd,
    uint8_t remote_addr, uint8_t local_addr)
{
  (void) (fd);
  (void) (remote_addr);
  (void) (local_addr);

  /* Open device */
  return uxr_initSerialArduino(platform->serial_instance);
}

bool uxr_close_serial_platform(struct uxrSerialPlatform* platform)
{
  (void)(platform);

  return uxr_closeSerialArduino();
}

size_t uxr_write_serial_data_platform(uxrSerialPlatform* platform, uint8_t* buf,
    size_t len, uint8_t* errcode)
{
  (void)(platform);

  size_t rv = 0;

  rv = uxr_writeSerialDataArduino(buf, len);
  if (0 != rv)
  {
    *errcode = 0;
  }
  else
  {
    *errcode = 1;
  }
  return rv;
}

size_t uxr_read_serial_data_platform(uxrSerialPlatform* platform, uint8_t* buf,
    size_t len, int timeout, uint8_t* errcode)
{
  (void)(platform);

  size_t rv = 0;

  rv = uxr_readSerialDataArduino(buf, len, timeout);

  if (0 < rv)
  {
    *errcode = 0;
  }
  else
  {
    *errcode = 1;
  }

  return rv;
}

