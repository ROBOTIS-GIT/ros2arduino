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

#ifndef _SRC_C_CORE_SESSION_STREAM_COMMON_RELIABLE_STREAM_INTERNAL_H_
#define _SRC_C_CORE_SESSION_STREAM_COMMON_RELIABLE_STREAM_INTERNAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t length_t;
#define INTERNAL_RELIABLE_BUFFER_OFFSET sizeof(length_t)


static inline size_t uxr_get_reliable_buffer_length(uint8_t* buffer)
{
    length_t length;
    memcpy(&length, buffer - INTERNAL_RELIABLE_BUFFER_OFFSET, INTERNAL_RELIABLE_BUFFER_OFFSET);
    return (size_t)length;
}

static inline void uxr_set_reliable_buffer_length(uint8_t* buffer, size_t length)
{
    length_t internal_length = (length_t)length;
    memcpy(buffer - INTERNAL_RELIABLE_BUFFER_OFFSET, &internal_length, INTERNAL_RELIABLE_BUFFER_OFFSET);
}

static inline uint8_t* uxr_get_reliable_buffer(uint8_t* buffer, size_t size, size_t history, size_t history_pos)
{
    return buffer + history_pos * (size / history) + INTERNAL_RELIABLE_BUFFER_OFFSET;
}

static inline size_t uxr_get_reliable_buffer_size(size_t size, size_t history)
{
    return size / history - INTERNAL_RELIABLE_BUFFER_OFFSET;
}


#ifdef __cplusplus
}
#endif

#endif // _SRC_C_CORE_SESSION_STREAM_COMMON_RELIABLE_STREAM_INTERNAL_H_
