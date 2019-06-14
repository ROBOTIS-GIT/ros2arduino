// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef UXR_CLIENT_CORE_SESSION_WRITE_ACCESS_H_
#define UXR_CLIENT_CORE_SESSION_WRITE_ACCESS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/core/session/session.h>

/**
 * @brief Buffers into the stream identified by `stream_id` an XRCE WRITE_DATA submessage.
 *        The submessage will be sent when `uxr_flash_output_stream` or `uxr_run_session` function are called.
 *        As a result of the reception of this submessage, the Agent will write a topic into the DDS Global-Data-Space.
 * @param session           A uxrSession structure previously initialized.
 * @param stream_id         The output stream identifier where the READ_DATA submessage will be buffered.
 * @param datawriter_id     The identifier of the XRCE DataWriter that will write the topic into the DDS GDS.
 * @param ub_topic          The ucdrBuffer structure used for serializing the topic.
 * @param topic_size        The size of the topic in bytes.
 * @return A `request_id` that identifies the request made by the Client.
 *         This could be used in the `uxr_run_session_until_one_status` or `uxr_run_session_until_all_status` functions.
 */
UXRDLLAPI bool uxr_prepare_output_stream(
        uxrSession* session,
        uxrStreamId stream_id,
        uxrObjectId datawriter_id,
        struct ucdrBuffer* ub_topic,
        uint32_t topic_size);

#ifdef __cplusplus
}
#endif

#endif // UXR_CLIENT_CORE_SESSION_WRITE_ACCESS_H_
