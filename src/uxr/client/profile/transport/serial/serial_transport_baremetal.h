#ifndef _UXR_CLIENT_SERIAL_TRANSPORT_BAREMETAL_H_
#define _UXR_CLIENT_SERIAL_TRANSPORT_BAREMETAL_H_

#include <uxr/client/config.h>
#if defined(PLATFORM_NAME_BAREMETAL)

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

#endif //PLATFORM_NAME_BAREMETAL

#endif //_UXR_CLIENT_SERIAL_TRANSPORT_BAREMETAL_H_