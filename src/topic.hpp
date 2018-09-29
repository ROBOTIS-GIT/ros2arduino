/*
 * topic.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_TOPIC_HPP_
#define ROS2_TOPIC_HPP_

#include <stdlib.h>
#include "micrortps.hpp"
#include "topic_config.h"

#define DEFAULT_TOPIC_XML ("<dds><topic><kind>NO_KEY</kind><name>%s%s</name><dataType>%s</dataType></topic></dds>")

namespace ros2 {

enum MessagePrefix{
  TOPICS_PUBLISH = 0,
  TOPICS_SUBSCRIBE,
  SERVICE_REQUEST,
  SERVICE_RESPONSE,
  SERVICE,
  PARAMETER,
  ACTION
};

const char* getPrefixString(MessagePrefix prefix);

/* Base Message Type */
template <typename MsgT>
class Topic
{

public:

  Topic(const char* type, uint8_t id):
    type_(type),
    id_(id)
  {
  }

  virtual ~Topic()
  {
  }

  virtual bool serialize(struct MicroBuffer* writer, const MsgT* topic) = 0;
  virtual bool deserialize(struct MicroBuffer* reader, MsgT* topic) = 0;

  virtual bool writeTopic(mrSession* session, mrStreamId stream_id, mrObjectId datawriter_id, MsgT* topic)
  {
    if (session == NULL)
    {
      return false;
    }

    bool written = false;
    uint32_t topic_length = size_of_topic(topic, 0);
    uint32_t payload_length = 0;
    payload_length += 4; //request_id + object_id
    payload_length += 4; //topic_length (remove in future version to be compliant)

    MicroBuffer mb;
    if(prepare_stream_to_write(&session->streams, stream_id, payload_length + topic_length + SUBHEADER_SIZE, &mb))
    {
      (void) write_submessage_header(&mb, SUBMESSAGE_ID_WRITE_DATA, (uint16_t)(payload_length + topic_length), FORMAT_DATA);

      WRITE_DATA_Payload_Data payload;
      init_base_object_request(&session->info, datawriter_id, &payload.base);
      (void) serialize_WRITE_DATA_Payload_Data(&mb, &payload);
      (void) serialize_uint32_t(&mb, topic_length); //REMOVE: when topics have not a previous size in the agent.

      MicroBuffer mb_topic;
      init_micro_buffer(&mb_topic, mb.iterator, topic_length);
      (void) serialize(&mb_topic, topic);

      written = true;
    }

    return written;
  }

  virtual uint32_t size_of_topic(const MsgT* topic, uint32_t size)
  {
    (void)(topic); (void)(size);

    return 0;
  }

  const char* type_;
  uint8_t id_;
};



} // namespace ros2



#endif /* ROS2_TOPIC_HPP_ */
