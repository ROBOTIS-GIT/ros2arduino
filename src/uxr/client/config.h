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

#include <user_config.h>

#define USER_TRANSPORT_TYPE_UDP    0
#define USER_TRANSPORT_TYPE_TCP    1
#define USER_TRANSPORT_TYPE_SERIAL 2

#define UXR_CLIENT_VERSION_MAJOR 1
#define UXR_CLIENT_VERSION_MINOR 2
#define UXR_CLIENT_VERSION_MICRO 1
#define UXR_CLIENT_VERSION_STR "1.2.1"

// related to user definition in sketch file (.ino)
#if defined(USER_TRANSPORT_TYPE)
#if (USER_TRANSPORT_TYPE == USER_TRANSPORT_TYPE_UDP)
#define PROFILE_UDP_TRANSPORT
#elif (USER_TRANSPORT_TYPE == USER_TRANSPORT_TYPE_TCP)
#define PROFILE_TCP_TRANSPORT
#elif (USER_TRANSPORT_TYPE == USER_TRANSPORT_TYPE_SERIAL)
#define PROFILE_SERIAL_TRANSPORT
#else
#error "Please enter a valid transport number (USER_TRANSPORT_TYPE, refer to user_config.h in examples.)"
#endif
#else //To support legacy
#define PROFILE_UDP_TRANSPORT
#define PROFILE_TCP_TRANSPORT
#define PROFILE_SERIAL_TRANSPORT
#endif /* USER_TRANSPORT_TYPE */

#if !defined(USER_ROS2_PUBLISHER_MAX)
#define USER_ROS2_PUBLISHER_MAX  5
#endif

#if !defined(USER_ROS2_SUBSCRIBER_MAX)
#define USER_ROS2_SUBSCRIBER_MAX  5
#endif

#if !defined(UXR_CREATE_ENTITIES_USING_REF)
#define UXR_CREATE_ENTITIES_USING_REF 0
#endif


//#define PROFILE_DISCOVERY

#ifdef PROFILE_UDP_TRANSPORT
#define UCLIENT_EXTERNAL_UDP
#if defined(USER_TRANSPORT_MTU)
#define UXR_CONFIG_UDP_TRANSPORT_MTU   USER_TRANSPORT_MTU
#else //To support legacy
#define UXR_CONFIG_UDP_TRANSPORT_MTU   2048
#endif
#endif

#ifdef PROFILE_TCP_TRANSPORT
#define UCLIENT_EXTERNAL_TCP
#if defined(USER_TRANSPORT_MTU)
#define UXR_CONFIG_TCP_TRANSPORT_MTU   USER_TRANSPORT_MTU
#else //To support legacy
#define UXR_CONFIG_TCP_TRANSPORT_MTU   2048
#endif
#endif

#ifdef PROFILE_SERIAL_TRANSPORT
#define UCLIENT_EXTERNAL_SERIAL
#if defined(USER_TRANSPORT_MTU)
#define UXR_CONFIG_SERIAL_TRANSPORT_MTU   USER_TRANSPORT_MTU
#else //To support legacy 
#define UXR_CONFIG_SERIAL_TRANSPORT_MTU   2048
#endif
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

#define UXR_CONFIG_MAX_OUTPUT_BEST_EFFORT_STREAMS     1
#define UXR_CONFIG_MAX_OUTPUT_RELIABLE_STREAMS        1
#define UXR_CONFIG_MAX_INPUT_BEST_EFFORT_STREAMS      1
#define UXR_CONFIG_MAX_INPUT_RELIABLE_STREAMS         1

#define UXR_CONFIG_MAX_SESSION_CONNECTION_ATTEMPTS    10
#define UXR_CONFIG_MIN_SESSION_CONNECTION_INTERVAL    1
#define UXR_CONFIG_MIN_HEARTBEAT_TIME_INTERVAL        50


#endif // _UXR_CLIENT_CONFIG_H_
