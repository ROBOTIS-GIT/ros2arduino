/*
 * xrcedds.hpp
 *
 *  Created on: Nov 14, 2018
 *      Author: Kei
 */

#ifndef XRCEDDS_HPP_
#define XRCEDDS_HPP_

#include <stdint.h>
#include "micro_xrce_dds/micro_xrce_dds.h"

namespace xrcedds {

/* session
  uxrSession *session;
  uxrStreamId output_stream_id;
  uxrStreamId input_stream_id;
*/

typedef enum XrceDdsCommportType{
  XRCE_DDS_COMM_SERIAL = 0,
  XRCE_DDS_COMM_UDP,
  XRCE_DDS_COMM_TCP
} XrceDdsCommportType_t;

typedef struct Transport{
  XrceDdsCommportType_t type;
  const char* server_ip;
  const char* server_port;
  void *comm_instance;
} Transport_t;

typedef struct Participant{
  bool is_created;
  uint16_t id;
} Participant_t;

typedef struct Subscriber{
  bool is_created;
  uint16_t id;
  Participant_t *participant;
} Subscriber_t;

typedef struct Publisher{
  bool is_created;
  uint16_t id;
  Participant_t *participant;
} Publisher_t;

typedef struct Replier {
  bool is_created;
  uint16_t id;
  Participant_t* participant;
  uint16_t request_id;
} Replier_t;

typedef struct DataReader{
  bool is_created;
  uint16_t id;
  uint16_t request_id;
  Subscriber_t *subscriber;
} DataReader_t;

typedef struct DataWriter{
  bool is_created;
  uint16_t id;
  uint16_t request_id;
  Publisher_t *publisher;
} DataWriter_t;




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


void init(uint8_t rtps_product,unsigned int client_key=0xAABBCCDD);

bool initTransportAndSession(Transport_t* transport_info, void* callback_repl_func, void* callback_func, void* args);
void deleteTransportAndSession(Transport_t* transport_info);

bool createParticipant(Participant_t* participant, const char* participant_name);

bool registerTopic(Participant_t* participant, const char* topic_name, const char* topic_type, uint8_t topic_id);

bool createPublisher(Participant_t* participant, Publisher_t* publisher);
bool createSubscriber(Participant_t* participant, Subscriber_t* subscriber);
bool createReplier(Participant_t* participant, Replier_t* replier, char* profile_name, char* service_name,
    char* reader_name, char* writer_name, const char* request_topic_type, const char* response_topic_type);

bool createDataWriter(Publisher_t* publisher, DataWriter_t* data_writer, char* writer_name, const char* topic_type);
bool createDataReader(Subscriber_t* subscriber, DataReader_t* data_reader, char* reader_name, const char* topic_type);

bool readData(DataReader_t* data_reader);
bool readReplierData(Replier_t* replier);
bool replyData(Replier_t* replier, void* sample_id, uint8_t* buffer, uint32_t topic_size);
bool writeData(DataWriter_t* data_writer, void* buffer, uint32_t topic_size);

void deleteEntity(DataReader_t* data_reader);
void deleteEntity(DataWriter_t* data_writer);
void deleteEntity(Replier_t* replier);

bool runCommunication(uint32_t timeout_ms);


} /* namespace rtps */



#endif /* XRCEDDS_HPP_ */
