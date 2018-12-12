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

#ifndef _UXR_CLIENT_CLIENT_H_
#define _UXR_CLIENT_CLIENT_H_

#include "config.h"

#ifdef PROFILE_CREATE_ENTITIES_REF
#include "profile/session/create_entities_ref.h"
#endif //PROFILE_CREATE_ENTITIES_REF

#ifdef PROFILE_CREATE_ENTITIES_XML
#include "profile/session/create_entities_xml.h"
#endif //PROFILE_CREATE_ENTITIES_XML

#ifdef PROFILE_READ_ACCESS
#include "profile/session/read_access.h"
#endif //PROFILE_READ_ACCESS

#ifdef PROFILE_WRITE_ACCESS
#include "profile/session/write_access.h"
#endif //PROFILE_WRITE_ACCESS

#ifdef PROFILE_DISCOVERY
#include "profile/discovery/discovery.h"
#endif //PROFILE_DISCOVERY

#include "transport.h"
#include "core/session/session.h"

#endif // _UXR_CLIENT_CLIENT_H_
