// Copyright 2017 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef _SRC_C_CORE_SESSION_STREAM_OUTPUT_RELIABLE_STREAM_INTERNAL_H_
#define _SRC_C_CORE_SESSION_STREAM_OUTPUT_RELIABLE_STREAM_INTERNAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/core/session/stream/output_reliable_stream.h>
#include <uxr/client/core/session/stream/seq_num.h>

#include <stddef.h>
#include <stdbool.h>

void uxr_init_output_reliable_stream(uxrOutputReliableStream* stream, uint8_t* buffer, size_t size, uint16_t history, uint8_t header_offset);
void uxr_reset_output_reliable_stream(uxrOutputReliableStream* stream);
bool uxr_prepare_reliable_buffer_to_write(uxrOutputReliableStream* stream, size_t size, struct ucdrBuffer* mb);
bool uxr_prepare_next_reliable_buffer_to_send(uxrOutputReliableStream* stream, uint8_t** buffer, size_t* length, uxrSeqNum* seq_num);

bool uxr_update_output_stream_heartbeat_timestamp(uxrOutputReliableStream* stream, int64_t current_timestamp);
uxrSeqNum uxr_begin_output_nack_buffer_it(const uxrOutputReliableStream* stream);
bool uxr_next_reliable_nack_buffer_to_send(uxrOutputReliableStream* stream, uint8_t** buffer, size_t *length, uxrSeqNum* seq_num_it);
void uxr_buffer_heartbeat(const uxrOutputReliableStream* stream, struct ucdrBuffer* mb);
void uxr_read_acknack(uxrOutputReliableStream* stream, struct ucdrBuffer* payload);

bool uxr_is_output_reliable_stream_busy(const uxrOutputReliableStream* stream);

size_t uxr_get_output_buffer_length(uint8_t* buffer);
void uxr_set_output_buffer_length(uint8_t* buffer, size_t length);
uint8_t* uxr_get_output_buffer(const uxrOutputReliableStream* stream, size_t history_pos);
size_t uxr_get_output_buffer_size(const uxrOutputReliableStream* stream);

#ifdef __cplusplus
}
#endif

#endif // _SRC_C_CORE_SESSION_STREAM_OUTPUT_RELIABLE_STREAM_INTERNAL_H_
