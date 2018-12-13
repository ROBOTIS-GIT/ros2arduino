/*
 * topic.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_TOPIC_HPP_
#define ROS2_TOPIC_HPP_

#include <stdlib.h>
#include "xrcedds/xrcedds.hpp"
#include "xrcedds/micro_xrce_dds/lib/thirdparty/microcdr/include/ucdr/microcdr.h"
#include "xrcedds/micro_xrce_dds/micro_xrce_dds.h"

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

  Topic(const char* type, const char* name):
    type_(type),
    name_(name)
  {
  }

  virtual ~Topic()
  {
  }

  virtual bool serialize(ucdrBuffer* writer, const MsgT* topic) = 0;
  virtual bool deserialize(ucdrBuffer* reader, MsgT* topic) = 0;
  virtual uint32_t size_of_topic(const MsgT* topic, uint32_t size)
  {
    (void)(topic); (void)(size);

    return 0;
  }

  const char* type_;
  const char* name_;
};



} // namespace ros2



#endif /* ROS2_TOPIC_HPP_ */
