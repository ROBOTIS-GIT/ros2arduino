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
#include <uxr/client/client.h>
#include <ucdr/microcdr.h>
#include "transport_ex.h"

typedef void(*uxr_onTopicUserCallback)(uint16_t id, void* msgs, void* args);
typedef void(*uxr_onReplyUserCallback)(uint16_t id, void* msgs, void* sample_id, void* args);

typedef struct uxr_session{
  bool               is_init;
  uint32_t           session_key;
  uxrSession         session;
  uxrCommunication  *comm_port;
  uxrStreamId        output_stream_id;
  uxrStreamId        input_stream_id;
#ifdef PROFILE_SERIAL_TRANSPORT
  uxrSerialTransport transport_serial;
  uxrSerialPlatform  platform_serial;
#endif
#ifdef PROFILE_UDP_TRANSPORT
  uxrUDPTransport    transport_udp;
  uxrUDPPlatform     platform_udp;
#endif
#ifdef PROFILE_TCP_TRANSPORT
  uxrTCPTransport    transport_tcp;
  uxrTCPPlatform     platform_tcp;
#endif
} uxr_session_t;

void uxr_onReplyCallback(struct uxrSession* session, uxrObjectId object_id,
  uint16_t request_id, SampleIdentity* sample_id, ucdrBuffer* ub,
  uint16_t length, void* args);
void uxr_onTopicCallback(struct uxrSession* session, uxrObjectId object_id, 
  uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer* ub, 
  uint16_t length, void* args);
void uxr_setOnReplyUserCallback(uxr_onReplyUserCallback callback_func);
void uxr_setOnTopicUserCallback(uxr_onTopicUserCallback callback_func);

#ifdef __cplusplus
}
#endif

#endif /* MICRO_XRCE_DDS_H_ */
