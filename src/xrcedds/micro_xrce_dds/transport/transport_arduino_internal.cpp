#include "transport_arduino_internal.h"
#include <Arduino.h>

static Stream *rtps_serial;

bool uxr_initSerialArduino(void *serial_device)
{
  bool rv = true;

  rtps_serial = (Stream*) serial_device;

  return rv;
}

bool uxr_closeSerialArduino()
{
  bool rv = true;

  return rv;
}

size_t uxr_writeSerialDataArduino(uint8_t* buf, size_t len)
{
  size_t tx_len = 0;

  tx_len = rtps_serial->write(buf, len);
  rtps_serial->flush();

  return tx_len;
}

size_t uxr_readSerialDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;

  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    rv = rtps_serial->available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    for (size_t i = 0; i < rv; i++)
    {
      buf[i] = rtps_serial->read();
    }
  }
  
  return rv;
}