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

#ifndef _UXR_CLIENT_CORE_COMMUNICATION_SERIAL_PROTOCOL_H_
#define _UXR_CLIENT_CORE_COMMUNICATION_SERIAL_PROTOCOL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define UXR_FRAMING_BEGIN_FLAG 0x7E
#define UXR_FRAMING_ESC_FLAG 0x7D
#define UXR_FRAMING_XOR_FLAG 0x20

typedef enum uxrSerialInputState
{
    UXR_SERIAL_UNINITIALIZED,
    UXR_SERIAL_READING_SRC_ADDR,
    UXR_SERIAL_READING_DST_ADDR,
    UXR_SERIAL_READING_LEN_LSB,
    UXR_SERIAL_READING_LEN_MSB,
    UXR_SERIAL_READING_PAYLOAD,
    UXR_SERIAL_READING_CRC_LSB,
    UXR_SERIAL_READING_CRC_MSB,

} uxrSerialInputState;

typedef struct uxrSerialIO
{
    uxrSerialInputState state;
    uint8_t local_addr;
    uint8_t rb[42];
    uint8_t rb_head;
    uint8_t rb_tail;
    uint8_t src_addr;
    uint16_t msg_len;
    uint16_t msg_pos;
    uint16_t msg_crc;
    uint16_t cmp_crc;
    uint8_t wb[42];
    uint8_t wb_pos;

} uxrSerialIO;

void uxr_init_serial_io(uxrSerialIO* serial_io, uint8_t local_addr);

void update_crc(uint16_t* crc, const uint8_t data);
bool get_next_octet(uxrSerialIO* serial_io, uint8_t* octet);
bool add_next_octet(uxrSerialIO* serial_io, uint8_t octet);

struct uxrSerialPlatform;
typedef size_t (*uxr_write_cb)(struct uxrSerialPlatform*, uint8_t*, size_t, uint8_t*);
typedef size_t (*uxr_read_cb)(struct uxrSerialPlatform*, uint8_t*, size_t, int, uint8_t*);

size_t uxr_write_serial_msg(uxrSerialIO* serial_io,
                            uxr_write_cb write_cb,
                            void* cb_arg,
                            const uint8_t* buf,
                            size_t len,
                            uint8_t remote_addr,
                            uint8_t* errcode);
size_t uxr_read_serial_msg(uxrSerialIO* serial_io,
                           uxr_read_cb read_cb,
                           void* cb_arg,
                           uint8_t* buf,
                           size_t len,
                           uint8_t* remote_addr,
                           int timeout,
                           uint8_t* errcode);

#ifdef __cplusplus
}
#endif

#endif //_UXR_CLIENT_CORE_COMMUNICATION_SERIAL_PROTOCOL_H_
