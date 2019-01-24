//    Copyright 2017 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#ifndef _UXR_CLIENT_CONFIG_H_
#define _UXR_CLIENT_CONFIG_H_

#define UXR_CLIENT_VERSION_MAJOR 1
#define UXR_CLIENT_VERSION_MINOR 0
#define UXR_CLIENT_VERSION_MICRO 1
#define UXR_CLIENT_VERSION_STR "1.0.1"


//external define

#define PLATFORM_NAME_ARDUINO
#define PROFILE_UDP_TRANSPORT
#define PROFILE_TCP_TRANSPORT
#define PROFILE_SERIAL_TRANSPORT
#define PROFILE_CREATE_ENTITIES_REF
#define PROFILE_CREATE_ENTITIES_XML
#define PROFILE_READ_ACCESS
#define PROFILE_WRITE_ACCESS
//#define PROFILE_DISCOVERY


#define UXR_CONFIG_MAX_OUTPUT_BEST_EFFORT_STREAMS     1
#define UXR_CONFIG_MAX_OUTPUT_RELIABLE_STREAMS        1
#define UXR_CONFIG_MAX_INPUT_BEST_EFFORT_STREAMS      1
#define UXR_CONFIG_MAX_INPUT_RELIABLE_STREAMS         1

#define UXR_CONFIG_MAX_SESSION_CONNECTION_ATTEMPTS    10
#define UXR_CONFIG_MIN_SESSION_CONNECTION_INTERVAL    1
#define UXR_CONFIG_MIN_HEARTBEAT_TIME_INTERVAL        1

#ifdef PROFILE_UDP_TRANSPORT
#define UXR_CONFIG_UDP_TRANSPORT_MTU                  512
#endif

#ifdef PROFILE_TCP_TRANSPORT
#define UXR_CONFIG_TCP_TRANSPORT_MTU                  512
#endif

#ifdef PROFILE_SERIAL_TRANSPORT
#define UXR_CONFIG_SERIAL_TRANSPORT_MTU               512
#endif

#if !defined(UXR_CONFIG_SERIAL_TRANSPORT_MTU)
#define UXR_CONFIG_SERIAL_TRANSPORT_MTU 0
#endif
#if !defined(UXR_CONFIG_UDP_TRANSPORT_MTU)
#define UXR_CONFIG_UDP_TRANSPORT_MTU 0
#endif
#if !defined(UXR_CONFIG_TCP_TRANSPORT_MTU)
#define UXR_CONFIG_TCP_TRANSPORT_MTU 0
#endif

#endif // _UXR_CLIENT_CONFIG_H_
