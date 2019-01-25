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

#ifndef _UXR_CLIENT_CORE_SESSION_SESSION_H_
#define _UXR_CLIENT_CORE_SESSION_SESSION_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/core/session/session_info.h>
#include <uxr/client/core/session/stream/stream_storage.h>

#define UXR_TIMEOUT_INF       -1

struct uxrSession;
struct uxrCommunication;

typedef void (*uxrOnStatusFunc) (struct uxrSession* session, uxrObjectId object_id, uint16_t request_id,
                             uint8_t status, void* args);
typedef void (*uxrOnTopicFunc) (struct uxrSession* session, uxrObjectId object_id, uint16_t request_id,
                             uxrStreamId stream_id, struct ucdrBuffer* mb, void* args);

typedef struct uxrSession
{
    uxrSessionInfo info;
    uxrStreamStorage streams;
    struct uxrCommunication* comm;

    const uint16_t* request_list;
    uint8_t* status_list;
    size_t request_status_list_size;

    uxrOnStatusFunc on_status;
    void* on_status_args;

    uxrOnTopicFunc on_topic;
    void* on_topic_args;

} uxrSession;

//==================================================================
//                         PUBLIC FUNCTIONS
//==================================================================
UXRDLLAPI void uxr_init_session(uxrSession* session, struct uxrCommunication* comm, uint32_t key);
UXRDLLAPI void uxr_set_status_callback(uxrSession* session, uxrOnStatusFunc on_status_func, void* args);
UXRDLLAPI void uxr_set_topic_callback(uxrSession* session, uxrOnTopicFunc on_topic_func, void* args);

UXRDLLAPI bool uxr_create_session(uxrSession* session);
UXRDLLAPI bool uxr_delete_session(uxrSession* session);

UXRDLLAPI uxrStreamId uxr_create_output_best_effort_stream(uxrSession* session, uint8_t* buffer, size_t size);
UXRDLLAPI uxrStreamId uxr_create_output_reliable_stream(uxrSession* session, uint8_t* buffer, size_t size, uint16_t history);
UXRDLLAPI uxrStreamId uxr_create_input_best_effort_stream(uxrSession* session);
UXRDLLAPI uxrStreamId uxr_create_input_reliable_stream(uxrSession* session, uint8_t* buffer, size_t size, uint16_t history);

UXRDLLAPI void uxr_flash_output_streams(uxrSession* session);
UXRDLLAPI bool uxr_run_session_time(uxrSession* session, int time);
UXRDLLAPI bool uxr_run_session_until_timeout(uxrSession* session, int timeout);
UXRDLLAPI bool uxr_run_session_until_confirm_delivery(uxrSession* session, int timeout);
UXRDLLAPI bool uxr_run_session_until_all_status(uxrSession* session, int timeout, const uint16_t* request_list, uint8_t* status_list, size_t list_size);
UXRDLLAPI bool uxr_run_session_until_one_status(uxrSession* session, int timeout, const uint16_t* request_list, uint8_t* status_list, size_t list_size);

#ifdef __cplusplus
}
#endif

#endif // _UXR_CLIENT_CORE_SESSION_SESSION_H
