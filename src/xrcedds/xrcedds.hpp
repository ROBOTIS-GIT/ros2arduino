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

namespace xrcedds {

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


/*
  Client (1)-(*) Participant (1)-(*) topic
                             (1)-(*) publisher  (1)-(*) data_writer
                             (1)-(*) subscriber (1)-(*) data_reader 
*/

// template<int MAX_PUBLISHER, 
//          int MAX_SUBSCRIBER,
//          int INPUT_SIZE = 512,
//          int OUTPUT_SIZE = 512>
// class XRCEDDS
// {
//   public:
//     virtual bool setupTransport() = 0;
//     virtual bool createClient() = 0;
//     virtual bool createEntity() = 0;
//     virtual bool deleteEntity() = 0;

//     virtual bool writeTopic() = 0;
//     virtual bool readTopic() = 0;

//   private:
//     virtual bool createParticipant() = 0;
//     virtual bool registerTopic() = 0;
//     virtual bool createPublisher() = 0;
//     virtual bool createSubscriber() = 0;
//     virtual bool createWriter() = 0;
//     virtual bool createReader() = 0;

//     Publisher_t publisher_[MAX_PUBLISHER];
//     Subscriber_t subscriber_[MAX_SUBSCRIBER];
// };


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
