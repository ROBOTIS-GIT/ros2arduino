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
#include <ucdr/types/array.h>
#include <ucdr/types/sequence.h>

#include "array_internals.h"
#include "sequence_internals.h"

// -------------------------------------------------------------------
//                INTERNAL UTIL IMPLEMENTATION
// -------------------------------------------------------------------
static inline void ucdr_deserialize_sequence_header(ucdrBuffer* mb, ucdrEndianness endianness, uint32_t capacity, uint32_t* size)
{
    ucdr_deserialize_endian_uint32_t(mb, endianness, size);
    if(*size > capacity)
    {
        mb->error = true;
    }
}

// -------------------------------------------------------------------
//                INTERNAL SERIALIZATION IMPLEMENTATION
// -------------------------------------------------------------------
bool ucdr_serialize_sequence_byte_1(ucdrBuffer* mb, ucdrEndianness endianness, const uint8_t* array, const uint32_t size)
{
    ucdr_serialize_endian_uint32_t(mb, endianness, size);
    return ucdr_serialize_array_byte_1(mb, array, size);
}

bool ucdr_serialize_sequence_byte_2(ucdrBuffer* mb, ucdrEndianness endianness, const uint16_t* array, const uint32_t size)
{
    ucdr_serialize_endian_uint32_t(mb, endianness, size);
    return ucdr_serialize_array_byte_2(mb, endianness, array, size);
}

bool ucdr_serialize_sequence_byte_4(ucdrBuffer* mb, ucdrEndianness endianness, const uint32_t* array, const uint32_t size)
{
    ucdr_serialize_endian_uint32_t(mb, endianness, size);
    return ucdr_serialize_array_byte_4(mb, endianness, array, size);
}

bool ucdr_serialize_sequence_byte_8(ucdrBuffer* mb, ucdrEndianness endianness, const uint64_t* array, const uint32_t size)
{
    ucdr_serialize_endian_uint32_t(mb, endianness, size);
    return ucdr_serialize_array_byte_8(mb, endianness, array, size);
}

bool ucdr_deserialize_sequence_byte_1(ucdrBuffer* mb, ucdrEndianness endianness, uint8_t* array, const uint32_t array_capacity, uint32_t* size)
{
    ucdr_deserialize_sequence_header(mb, endianness, array_capacity, size);
    return ucdr_deserialize_array_byte_1(mb, array, *size);
}

bool ucdr_deserialize_sequence_byte_2(ucdrBuffer* mb, ucdrEndianness endianness, uint16_t* array, const uint32_t array_capacity, uint32_t* size)
{
    ucdr_deserialize_sequence_header(mb, endianness, array_capacity, size);
    return ucdr_deserialize_array_byte_2(mb, endianness, array, *size);
}

bool ucdr_deserialize_sequence_byte_4(ucdrBuffer* mb, ucdrEndianness endianness, uint32_t* array, const uint32_t array_capacity, uint32_t* size)
{
    ucdr_deserialize_sequence_header(mb, endianness, array_capacity, size);
    return ucdr_deserialize_array_byte_4(mb, endianness, array, *size);
}

bool ucdr_deserialize_sequence_byte_8(ucdrBuffer* mb, ucdrEndianness endianness, uint64_t* array, const uint32_t array_capacity, uint32_t* size)
{
    ucdr_deserialize_sequence_header(mb, endianness, array_capacity, size);
    return ucdr_deserialize_array_byte_8(mb, endianness, array, *size);
}

// -------------------------------------------------------------------
//              PUBLIC SERIALIZATION IMPLEMENTATIONS
// -------------------------------------------------------------------

bool ucdr_serialize_sequence_char(ucdrBuffer* mb, const char* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_sequence_bool(ucdrBuffer* mb, const bool* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_sequence_uint8_t(ucdrBuffer* mb, const uint8_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_sequence_uint16_t(ucdrBuffer* mb, const uint16_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_2(mb, mb->endianness, (uint16_t*)array, size);
}

bool ucdr_serialize_sequence_uint32_t(ucdrBuffer* mb, const uint32_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_4(mb, mb->endianness, (uint32_t*)array, size);
}

bool ucdr_serialize_sequence_uint64_t(ucdrBuffer* mb, const uint64_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_8(mb, mb->endianness, (uint64_t*)array, size);
}

bool ucdr_serialize_sequence_int8_t(ucdrBuffer* mb, const int8_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_sequence_int16_t(ucdrBuffer* mb, const int16_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_2(mb, mb->endianness, (uint16_t*)array, size);
}

bool ucdr_serialize_sequence_int32_t(ucdrBuffer* mb, const int32_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_4(mb, mb->endianness, (uint32_t*)array, size);
}

bool ucdr_serialize_sequence_int64_t(ucdrBuffer* mb, const int64_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_8(mb, mb->endianness, (uint64_t*)array, size);
}

bool ucdr_serialize_sequence_float(ucdrBuffer* mb, const float* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_4(mb, mb->endianness, (uint32_t*)array, size);
}

bool ucdr_serialize_sequence_double(ucdrBuffer* mb, const double* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_8(mb, mb->endianness, (uint64_t*)array, size);
}

bool ucdr_deserialize_sequence_char(ucdrBuffer* mb, char* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_bool(ucdrBuffer* mb, bool* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_uint8_t(ucdrBuffer* mb, uint8_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_uint16_t(ucdrBuffer* mb, uint16_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_2(mb, mb->endianness, (uint16_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_uint32_t(ucdrBuffer* mb, uint32_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_4(mb, mb->endianness, (uint32_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_uint64_t(ucdrBuffer* mb, uint64_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_8(mb, mb->endianness, (uint64_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_int8_t(ucdrBuffer* mb, int8_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, mb->endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_int16_t(ucdrBuffer* mb, int16_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_2(mb, mb->endianness, (uint16_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_int32_t(ucdrBuffer* mb, int32_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_4(mb, mb->endianness, (uint32_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_int64_t(ucdrBuffer* mb, int64_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_8(mb, mb->endianness, (uint64_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_float(ucdrBuffer* mb, float* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_4(mb, mb->endianness, (uint32_t*)array, array_capacity, size);
}

bool ucdr_deserialize_sequence_double(ucdrBuffer* mb, double* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_8(mb, mb->endianness, (uint64_t*)array, array_capacity, size);
}

bool ucdr_serialize_endian_sequence_char(ucdrBuffer* mb, const ucdrEndianness endianness, const char* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_endian_sequence_bool(ucdrBuffer* mb, const ucdrEndianness endianness, const bool* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_endian_sequence_uint8_t(ucdrBuffer* mb, const ucdrEndianness endianness, const uint8_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_endian_sequence_uint16_t(ucdrBuffer* mb, const ucdrEndianness endianness, const uint16_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_2(mb, endianness, (uint16_t*)array, size);
}

bool ucdr_serialize_endian_sequence_uint32_t(ucdrBuffer* mb, const ucdrEndianness endianness, const uint32_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_4(mb, endianness, (uint32_t*)array, size);
}

bool ucdr_serialize_endian_sequence_uint64_t(ucdrBuffer* mb, const ucdrEndianness endianness, const uint64_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_8(mb, endianness, (uint64_t*)array, size);
}

bool ucdr_serialize_endian_sequence_int8_t(ucdrBuffer* mb, const ucdrEndianness endianness, const int8_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_1(mb, endianness, (uint8_t*)array, size);
}

bool ucdr_serialize_endian_sequence_int16_t(ucdrBuffer* mb, const ucdrEndianness endianness, const int16_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_2(mb, endianness, (uint16_t*)array, size);
}

bool ucdr_serialize_endian_sequence_int32_t(ucdrBuffer* mb, const ucdrEndianness endianness, const int32_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_4(mb, endianness, (uint32_t*)array, size);
}

bool ucdr_serialize_endian_sequence_int64_t(ucdrBuffer* mb, const ucdrEndianness endianness, const int64_t* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_8(mb, endianness, (uint64_t*)array, size);
}

bool ucdr_serialize_endian_sequence_float(ucdrBuffer* mb, const ucdrEndianness endianness, const float* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_4(mb, endianness, (uint32_t*)array, size);
}

bool ucdr_serialize_endian_sequence_double(ucdrBuffer* mb, const ucdrEndianness endianness, const double* array, const uint32_t size)
{
    return ucdr_serialize_sequence_byte_8(mb, endianness, (uint64_t*)array, size);
}

bool ucdr_deserialize_endian_sequence_char(ucdrBuffer* mb, const ucdrEndianness endianness, char* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_bool(ucdrBuffer* mb, const ucdrEndianness endianness, bool* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_uint8_t(ucdrBuffer* mb, const ucdrEndianness endianness, uint8_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_uint16_t(ucdrBuffer* mb, const ucdrEndianness endianness, uint16_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_2(mb, endianness, (uint16_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_uint32_t(ucdrBuffer* mb, const ucdrEndianness endianness, uint32_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_4(mb, endianness, (uint32_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_uint64_t(ucdrBuffer* mb, const ucdrEndianness endianness, uint64_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_8(mb, endianness, (uint64_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_int8_t(ucdrBuffer* mb, const ucdrEndianness endianness, int8_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_1(mb, endianness, (uint8_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_int16_t(ucdrBuffer* mb, const ucdrEndianness endianness, int16_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_2(mb, endianness, (uint16_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_int32_t(ucdrBuffer* mb, const ucdrEndianness endianness, int32_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_4(mb, endianness, (uint32_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_int64_t(ucdrBuffer* mb, const ucdrEndianness endianness, int64_t* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_8(mb, endianness, (uint64_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_float(ucdrBuffer* mb, const ucdrEndianness endianness, float* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_4(mb, endianness, (uint32_t*)array, array_capacity, size);
}

bool ucdr_deserialize_endian_sequence_double(ucdrBuffer* mb, const ucdrEndianness endianness, double* array, const uint32_t array_capacity, uint32_t* size)
{
    return ucdr_deserialize_sequence_byte_8(mb, endianness, (uint64_t*)array, array_capacity, size);
}

