# ros2xrcedds

This is a library for communicating with ros2 and XRCEDDS.

It is used in ros2arduino, and is an intermediate library created for easy adaptation to other baremetal platforms.

Currently the only free XRCEDDS library available is MicroXRCEDDS.

It is open source and does not guarantee it. For those who want to use it, briefly write down how to create a library for different platforms based on this library as shown below.

<br>

## UXR related usage

### Added user config file (.h file)
MicroXRCEDDS includes the following config.h file.
```c
#include <ucdr/config.h>
#include <uxr/client/config.h>
```
Therefore, it is necessary to create this file. The format of this file(config.h.in) exists in the following location.
```bash
xrcedds/micro_xrce_dds/lib/thirdparty/microcdr/include/ucdr/
xrcedds/micro_xrce_dds/lib/include/uxr/client/
```
You can create a config.h file in this location, or create it elsewhere (see [ros2arduino](https://github.com/ROBOTIS-GIT/ros2arduino/blob/master/src/uxr/client/config.h))

### Define functions for your system for baremetal.
#### Time
For baremetal, you need to define the function below. 
This function should be used to measure and return milliseconds with the device's timer.
```c
extern "C" uint32_t dds_getMilliseconds (void);
```
For example, in the case of ros2arduino, it is [defined](https://github.com/ROBOTIS-GIT/ros2arduino/blob/master/src/uxr/xrcedds_time.cpp) as follows.
```cpp
extern "C" uint32_t dds_getMilliseconds (void)
{
  return millis();
}
```

#### Transport
You must define functions for baremetal transport layer.
To do this, you need to define the functions declared in the second group header files(\*_internal) below using the structure in the first group header files(\*_baremetal) below.

- [serial_transport_baremetal.h](https://github.com/ROBOTIS-GIT/ros2xrcedds/blob/master/xrcedds/micro_xrce_dds/lib/include/uxr/client/profile/transport/serial/serial_transport_baremetal.h)
- [udp_transport_baremetal.h](https://github.com/ROBOTIS-GIT/ros2xrcedds/blob/master/xrcedds/micro_xrce_dds/lib/include/uxr/client/profile/transport/udp/udp_transport_baremetal.h)
- [tcp_transport_baremetal.h](https://github.com/ROBOTIS-GIT/ros2xrcedds/blob/master/xrcedds/micro_xrce_dds/lib/include/uxr/client/profile/transport/tcp/tcp_transport_baremetal.h)
<br>

- [serial_transport_internal.h](https://github.com/ROBOTIS-GIT/ros2xrcedds/blob/master/xrcedds/micro_xrce_dds/lib/src/c/profile/transport/serial/serial_transport_internal.h)
- [udp_transport_internal.h](https://github.com/ROBOTIS-GIT/ros2xrcedds/blob/master/xrcedds/micro_xrce_dds/lib/src/c/profile/transport/udp/udp_transport_internal.h)
- [tcp_transport_internal.h](https://github.com/ROBOTIS-GIT/ros2xrcedds/blob/master/xrcedds/micro_xrce_dds/lib/src/c/profile/transport/tcp/tcp_transport_internal.h)

In this regard, please refer to [ros2arduino](https://github.com/ROBOTIS-GIT/ros2arduino/tree/master/src/uxr/client/profile/transport).

