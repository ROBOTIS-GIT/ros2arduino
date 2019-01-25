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

#ifndef _UXR_CLIENT_CORE_SESSION_STREAM_STREAM_ID_H_
#define _UXR_CLIENT_CORE_SESSION_STREAM_STREAM_ID_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/dll.h>
#include <stdint.h>

typedef enum uxrStreamType
{
    UXR_NONE_STREAM,
    UXR_BEST_EFFORT_STREAM,
    UXR_RELIABLE_STREAM

} uxrStreamType;

typedef enum uxrStreamDirection
{
    UXR_INPUT_STREAM,
    UXR_OUTPUT_STREAM

} uxrStreamDirection;

typedef struct uxrStreamId
{
    uint8_t raw;
    uint8_t index;
    uxrStreamType type;
    uxrStreamDirection direction;

} uxrStreamId;

UXRDLLAPI uxrStreamId uxr_stream_id(uint8_t index, uxrStreamType type, uxrStreamDirection direction);
UXRDLLAPI uxrStreamId uxr_stream_id_from_raw(uint8_t stream_id_raw, uxrStreamDirection direction);

#ifdef __cplusplus
}
#endif

#endif // _UXR_CLIENT_CORE_SESSION_STREAM_STREAM_ID_H
