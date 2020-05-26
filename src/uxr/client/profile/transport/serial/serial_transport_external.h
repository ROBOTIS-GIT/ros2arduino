#ifndef UXR_CLIENT_PROFILE_TRANSPORT_SERIAL_EXTERNAL_H_
#define UXR_CLIENT_PROFILE_TRANSPORT_SERIAL_EXTERNAL_H_

#include <uxr/client/config.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct uxrSerialPlatform
{
  void *serial_instance;
  uint32_t baudrate;
} uxrSerialPlatform;

#ifdef __cplusplus
}
#endif

#endif // UXR_CLIENT_PROFILE_TRANSPORT_SERIAL_EXTERNAL_H_