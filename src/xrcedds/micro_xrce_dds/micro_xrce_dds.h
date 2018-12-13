/*
 * micro_xrce_dds.hpp
 *
 *  Created on: dec 14, 2018
 *      Author: Kei
 */

#ifndef MICRO_XRCE_DDS_H_
#define MICRO_XRCE_DDS_H_

#include <stdio.h>
#include "micro_xrce_dds_xml.h"

typedef struct uxr_session{
  bool               is_init;
  uint32_t           session_key;
  uxrSession         session;
  uxrCommunication  *comm_port;
  uxrStreamId        output_stream_id;
  uxrStreamId        input_stream_id;
  uxrSerialTransport transport;
  uxrSerialPlatform  platform;
} uxr_session_t;



#endif /* MICRO_XRCE_DDS_H_ */
