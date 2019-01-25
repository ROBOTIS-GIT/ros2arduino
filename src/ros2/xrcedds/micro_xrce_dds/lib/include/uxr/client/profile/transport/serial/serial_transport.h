// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _UXR_CLIENT_SERIAL_TRANSPORT_H_
#define _UXR_CLIENT_SERIAL_TRANSPORT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/core/communication/communication.h>
#include <uxr/client/core/communication/serial_protocol.h>
#include <uxr/client/config.h>
#include <uxr/client/dll.h>

struct uxrSerialPlatform;

typedef struct uxrSerialTransport
{
    uint8_t buffer[UXR_CONFIG_SERIAL_TRANSPORT_MTU];
    uxrSerialIO serial_io;
    uint8_t remote_addr;
    uxrCommunication comm;
    struct uxrSerialPlatform* platform;

} uxrSerialTransport;


UXRDLLAPI bool uxr_init_serial_transport(uxrSerialTransport* transport,
                                         struct uxrSerialPlatform* platform,
                                         const int fd,
                                         uint8_t remote_addr,
                                         uint8_t local_addr);

UXRDLLAPI bool uxr_close_serial_transport(uxrSerialTransport* transport);

#ifdef __cplusplus
}
#endif

#endif //_UXR_CLIENT_SERIAL_TRANSPORT_H_
