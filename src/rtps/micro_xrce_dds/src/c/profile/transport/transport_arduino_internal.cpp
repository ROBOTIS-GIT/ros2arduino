#include "transport_arduino_internal.h"
#include <Arduino.h>

bool uxr_initSerialArduino(uint32_t baudrate)
{
  bool rv = false;

  Serial.begin(baudrate);
  rv = true;

  return rv;
}

bool uxr_closeSerialArduino()
{
  bool rv = false;

  Serial.end();
  rv = true;

  return rv;
}

size_t uxr_writeSerialDataArduino(uint8_t* buf, size_t len)
{
  return Serial.write(buf, len);
}

size_t uxr_readSerialDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;

  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    rv = Serial.available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    for (size_t i = 0; i < rv; i++)
    {
      buf[i] = Serial.read();
    }
  }
  
  return rv;
}