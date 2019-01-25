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

#ifndef _UXR_CLIENT_UDP_TRANSPORT_H_
#define _UXR_CLIENT_UDP_TRANSPORT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/core/communication/communication.h>
#include <uxr/client/config.h>
#include <uxr/client/dll.h>

struct uxrUDPPlatform;

typedef struct uxrUDPTransport
{
    uint8_t buffer[UXR_CONFIG_UDP_TRANSPORT_MTU];
    uxrCommunication comm;
    struct uxrUDPPlatform* platform;

} uxrUDPTransport;

UXRDLLAPI bool uxr_init_udp_transport(uxrUDPTransport* transport,
                                      struct uxrUDPPlatform* platform,
                                      const char* ip,
                                      uint16_t port);

UXRDLLAPI bool uxr_close_udp_transport(uxrUDPTransport* transport);


#ifdef __cplusplus
}
#endif

#endif //_UXR_CLIENT_UDP_TRANSPORT_H_
