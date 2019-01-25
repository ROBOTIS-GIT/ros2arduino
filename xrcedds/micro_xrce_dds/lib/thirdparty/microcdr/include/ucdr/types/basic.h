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

#ifndef _MICROCDR_TYPES_BASIC_H_
#define _MICROCDR_TYPES_BASIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ucdr/common.h>

// -------------------------------------------------------------------
//                   PUBLIC SERIALIZATION FUNCTIONS
// -------------------------------------------------------------------

UCDRDLLAPI bool ucdr_serialize_char(ucdrBuffer* mb, const char value);
UCDRDLLAPI bool ucdr_serialize_bool(ucdrBuffer* mb, const bool value);
UCDRDLLAPI bool ucdr_serialize_uint8_t(ucdrBuffer* mb, const uint8_t value);
UCDRDLLAPI bool ucdr_serialize_uint16_t(ucdrBuffer* mb, const uint16_t value);
UCDRDLLAPI bool ucdr_serialize_uint32_t(ucdrBuffer* mb, const uint32_t value);
UCDRDLLAPI bool ucdr_serialize_uint64_t(ucdrBuffer* mb, const uint64_t value);
UCDRDLLAPI bool ucdr_serialize_int8_t(ucdrBuffer* mb, const int8_t value);
UCDRDLLAPI bool ucdr_serialize_int16_t(ucdrBuffer* mb, const int16_t value);
UCDRDLLAPI bool ucdr_serialize_int32_t(ucdrBuffer* mb, const int32_t value);
UCDRDLLAPI bool ucdr_serialize_int64_t(ucdrBuffer* mb, const int64_t value);
UCDRDLLAPI bool ucdr_serialize_float(ucdrBuffer* mb, const float value);
UCDRDLLAPI bool ucdr_serialize_double(ucdrBuffer* mb, const double value);

UCDRDLLAPI bool ucdr_deserialize_char(ucdrBuffer* mb, char* value);
UCDRDLLAPI bool ucdr_deserialize_bool(ucdrBuffer* mb, bool* value);
UCDRDLLAPI bool ucdr_deserialize_uint8_t(ucdrBuffer* mb, uint8_t* value);
UCDRDLLAPI bool ucdr_deserialize_uint16_t(ucdrBuffer* mb, uint16_t* value);
UCDRDLLAPI bool ucdr_deserialize_uint32_t(ucdrBuffer* mb, uint32_t* value);
UCDRDLLAPI bool ucdr_deserialize_uint64_t(ucdrBuffer* mb, uint64_t* value);
UCDRDLLAPI bool ucdr_deserialize_int8_t(ucdrBuffer* mb, int8_t* value);
UCDRDLLAPI bool ucdr_deserialize_int16_t(ucdrBuffer* mb, int16_t* value);
UCDRDLLAPI bool ucdr_deserialize_int32_t(ucdrBuffer* mb, int32_t* value);
UCDRDLLAPI bool ucdr_deserialize_int64_t(ucdrBuffer* mb, int64_t* value);
UCDRDLLAPI bool ucdr_deserialize_float(ucdrBuffer* mb, float* value);
UCDRDLLAPI bool ucdr_deserialize_double(ucdrBuffer* mb, double* value);

UCDRDLLAPI bool ucdr_serialize_endian_uint16_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint16_t value);
UCDRDLLAPI bool ucdr_serialize_endian_uint32_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint32_t value);
UCDRDLLAPI bool ucdr_serialize_endian_uint64_t(ucdrBuffer* mb, ucdrEndianness endianness, const uint64_t value);
UCDRDLLAPI bool ucdr_serialize_endian_int16_t(ucdrBuffer* mb, ucdrEndianness endianness, const int16_t value);
UCDRDLLAPI bool ucdr_serialize_endian_int32_t(ucdrBuffer* mb, ucdrEndianness endianness, const int32_t value);
UCDRDLLAPI bool ucdr_serialize_endian_int64_t(ucdrBuffer* mb, ucdrEndianness endianness, const int64_t value);
UCDRDLLAPI bool ucdr_serialize_endian_float(ucdrBuffer* mb, ucdrEndianness endianness, const float value);
UCDRDLLAPI bool ucdr_serialize_endian_double(ucdrBuffer* mb, ucdrEndianness endianness, const double value);

UCDRDLLAPI bool ucdr_deserialize_endian_uint16_t(ucdrBuffer* mb, ucdrEndianness endianness, uint16_t* value);
UCDRDLLAPI bool ucdr_deserialize_endian_uint32_t(ucdrBuffer* mb, ucdrEndianness endianness, uint32_t *value);
UCDRDLLAPI bool ucdr_deserialize_endian_uint64_t(ucdrBuffer* mb, ucdrEndianness endianness, uint64_t* value);
UCDRDLLAPI bool ucdr_deserialize_endian_int16_t(ucdrBuffer* mb, ucdrEndianness endianness, int16_t* value);
UCDRDLLAPI bool ucdr_deserialize_endian_int32_t(ucdrBuffer* mb, ucdrEndianness endianness, int32_t* value);
UCDRDLLAPI bool ucdr_deserialize_endian_int64_t(ucdrBuffer* mb, ucdrEndianness endianness, int64_t* value);
UCDRDLLAPI bool ucdr_deserialize_endian_float(ucdrBuffer* mb, ucdrEndianness endianness, float* value);
UCDRDLLAPI bool ucdr_deserialize_endian_double(ucdrBuffer* mb, ucdrEndianness endianness, double* value);

#ifdef __cplusplus
}
#endif

#endif //_MICROCDR_TYPES_BASIC_H_
