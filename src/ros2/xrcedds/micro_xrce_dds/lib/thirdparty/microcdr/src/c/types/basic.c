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

#include <ucdr/types/basic.h>
#include "../common_internals.h"
#include "basic_internals.h"

#include <string.h>


// -------------------------------------------------------------------
//                INTERNAL SERIALIZATION IMPLEMENTATION
// -------------------------------------------------------------------

bool ucdr_serialize_byte_1(ucdrBuffer* mb, const uint8_t* byte)
{
    uint32_t data_size = sizeof(uint8_t);
    if(ucdr_check_buffer(mb, data_size))
    {
        *mb->iterator = *byte;

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

bool ucdr_serialize_byte_2(ucdrBuffer* mb, const ucdrEndianness endianness, const uint16_t* bytes)
{
    uint32_t data_size = sizeof(uint16_t);
    uint32_t alignment = ucdr_buffer_alignment(mb, data_size);

    if(ucdr_check_buffer(mb, alignment + data_size))
    {
        mb->iterator += alignment;

        if(UCDR_MACHINE_ENDIANNESS == endianness)
        {
            memcpy(mb->iterator, bytes, data_size);
        }
        else
        {
            uint8_t* bytes_pointer = (uint8_t*)bytes;
            *mb->iterator = *(bytes_pointer + 1);
            *(mb->iterator + 1) = *bytes_pointer;
        }

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

bool ucdr_serialize_byte_4(ucdrBuffer* mb, const ucdrEndianness endianness, const uint32_t* bytes)
{
    uint32_t data_size = sizeof(uint32_t);
    uint32_t alignment = ucdr_buffer_alignment(mb, data_size);

    if(ucdr_check_buffer(mb, alignment + data_size))
    {
        mb->iterator += alignment;

        if(UCDR_MACHINE_ENDIANNESS == endianness)
        {
            memcpy(mb->iterator, bytes, data_size);
        }
        else
        {
            uint8_t* bytes_pointer = (uint8_t*)bytes;
            *mb->iterator = *(bytes_pointer + 3);
            *(mb->iterator + 1) = *(bytes_pointer + 2);
            *(mb->iterator + 2) = *(bytes_pointer + 1);
            *(mb->iterator + 3) = *bytes_pointer;
        }

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

bool ucdr_serialize_byte_8(ucdrBuffer* mb, const ucdrEndianness endianness, const uint64_t* bytes)
{
    uint32_t data_size = sizeof(uint64_t);
    uint32_t alignment = ucdr_buffer_alignment(mb, data_size);

    if(ucdr_check_buffer(mb, alignment + data_size))
    {
        mb->iterator += alignment;

        if(UCDR_MACHINE_ENDIANNESS == endianness)
        {
            memcpy(mb->iterator, bytes, data_size);
        }
        else
        {
            uint8_t* bytes_pointer = (uint8_t*)bytes;
            *mb->iterator = *(bytes_pointer + 7);
            *(mb->iterator + 1) = *(bytes_pointer + 6);
            *(mb->iterator + 2) = *(bytes_pointer + 5);
            *(mb->iterator + 3) = *(bytes_pointer + 4);
            *(mb->iterator + 4) = *(bytes_pointer + 3);
            *(mb->iterator + 5) = *(bytes_pointer + 2);
            *(mb->iterator + 6) = *(bytes_pointer + 1);
            *(mb->iterator + 7) = *bytes_pointer;
        }

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

bool ucdr_deserialize_byte_1(ucdrBuffer* mb, uint8_t* byte)
{
    uint32_t data_size = sizeof(uint8_t);
    if(ucdr_check_buffer(mb, data_size))
    {
        *byte = *mb->iterator;

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

bool ucdr_deserialize_byte_2(ucdrBuffer* mb, const ucdrEndianness endianness, uint16_t* bytes)
{
    uint32_t data_size = sizeof(uint16_t);
    uint32_t alignment = ucdr_buffer_alignment(mb, data_size);

    if(ucdr_check_buffer(mb, alignment + data_size))
    {
        mb->iterator += alignment;

        if(UCDR_MACHINE_ENDIANNESS == endianness)
        {
            memcpy(bytes, mb->iterator, data_size);
        }
        else
        {
            uint8_t* bytes_pointer = (uint8_t*)bytes;
            *bytes_pointer = *(mb->iterator + 1);
            *(bytes_pointer + 1) = *mb->iterator ;
        }

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

bool ucdr_deserialize_byte_4(ucdrBuffer* mb, const ucdrEndianness endianness, uint32_t* bytes)
{
    uint32_t data_size = sizeof(uint32_t);
    uint32_t alignment = ucdr_buffer_alignment(mb, data_size);

    if(ucdr_check_buffer(mb, alignment + data_size))
    {
        mb->iterator += alignment;

        if(UCDR_MACHINE_ENDIANNESS == endianness)
        {
            memcpy(bytes, mb->iterator, data_size);
        }
        else
        {
            uint8_t* bytes_pointer = (uint8_t*)bytes;
            *bytes_pointer = *(mb->iterator + 3);
            *(bytes_pointer + 1) = *(mb->iterator + 2);
            *(bytes_pointer + 2) = *(mb->iterator + 1);
            *(bytes_pointer + 3) = *mb->iterator;
        }

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

bool ucdr_deserialize_byte_8(ucdrBuffer* mb, const ucdrEndianness endianness, uint64_t* bytes)
{
    uint32_t data_size = sizeof(uint64_t);
    uint32_t alignment = ucdr_buffer_alignment(mb, data_size);

    if(ucdr_check_buffer(mb, alignment + data_size))
    {
        mb->iterator += alignment;

        if(UCDR_MACHINE_ENDIANNESS == endianness)
        {
            memcpy(bytes, mb->iterator, data_size);
        }
        else
        {
            uint8_t* bytes_pointer = (uint8_t*)bytes;
            *bytes_pointer = *(mb->iterator + 7);
            *(bytes_pointer + 1) = *(mb->iterator + 6);
            *(bytes_pointer + 2) = *(mb->iterator + 5);
            *(bytes_pointer + 3) = *(mb->iterator + 4);
            *(bytes_pointer + 4) = *(mb->iterator + 3);
            *(bytes_pointer + 5) = *(mb->iterator + 2);
            *(bytes_pointer + 6) = *(mb->iterator + 1);
            *(bytes_pointer + 7) = *mb->iterator;
        }

        mb->iterator += data_size;
        mb->last_data_size = data_size;
    }
    return !mb->error;
}

// -------------------------------------------------------------------
//                  PUBLIC SERIALIZATION IMPLEMENTATION
// -------------------------------------------------------------------

bool ucdr_serialize_char(ucdrBuffer* mb, const char value)
{
    return ucdr_serialize_byte_1(mb, (uint8_t*)&value);
}

bool ucdr_serialize_bool(ucdrBuffer* mb, const bool value)
{
    return ucdr_serialize_byte_1(mb, (uint8_t*)&value);
}

bool ucdr_serialize_uint8_t(ucdrBuffer* mb, const uint8_t value)
{
    return ucdr_serialize_byte_1(mb, &value);
}

bool ucdr_serialize_uint16_t(ucdrBuffer* mb, const uint16_t value)
{
    return ucdr_serialize_byte_2(mb, mb->endianness, &value);
}

bool ucdr_serialize_uint32_t(ucdrBuffer* mb, const uint32_t value)
{
    return ucdr_serialize_byte_4(mb, mb->endianness, &value);
}

bool ucdr_serialize_uint64_t(ucdrBuffer* mb, const uint64_t value)
{
    return ucdr_serialize_byte_8(mb, mb->endianness, &value);
}

bool ucdr_serialize_int8_t(ucdrBuffer* mb, const int8_t value)
{
    return ucdr_serialize_byte_1(mb, (uint8_t*)&value);
}

bool ucdr_serialize_int16_t(ucdrBuffer* mb, const int16_t value)
{
    return ucdr_serialize_byte_2(mb, mb->endianness, (uint16_t*)&value);
}

bool ucdr_serialize_int32_t(ucdrBuffer* mb, const int32_t value)
{
    return ucdr_serialize_byte_4(mb, mb->endianness, (uint32_t*)&value);
}

bool ucdr_serialize_int64_t(ucdrBuffer* mb, const int64_t value)
{
    return ucdr_serialize_byte_8(mb, mb->endianness, (uint64_t*)&value);
}

bool ucdr_serialize_float(ucdrBuffer* mb, const float value)
{
    return ucdr_serialize_byte_4(mb, mb->endianness, (uint32_t*)&value);
}

bool ucdr_serialize_double(ucdrBuffer* mb, const double value)
{
    return ucdr_serialize_byte_8(mb, mb->endianness, (uint64_t*)&value);
}

bool ucdr_deserialize_char(ucdrBuffer* mb, char* value)
{
    return ucdr_deserialize_byte_1(mb, (uint8_t*)value);
}

bool ucdr_deserialize_bool(ucdrBuffer* mb, bool* value)
{
    return ucdr_deserialize_byte_1(mb, (uint8_t*)value);
}

bool ucdr_deserialize_uint8_t(ucdrBuffer* mb, uint8_t* value)
{
    return ucdr_deserialize_byte_1(mb, value);
}

bool ucdr_deserialize_uint16_t(ucdrBuffer* mb, uint16_t* value)
{
    return ucdr_deserialize_byte_2(mb, mb->endianness, value);
}

bool ucdr_deserialize_uint32_t(ucdrBuffer* mb, uint32_t* value)
{
    return ucdr_deserialize_byte_4(mb, mb->endianness, value);
}

bool ucdr_deserialize_uint64_t(ucdrBuffer* mb, uint64_t* value)
{
    return ucdr_deserialize_byte_8(mb, mb->endianness, value);
}

bool ucdr_deserialize_int8_t(ucdrBuffer* mb, int8_t* value)
{
    return ucdr_deserialize_byte_1(mb, (uint8_t*)value);
}

bool ucdr_deserialize_int16_t(ucdrBuffer* mb, int16_t* value)
{
    return ucdr_deserialize_byte_2(mb, mb->endianness, (uint16_t*)value);
}

bool ucdr_deserialize_int32_t(ucdrBuffer* mb, int32_t* value)
{
    return ucdr_deserialize_byte_4(mb, mb->endianness, (uint32_t*)value);
}

bool ucdr_deserialize_int64_t(ucdrBuffer* mb, int64_t* value)
{
    return ucdr_deserialize_byte_8(mb, mb->endianness, (uint64_t*)value);
}

bool ucdr_deserialize_float(ucdrBuffer* mb, float* value)
{
    return ucdr_deserialize_byte_4(mb, mb->endianness, (uint32_t*)value);
}

bool ucdr_deserialize_double(ucdrBuffer* mb, double* value)
{
    return ucdr_deserialize_byte_8(mb, mb->endianness, (uint64_t*)value);
}

bool ucdr_serialize_endian_uint16_t(ucdrBuffer* mb, const ucdrEndianness endianness, const uint16_t value)
{
    return ucdr_serialize_byte_2(mb, endianness, &value);
}

bool ucdr_serialize_endian_uint32_t(ucdrBuffer* mb, const ucdrEndianness endianness, const uint32_t value)
{
    return ucdr_serialize_byte_4(mb, endianness, &value);
}

bool ucdr_serialize_endian_uint64_t(ucdrBuffer* mb, const ucdrEndianness endianness, const uint64_t value)
{
    return ucdr_serialize_byte_8(mb, endianness, &value);
}

bool ucdr_serialize_endian_int16_t(ucdrBuffer* mb, const ucdrEndianness endianness, const int16_t value)
{
    return ucdr_serialize_byte_2(mb, endianness, (uint16_t*)&value);
}

bool ucdr_serialize_endian_int32_t(ucdrBuffer* mb, const ucdrEndianness endianness, const int32_t value)
{
    return ucdr_serialize_byte_4(mb, endianness, (uint32_t*)&value);
}

bool ucdr_serialize_endian_int64_t(ucdrBuffer* mb, const ucdrEndianness endianness, const int64_t value)
{
    return ucdr_serialize_byte_8(mb, endianness, (uint64_t*)&value);
}

bool ucdr_serialize_endian_float(ucdrBuffer* mb, const ucdrEndianness endianness, const float value)
{
    return ucdr_serialize_byte_4(mb, endianness, (uint32_t*)&value);
}

bool ucdr_serialize_endian_double(ucdrBuffer* mb, const ucdrEndianness endianness, const double value)
{
    return ucdr_serialize_byte_8(mb, endianness, (uint64_t*)&value);
}

bool ucdr_deserialize_endian_uint16_t(ucdrBuffer* mb, const ucdrEndianness endianness, uint16_t* value)
{
    return ucdr_deserialize_byte_2(mb, endianness, value);
}

bool ucdr_deserialize_endian_uint32_t(ucdrBuffer* mb, const ucdrEndianness endianness, uint32_t* value)
{
    return ucdr_deserialize_byte_4(mb, endianness, value);
}

bool ucdr_deserialize_endian_uint64_t(ucdrBuffer* mb, const ucdrEndianness endianness, uint64_t* value)
{
    return ucdr_deserialize_byte_8(mb, endianness, value);
}

bool ucdr_deserialize_endian_int16_t(ucdrBuffer* mb, const ucdrEndianness endianness, int16_t* value)
{
    return ucdr_deserialize_byte_2(mb, endianness, (uint16_t*)value);
}

bool ucdr_deserialize_endian_int32_t(ucdrBuffer* mb, const ucdrEndianness endianness, int32_t* value)
{
    return ucdr_deserialize_byte_4(mb, endianness, (uint32_t*)value);
}

bool ucdr_deserialize_endian_int64_t(ucdrBuffer* mb, const ucdrEndianness endianness, int64_t* value)
{
    return ucdr_deserialize_byte_8(mb, endianness, (uint64_t*)value);
}

bool ucdr_deserialize_endian_float(ucdrBuffer* mb, const ucdrEndianness endianness, float* value)
{
    return ucdr_deserialize_byte_4(mb, endianness, (uint32_t*)value);
}

bool ucdr_deserialize_endian_double(ucdrBuffer* mb, const ucdrEndianness endianness, double* value)
{
    return ucdr_deserialize_byte_8(mb, endianness, (uint64_t*)value);
}

