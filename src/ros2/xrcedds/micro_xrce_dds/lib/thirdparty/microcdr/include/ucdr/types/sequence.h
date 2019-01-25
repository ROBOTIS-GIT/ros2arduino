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

#ifndef _MICROCDR_TYPES_SEQUENCE_H_
#define _MICROCDR_TYPES_SEQUENCE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ucdr/common.h>

// -------------------------------------------------------------------
//                   PUBLIC SERIALIZATION FUNCTIONS
// -------------------------------------------------------------------

UCDRDLLAPI bool ucdr_serialize_sequence_char(ucdrBuffer* mb, const char* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_bool(ucdrBuffer* mb, const bool* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_uint8_t(ucdrBuffer* mb, const uint8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_uint16_t(ucdrBuffer* mb, const uint16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_uint32_t(ucdrBuffer* mb, const uint32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_uint64_t(ucdrBuffer* mb, const uint64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_int8_t(ucdrBuffer* mb, const int8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_int16_t(ucdrBuffer* mb, const int16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_int32_t(ucdrBuffer* mb, const int32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_int64_t(ucdrBuffer* mb, const int64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_float(ucdrBuffer* mb, const float* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_sequence_double(ucdrBuffer* mb, const double* array, const uint32_t size);

UCDRDLLAPI bool ucdr_deserialize_sequence_char(ucdrBuffer* mb, char* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_bool(ucdrBuffer* mb, bool* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_uint8_t(ucdrBuffer* mb, uint8_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_uint16_t(ucdrBuffer* mb, uint16_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_uint32_t(ucdrBuffer* mb, uint32_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_uint64_t(ucdrBuffer* mb, uint64_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_int8_t(ucdrBuffer* mb, int8_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_int16_t(ucdrBuffer* mb, int16_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_int32_t(ucdrBuffer* mb, int32_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_int64_t(ucdrBuffer* mb, int64_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_float(ucdrBuffer* mb, float* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_sequence_double(ucdrBuffer* mb, double* array, const uint32_t array_capacity, uint32_t* size);

UCDRDLLAPI bool ucdr_serialize_endian_sequence_char(ucdrBuffer* mb, ucdrEndianness endianness, const char* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_bool(ucdrBuffer* mb, ucdrEndianness endianness, const bool* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_uint8_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_uint16_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_uint32_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_uint64_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_int8_t(ucdrBuffer* mb, ucdrEndianness endianness, const int8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_int16_t(ucdrBuffer* mb, ucdrEndianness endianness, const int16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_int32_t(ucdrBuffer* mb, ucdrEndianness endianness, const int32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_int64_t(ucdrBuffer* mb, ucdrEndianness endianness, const int64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_float(ucdrBuffer* mb, ucdrEndianness endianness, const float* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_sequence_double(ucdrBuffer* mb, ucdrEndianness endianness, const double* array, const uint32_t size);

UCDRDLLAPI bool ucdr_deserialize_endian_sequence_char(ucdrBuffer* mb, ucdrEndianness endianness, char* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_bool(ucdrBuffer* mb, ucdrEndianness endianness, bool* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_uint8_t(ucdrBuffer* mb, ucdrEndianness endianness, uint8_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_uint16_t(ucdrBuffer* mb, ucdrEndianness endianness, uint16_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_uint32_t(ucdrBuffer* mb, ucdrEndianness endianness, uint32_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_uint64_t(ucdrBuffer* mb, ucdrEndianness endianness, uint64_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_int8_t(ucdrBuffer* mb, ucdrEndianness endianness, int8_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_int16_t(ucdrBuffer* mb, ucdrEndianness endianness, int16_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_int32_t(ucdrBuffer* mb, ucdrEndianness endianness, int32_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_int64_t(ucdrBuffer* mb, ucdrEndianness endianness, int64_t* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_float(ucdrBuffer* mb, ucdrEndianness endianness, float* array, const uint32_t array_capacity, uint32_t* size);
UCDRDLLAPI bool ucdr_deserialize_endian_sequence_double(ucdrBuffer* mb, ucdrEndianness endianness, double* array, const uint32_t array_capacity, uint32_t* size);

#ifdef __cplusplus
}
#endif

#endif //_MICROCDR_TYPES_SEQUENCE_H_
