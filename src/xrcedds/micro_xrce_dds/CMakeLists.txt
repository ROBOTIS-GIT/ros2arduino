# Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###############################################################################
# CMake build rules for Micro XRCE-DDS Client
###############################################################################
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(IS_TOP_LEVEL TRUE)
if(PROJECT_SOURCE_DIR)
    set(IS_TOP_LEVEL FALSE)
endif()

# Set CMAKE_BUILD_TYPE to Release by default.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

###############################################################################
# Product information
###############################################################################
if(CMAKE_VERSION VERSION_LESS 3.0)
    project(microxrcedds_client C)
    set(PROJECT_VERSION_MAJOR 1)
    set(PROJECT_VERSION_MINOR 0)
    set(PROJECT_VERSION_PATCH 0)
    set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
else()
    cmake_policy(SET CMP0048 NEW)
    project(microxrcedds_client VERSION "1.0.0" LANGUAGES C)
endif()

###############################################################################
# eProsima build options
###############################################################################
option(EPROSIMA_BUILD "Activate internal building" OFF)
option(EPROSIMA_BUILD_EXAMPLES "Compile examples" OFF)
option(EPROSIMA_BUILD_TESTS "Activate the building tests" OFF)
option(THIRDPARTY "Activate the build of thirdparties" OFF)
option(VERBOSE_SERIALIZATION "Use verbose output" OFF)
option(VERBOSE_MESSAGE "Use verbose output" OFF)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    option(MEMORY_PERFORMANCE "Memory performace test." OFF)
endif()

if(EPROSIMA_BUILD)
    set(EPROSIMA_BUILD_EXAMPLES ON)
    set(EPROSIMA_BUILD_TESTS ON)
    set(THIRDPARTY ON)
endif()

###############################################################################
# Client configuration options
###############################################################################

# Load configuration file.
set(CLIENT_CONFIG "${PROJECT_SOURCE_DIR}/client.config" CACHE PATH "Configuration client file.")

if(EXISTS ${CLIENT_CONFIG})
    configure_file(${CLIENT_CONFIG} ${PROJECT_SOURCE_DIR}/CMakeCache.txt COPYONLY)
    load_cache(${PROJECT_SOURCE_DIR})
    file(REMOVE ${PROJECT_SOURCE_DIR}/CMakeCache.txt)
else()
    message(FATAL_ERROR "No client.config found.")
endif()

if(PROFILE_CREATE_ENTITIES_REF OR PROFILE_CREATE_ENTITIES_XML)
    set(PROFILE_COMMON_CREATE_ENTITIES ON)
else()
    set(PROFILE_COMMON_CREATE_ENTITIES OFF)
endif()

if(CONFIG_MACHINE_ENDIANNESS)
    set(__BIG_ENDIAN__ CONFIG_MACHINE_ENDIANNESS)
endif()

###############################################################################
# Check MSVC architecture
###############################################################################
include(${PROJECT_SOURCE_DIR}/cmake/common/check_configuration.cmake)
if(MSVC OR MSVC_IDE)
    check_msvc_arch()
endif()

###############################################################################
# Load external eProsima projects.
###############################################################################
include(${PROJECT_SOURCE_DIR}/cmake/common/eprosima_libraries.cmake)
eprosima_find_package(microcdr)

###############################################################################
# Sources
###############################################################################

# Check platform.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_NAME_LINUX ON)
    set(TRANSPORT_TYPES "UDP" "TCP" "SERIAL")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM_NAME_WINDOWS ON)
    set(TRANSPORT_TYPES "UDP" "TCP")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    if(PLATFORM_NAME STREQUAL "nuttx")
        set(PLATFORM_NAME_NUTTX ON)
        set(TRANSPORT_TYPES "UDP" "TCP" "SERIAL")
    endif()
endif()

# Transport sources.
foreach(TRANS_TYPE ${TRANSPORT_TYPES})
    if(PROFILE_${TRANS_TYPE}_TRANSPORT)
        string(TOLOWER ${TRANS_TYPE} TT)
        if(PLATFORM_NAME_LINUX)
            set(${TRANS_TYPE}_SRCS
                src/c/profile/transport/${TT}/${TT}_transport.c
                src/c/profile/transport/${TT}/${TT}_transport_linux.c
                )
        elseif(PLATFORM_NAME_WINDOWS)
            set(${TRANS_TYPE}_SRCS
                src/c/profile/transport/${TT}/${TT}_transport.c
                src/c/profile/transport/${TT}/${TT}_transport_windows.c
                )
        elseif(PLATFORM_NAME_NUTTX)
            set(${TRANS_TYPE}_SRCS
                src/c/profile/transport/${TT}/${TT}_transport.c
                src/c/profile/transport/${TT}/${TT}_transport_linux.c
                )
        endif()
    endif()
endforeach()

# Transport discovery source.
if(PROFILE_DISCOVERY)
    if(PLATFORM_NAME_LINUX)
        set(UDP_DISCOVERY_SRCS src/c/profile/discovery/transport/udp_transport_linux_datagram.c)
    elseif(PLATFORM_NAME_NUTTX)
        set(UDP_DISCOVERY_SRCS src/c/profile/discovery/transport/udp_transport_linux_datagram.c)
    endif()
endif()

# Other sources
set(SRCS
    src/c/core/communication/serial_protocol.c
    src/c/core/session/stream/input_best_effort_stream.c
    src/c/core/session/stream/input_reliable_stream.c
    src/c/core/session/stream/output_best_effort_stream.c
    src/c/core/session/stream/output_reliable_stream.c
    src/c/core/session/stream/stream_storage.c
    src/c/core/session/stream/stream_id.c
    src/c/core/session/stream/seq_num.c
    src/c/core/session/session.c
    src/c/core/session/session_info.c
    src/c/core/session/submessage.c
    src/c/core/session/object_id.c
    src/c/core/serialization/xrce_protocol.c
    src/c/core/serialization/xrce_header.c
    src/c/core/serialization/xrce_subheader.c
    src/c/util/time.c
    $<$<OR:$<BOOL:${VERBOSE_MESSAGE}>,$<BOOL:${VERBOSE_SERIALIZATION}>>:src/c/core/log/log.c>
    $<$<BOOL:${PROFILE_COMMON_CREATE_ENTITIES}>:src/c/profile/session/common_create_entities.c>
    $<$<BOOL:${PROFILE_CREATE_ENTITIES_REF}>:src/c/profile/session/create_entities_ref.c>
    $<$<BOOL:${PROFILE_CREATE_ENTITIES_XML}>:src/c/profile/session/create_entities_xml.c>
    $<$<BOOL:${PROFILE_READ_ACCESS}>:src/c/profile/session/read_access.c>
    $<$<BOOL:${PROFILE_WRITE_ACCESS}>:src/c/profile/session/write_access.c>
    $<$<AND:$<NOT:$<PLATFORM_ID:Windows>>,$<BOOL:${PROFILE_DISCOVERY}>>:src/c/profile/discovery/discovery.c>
    ${UDP_DISCOVERY_SRCS}
    ${UDP_SRCS}
    ${TCP_SRCS}
    ${SERIAL_SRCS}
    )

###############################################################################
# Target
###############################################################################
add_library(${PROJECT_NAME} ${SRCS})

set_common_compile_options(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    )

target_link_libraries(${PROJECT_NAME} PUBLIC microcdr
    $<$<BOOL:${WIN32}>:ws2_32>
    )
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/c>
    )

###############################################################################
# Config
###############################################################################

# Install path
set(BIN_INSTALL_DIR bin/ CACHE PATH "Installation directory for binaries")
set(INCLUDE_INSTALL_DIR include/ CACHE PATH "Installation directory for C headers")
set(LIB_INSTALL_DIR lib/ CACHE PATH "Installation directory for libraries")
set(DATA_INSTALL_DIR share/ CACHE PATH "Installation directory for data")
if(WIN32)
    set(LICENSE_INSTALL_DIR . CACHE PATH "Installation directory for licenses")
else()
    set(LICENSE_INSTALL_DIR ${DATA_INSTALL_DIR}/${PROJECT_NAME} CACHE PATH "Installation directory for licenses")
endif()

# Create source files with the define
configure_file(${PROJECT_SOURCE_DIR}/include/uxr/client/config.h.in
    ${PROJECT_BINARY_DIR}/include/uxr/client/config.h
    )

###############################################################################
# Compile setting
###############################################################################

# Definitions
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -D$<$<BOOL:${VERBOSE_SERIALIZATION}>:UXR_SERIALIZATION_LOGS>
        -D$<$<BOOL:${VERBOSE_MESSAGE}>:UXR_MESSAGE_LOGS>
    )

get_target_property(TARGET_TYPE ${PROJECT_NAME} TYPE)
if(MSVC OR MSVC_IDE)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4996)
    if(TARGET_TYPE STREQUAL "SHARED_LIBRARY")
        target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            -D${PROJECT_NAME}_SHARED
        )
    endif()
endif()

###############################################################################
# Examples
###############################################################################
if(EPROSIMA_BUILD_EXAMPLES)
    add_subdirectory(examples/PublishHelloWorld)
    add_subdirectory(examples/SubscribeHelloWorld)
    add_subdirectory(examples/ShapesDemo)
    add_subdirectory(examples/Deployment)
    if(PLATFORM_NAME_LINUX OR PLATFORM_NAME_NUTTX)
        add_subdirectory(examples/Discovery)
    endif()
endif()

###############################################################################
# Test
###############################################################################
if(EPROSIMA_BUILD_TESTS AND IS_TOP_LEVEL)
    include(${PROJECT_SOURCE_DIR}/cmake/common/gtest.cmake)
    check_gtest()
    enable_testing()
    include(CTest)

    if(NOT ((MSVC OR MSVC_IDE) AND EPROSIMA_INSTALLER))
        add_subdirectory(test/unitary/streams)
        add_subdirectory(test/integration/interaction)

        if(NOT WIN32)
            add_subdirectory(test/transport/serial_comm)
            add_subdirectory(test/integration/cross_serialization)
        endif()
    endif()
endif()

if(PLATFORM_NAME_LINUX AND MEMORY_PERFORMANCE)
    add_subdirectory(test/memory/consumption)
endif()

###############################################################################
# Packaging
###############################################################################

# Install library
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    COMPONENT libraries
    )

# Install includes
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/uxr/client/
    DESTINATION ${INCLUDE_INSTALL_DIR}/uxr/client
    FILES_MATCHING PATTERN "*.h"
    )

# Install config.h
install(FILES ${PROJECT_BINARY_DIR}/include/uxr/client/config.h
    DESTINATION ${INCLUDE_INSTALL_DIR}/uxr/client
    )

# Export library
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/cmake
    )

# Package configuration
include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/packaging/Config.cmake.in
    ${PROJECT_BINARY_DIR}/cmake/config/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/cmake
    PATH_VARS BIN_INSTALL_DIR INCLUDE_INSTALL_DIR LIB_INSTALL_DIR DATA_INSTALL_DIR
    )
install(FILES ${PROJECT_BINARY_DIR}/cmake/config/${PROJECT_NAME}Config.cmake
    DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/cmake
    )
