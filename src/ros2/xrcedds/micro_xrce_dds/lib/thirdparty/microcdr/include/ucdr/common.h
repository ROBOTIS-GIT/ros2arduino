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

#ifndef _MICROCDR_COMMON_H_
#define _MICROCDR_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ucdr/dll.h>
#include <ucdr/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum ucdrEndianness {
    UCDR_BIG_ENDIANNESS,
    UCDR_LITTLE_ENDIANNESS

} ucdrEndianness;

typedef struct ucdrBuffer
{
    uint8_t *init;
    uint8_t *final;
    uint8_t *iterator;

    ucdrEndianness endianness;
    uint32_t last_data_size;

    bool error;

} ucdrBuffer;

UCDRDLLAPI extern const ucdrEndianness UCDR_MACHINE_ENDIANNESS;

// ------------------------------------------------
//              Main library functions
// ------------------------------------------------
UCDRDLLAPI void ucdr_init_buffer               (ucdrBuffer* mb, uint8_t* data, const uint32_t size);
UCDRDLLAPI void ucdr_init_buffer_offset        (ucdrBuffer* mb, uint8_t* data, const uint32_t size, uint32_t offset);
UCDRDLLAPI void ucdr_init_buffer_offset_endian (ucdrBuffer* mb, uint8_t* data, const uint32_t size, uint32_t offset, ucdrEndianness endianness);
UCDRDLLAPI void ucdr_copy_buffer               (ucdrBuffer* mb_dest, const ucdrBuffer* mb_source);

UCDRDLLAPI void ucdr_reset_buffer        (ucdrBuffer* mb);
UCDRDLLAPI void ucdr_reset_buffer_offset (ucdrBuffer* mb, const uint32_t offset);

UCDRDLLAPI void     ucdr_align_to              (ucdrBuffer* mb, const uint32_t alignment);
UCDRDLLAPI uint32_t ucdr_alignment             (uint32_t buffer_position, const uint32_t data_size);
UCDRDLLAPI uint32_t ucdr_buffer_alignment(const ucdrBuffer* mb, const uint32_t data_size);

UCDRDLLAPI size_t     ucdr_buffer_size      (const ucdrBuffer* mb);
UCDRDLLAPI size_t     ucdr_buffer_length    (const ucdrBuffer* mb);
UCDRDLLAPI size_t     ucdr_buffer_remaining (const ucdrBuffer* mb);
UCDRDLLAPI ucdrEndianness ucdr_buffer_endianness(const ucdrBuffer* mb);
UCDRDLLAPI bool       ucdr_buffer_has_error (const ucdrBuffer* mb);

#ifdef __cplusplus
}
#endif

#endif //_MICROCDR_COMMON_H_
