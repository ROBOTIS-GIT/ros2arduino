// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef _UXR_CLIENT_PROFILE_SESSION_CREATE_ENTITIES_XML_H_
#define _UXR_CLIENT_PROFILE_SESSION_CREATE_ENTITIES_XML_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/profile/session/common_create_entities.h>

UXRDLLAPI uint16_t uxr_buffer_create_participant_xml(uxrSession* session, uxrStreamId stream_id,
                                         uxrObjectId object_id, uint16_t domain, const char* xml, uint8_t mode);

UXRDLLAPI uint16_t uxr_buffer_create_topic_xml(uxrSession* session, uxrStreamId stream_id,
                                   uxrObjectId object_id, uxrObjectId participant_id, const char* xml, uint8_t mode);

UXRDLLAPI uint16_t uxr_buffer_create_publisher_xml(uxrSession* session, uxrStreamId stream_id,
                                       uxrObjectId object_id, uxrObjectId participant_id, const char* xml, uint8_t mode);

UXRDLLAPI uint16_t uxr_buffer_create_subscriber_xml(uxrSession* session, uxrStreamId stream_id,
                                        uxrObjectId object_id, uxrObjectId participant_id, const char* xml, uint8_t mode);

UXRDLLAPI uint16_t uxr_buffer_create_datawriter_xml(uxrSession* session, uxrStreamId stream_id,
                                        uxrObjectId object_id, uxrObjectId publisher_id, const char* xml, uint8_t mode);

UXRDLLAPI uint16_t uxr_buffer_create_datareader_xml(uxrSession* session, uxrStreamId stream_id,
                                        uxrObjectId object_id, uxrObjectId subscriber_id, const char* xml, uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif //_UXR_CLIENT_PROFILE_SESSION_CREATE_ENTITIES_XML_H_
