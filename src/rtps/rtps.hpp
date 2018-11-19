/*
 * rtps.hpp
 *
 *  Created on: Nov 14, 2018
 *      Author: Kei
 */

#ifndef RTPS_HPP_
#define RTPS_HPP_

#include <stdint.h>
#include "micro_xrce_dds/include/uxr/client/client.h"

namespace rtps {

typedef struct Participant{
  bool is_init;
  uxrObjectId id;
  uxrSession *session;
  uxrStreamId output_stream_id;
  uxrStreamId input_stream_id;
} Participant_t;

typedef struct Subscriber{
  bool is_init;
  uxrObjectId id;
  uxrObjectId reader_id;
  uint16_t read_req;
  uxrDeliveryControl delivery_control;
  Participant_t *participant;
} Subscriber_t;

typedef struct Publisher{
  bool is_init;
  uxrObjectId id;
  uxrObjectId writer_id;
  Participant_t *participant;
} Publisher_t;

typedef struct Transport{
  uint8_t type;
  const char* server_ip;
  uint16_t server_port;
  void *serial_device;
} Transport_t;

//class rtps
//{
//  public:
//
//
//
//  private:
//
//};


void init(uint8_t rtps_product);
bool initTransportAndSession(Transport_t* transport_info, uxrOnTopicFunc callback_func, void* callback_args);
void deleteTransportAndSession(void);
bool createParticipant(Participant_t* participant, const char* participant_profile);
bool registerTopic(Participant_t* participant, char* topic_profile, uint8_t topic_id);
bool createPublisher(Participant_t* participant, Publisher_t* publisher, char* publisher_profile, char* writer_profile);
bool createSubscriber(Participant_t* participant, Subscriber_t* subscriber, char* subscriber_profile, char* reader_profile);
void subscribe(Subscriber_t* subscriber);
void publish(Publisher_t* publisher, void* buffer, uint32_t topic_size);
bool runCommunication(uint32_t timeout_ms);


} /* namespace rtps */



#endif /* RTPS_HPP_ */
