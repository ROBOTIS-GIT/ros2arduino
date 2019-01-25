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

#include <ucdr/common.h>

#include <string.h>

#if __BIG_ENDIAN__
    const ucdrEndianness UCDR_MACHINE_ENDIANNESS = UCDR_BIG_ENDIANNESS;
#else
    const ucdrEndianness UCDR_MACHINE_ENDIANNESS = UCDR_LITTLE_ENDIANNESS;
#endif

// -------------------------------------------------------------------
//                   INTERNAL UTIL IMPLEMENTATIONS
// -------------------------------------------------------------------
bool ucdr_check_buffer(ucdrBuffer* mb, const uint32_t bytes)
{
    if(!mb->error)
    {
        bool fit = mb->iterator + bytes <= mb->final;
        if(!fit)
        {
            mb->error = true;
        }
    }

    return !mb->error;
}

// -------------------------------------------------------------------
//                       PUBLIC IMPLEMENTATION
// -------------------------------------------------------------------
void ucdr_init_buffer(ucdrBuffer* mb, uint8_t* data, const uint32_t size)
{
    ucdr_init_buffer_offset(mb, data, size, 0U);
}

void ucdr_init_buffer_offset(ucdrBuffer* mb, uint8_t* data, const uint32_t size, uint32_t offset)
{
    ucdr_init_buffer_offset_endian(mb, data, size, offset, UCDR_MACHINE_ENDIANNESS);
}

void ucdr_init_buffer_offset_endian(ucdrBuffer* mb, uint8_t* data, const uint32_t size, uint32_t offset, ucdrEndianness endianness)
{
    mb->init = data;
    mb->final = mb->init + size;
    mb->iterator = mb->init + offset;
    mb->last_data_size = 0U;
    mb->endianness = endianness;
    mb->error = false;
}


void ucdr_copy_buffer(ucdrBuffer* mb_dest, const ucdrBuffer* mb_source)
{
    memcpy(mb_dest, mb_source, sizeof(ucdrBuffer));
}

void ucdr_reset_buffer(ucdrBuffer* mb)
{
    ucdr_reset_buffer_offset(mb, 0U);
}

void ucdr_reset_buffer_offset(ucdrBuffer* mb, const uint32_t offset)
{
    mb->iterator = mb->init + offset;
    mb->last_data_size = 0U;
    mb->error = false;
}

void ucdr_align_to(ucdrBuffer* mb, const uint32_t size)
{
    uint32_t offset = ucdr_buffer_alignment(mb, size);
    mb->iterator += offset;
    if(mb->iterator > mb->final)
    {
        mb->iterator = mb->final;
    }

    mb->last_data_size = size;
}

uint32_t ucdr_alignment(uint32_t current_alignment, const uint32_t data_size)
{
    return ((data_size - (current_alignment % data_size)) & (data_size - 1));
}

uint32_t ucdr_buffer_alignment(const ucdrBuffer* mb, const uint32_t data_size)
{
    if(data_size > mb->last_data_size)
    {
        return (data_size - ((uint32_t)(mb->iterator - mb->init) % data_size)) & (data_size - 1);
    }

    return 0;
}

size_t ucdr_buffer_size(const ucdrBuffer* mb)
{
    return (size_t)(mb->final - mb->init);
}

size_t ucdr_buffer_length(const ucdrBuffer* mb)
{
    return (size_t)(mb->iterator - mb->init);
}

size_t ucdr_buffer_remaining(const ucdrBuffer* mb)
{
    return (size_t)(mb->final - mb->iterator);
}

ucdrEndianness ucdr_buffer_endianness(const ucdrBuffer* mb)
{
    return mb->endianness;
}

bool ucdr_buffer_has_error(const ucdrBuffer* mb)
{
    return mb->error;
}
