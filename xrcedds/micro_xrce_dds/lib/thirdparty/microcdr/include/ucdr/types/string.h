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

#ifndef _MICROCDR_TYPES_STRING_H_
#define _MICROCDR_TYPES_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ucdr/common.h>

#include <stdint.h>
#include <stdbool.h>

// -------------------------------------------------------------------
//                   PUBLIC SERIALIZATION FUNCTIONS
// -------------------------------------------------------------------

UCDRDLLAPI bool ucdr_serialize_string(ucdrBuffer* mb, const char* string);
UCDRDLLAPI bool ucdr_deserialize_string(ucdrBuffer* mb, char* string, const uint32_t string_capacity);

UCDRDLLAPI bool ucdr_serialize_endian_string(ucdrBuffer* mb, ucdrEndianness endianness, const char* string);
UCDRDLLAPI bool ucdr_deserialize_endian_string(ucdrBuffer* mb, ucdrEndianness endianness, char* string, const uint32_t string_capacity);

#ifdef __cplusplus
}
#endif

#endif //_MICROCDR_TYPES_STRING_H_
