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

#ifndef _SRC_C_CLIENT_UDP_TRANSPORT_DATAGRAM_INTERNAL_H_
#define _SRC_C_CLIENT_UDP_TRANSPORT_DATAGRAM_INTERNAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/dll.h>
#include <uxr/client/config.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if defined(PLATFORM_NAME_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#elif defined(PLATFORM_NAME_WINDOWS)
#endif


#define UXR_UDP_TRANSPORT_MTU_DATAGRAM 52 //Adjusted to the minimun necessary buffer for discovery messages.

typedef struct uxrUDPTransportDatagram
{
#if defined(PLATFORM_NAME_LINUX)
    uint8_t buffer[UXR_UDP_TRANSPORT_MTU_DATAGRAM];
    struct pollfd poll_fd;
#elif defined(PLATFORM_NAME_WINDOWS)
    #error "Windows platform not implemented"
#endif

} uxrUDPTransportDatagram;

bool uxr_init_udp_transport_datagram(struct uxrUDPTransportDatagram* transport);
bool uxr_udp_send_datagram_to(struct uxrUDPTransportDatagram* transport, const uint8_t* buf, size_t len, const char* ip, uint16_t port);
bool uxr_udp_recv_datagram(struct uxrUDPTransportDatagram* transport, uint8_t** buf, size_t* len, int timeout);
void uxr_bytes_to_ip(const uint8_t* bytes, char* ip);

#ifdef __cplusplus
}
#endif

#endif //_SRC_C_CLIENT_UDP_TRANSPORT_DATAGRAM_INTERNAL_H_
