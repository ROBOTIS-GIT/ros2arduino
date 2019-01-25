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

#ifndef _UXR_CLIENT_PROFILE_DISCOVERY_DISCOVERY_H_
#define _UXR_CLIENT_PROFILE_DISCOVERY_DISCOVERY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <uxr/client/config.h>
#include <uxr/client/dll.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct uxrAgentAddress
{
    char ip[16]; //xxx.xxx.xxx.xxx\0
    uint16_t port;

} uxrAgentAddress;

typedef bool (*uxrOnAgentFound) (const uxrAgentAddress* address, int64_t timestamp, void* args);


UXRDLLAPI bool uxr_discovery_agents_multicast(uint32_t attempts, int period, uxrOnAgentFound on_agent_func, void* args, uxrAgentAddress* chosen);

UXRDLLAPI bool uxr_discovery_agents_unicast(uint32_t attempts, int period, uxrOnAgentFound on_agent_func, void* args, uxrAgentAddress* chosen,
                                          const uxrAgentAddress* agent_list, size_t agent_list_size);

#ifdef __cplusplus
}
#endif

#endif //_UXR_CLIENT_PROFILE_DISCOVERY_DISCOVERY_H_
