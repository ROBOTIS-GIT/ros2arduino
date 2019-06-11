#include <Arduino.h>

extern "C" uint32_t dds_getMilliseconds(void)
{
  return millis();
}

extern "C" uint32_t dds_getMicroseconds(void)
{
  return micros();
}