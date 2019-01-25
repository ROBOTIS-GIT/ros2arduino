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

#ifndef _MICROCDR_TYPES_ARRAY_H_
#define _MICROCDR_TYPES_ARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ucdr/common.h>

// -------------------------------------------------------------------
//                   PUBLIC SERIALIZATION FUNCTIONS
// -------------------------------------------------------------------

UCDRDLLAPI bool ucdr_serialize_array_char(ucdrBuffer* mb, const char* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_bool(ucdrBuffer* mb, const bool* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_uint8_t(ucdrBuffer* mb, const uint8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_uint16_t(ucdrBuffer* mb, const uint16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_uint32_t(ucdrBuffer* mb, const uint32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_uint64_t(ucdrBuffer* mb, const uint64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_int8_t(ucdrBuffer* mb, const int8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_int16_t(ucdrBuffer* mb, const int16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_int32_t(ucdrBuffer* mb, const int32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_int64_t(ucdrBuffer* mb, const int64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_float(ucdrBuffer* mb, const float* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_array_double(ucdrBuffer* mb, const double* array, const uint32_t size);

UCDRDLLAPI bool ucdr_deserialize_array_char(ucdrBuffer* mb, char* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_bool(ucdrBuffer* mb, bool* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_uint8_t(ucdrBuffer* mb, uint8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_uint16_t(ucdrBuffer* mb, uint16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_uint32_t(ucdrBuffer* mb, uint32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_uint64_t(ucdrBuffer* mb, uint64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_int8_t(ucdrBuffer* mb, int8_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_int16_t(ucdrBuffer* mb, int16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_int32_t(ucdrBuffer* mb, int32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_int64_t(ucdrBuffer* mb, int64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_float(ucdrBuffer* mb, float* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_array_double(ucdrBuffer* mb, double* array, const uint32_t size);

UCDRDLLAPI bool ucdr_serialize_endian_array_uint16_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_array_uint32_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_array_uint64_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_array_int16_t(ucdrBuffer* mb, ucdrEndianness endianness, const int16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_array_int32_t(ucdrBuffer* mb, ucdrEndianness endianness, const int32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_array_int64_t(ucdrBuffer* mb, ucdrEndianness endianness, const int64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_array_float(ucdrBuffer* mb, ucdrEndianness endianness, const float* array, const uint32_t size);
UCDRDLLAPI bool ucdr_serialize_endian_array_double(ucdrBuffer* mb, ucdrEndianness endianness, const double* array, const uint32_t size);

UCDRDLLAPI bool ucdr_deserialize_endian_array_uint16_t(ucdrBuffer* mb, ucdrEndianness endianness, uint16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_endian_array_uint32_t(ucdrBuffer* mb, ucdrEndianness endianness, uint32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_endian_array_uint64_t(ucdrBuffer* mb, ucdrEndianness endianness, uint64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_endian_array_int16_t(ucdrBuffer* mb, ucdrEndianness endianness, int16_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_endian_array_int32_t(ucdrBuffer* mb, ucdrEndianness endianness, int32_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_endian_array_int64_t(ucdrBuffer* mb, ucdrEndianness endianness, int64_t* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_endian_array_float(ucdrBuffer* mb, ucdrEndianness endianness, float* array, const uint32_t size);
UCDRDLLAPI bool ucdr_deserialize_endian_array_double(ucdrBuffer* mb, ucdrEndianness endianness, double* array, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif //_MICROCDR_TYPES_ARRAY_H_
