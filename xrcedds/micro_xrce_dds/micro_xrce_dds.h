/*
 * micro_xrce_dds.hpp
 *
 *  Created on: Dec 14, 2018
 *      Author: Kei
 */

#ifndef MICRO_XRCE_DDS_H_
#define MICRO_XRCE_DDS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "micro_xrce_dds_xml.h"
#include <ucdr/microcdr.h>

typedef void(*uxr_onTopicUserCallback)(uint16_t id, void* msgs, void* args);

typedef struct uxr_session{
  bool               is_init;
  uint32_t           session_key;
  uxrSession         session;
  uxrCommunication  *comm_port;
  uxrStreamId        output_stream_id;
  uxrStreamId        input_stream_id;
  uxrSerialTransport transport_serial;
  uxrSerialPlatform  platform_serial;
  uxrUDPTransport    transport_udp;
  uxrUDPPlatform     platform_udp;
  uxrTCPTransport    transport_tcp;
  uxrTCPPlatform     platform_tcp;
} uxr_session_t;

void uxr_onTopicCallback(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, ucdrBuffer* mb, void* args);
void uxr_setOnTopicUserCallback(uxr_onTopicUserCallback callback_func);

#ifdef __cplusplus
}
#endif

#endif /* MICRO_XRCE_DDS_H_ */
