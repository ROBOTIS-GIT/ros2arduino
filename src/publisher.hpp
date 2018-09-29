/*
 * publisher.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_PUBLISHER_HPP_
#define ROS2_PUBLISHER_HPP_

#include <stdio.h>
#include "micrortps.hpp"
#include "node_handle.hpp"
#include "topic.hpp"
#include "hw.h"

#define DEFAULT_WRITER_XML ("<profiles><publisher profile_name=\"default_xrce_publisher_profile\"><topic><kind>NO_KEY</kind><name>%s%s</name><dataType>%s</dataType><historyQos><kind>KEEP_LAST</kind><depth>1</depth></historyQos><durability><kind>TRANSIENT_LOCAL</kind></durability></topic></publisher></profiles>")

extern uint32_t micros(void);



namespace ros2 {

template <typename MsgT>
class Publisher:public PublisherHandle
{
 
public:
  Publisher(micrortps::Participant_t* node, const char* name)
    : PublisherHandle()
  {
    node_ = node;
    name_ = name;
    this->recreate();
  }

  void publish(void)
  {
    if(is_registered_ ==  false)
    {
      return;
    }

    if(callback != NULL)
    {
      callback((void*)&topic_, callback_arg);
    }

    topic_.writeTopic(node_->session, publisher_.participant->output_stream_id, publisher_.writer_id, &topic_);
  }

  void recreate()
  {
    char publisher_profile[100] = {0, };
    sprintf(publisher_profile, "<publisher name=\"%s\"", name_);

    char writer_profile[512] = {0, };
    sprintf(writer_profile, DEFAULT_WRITER_XML, getPrefixString(TOPICS_PUBLISH), name_, topic_.type_);
    is_registered_ = micrortps::createPublisher(node_, &publisher_, publisher_profile, writer_profile);
    this->writer_id_ = publisher_.writer_id.id;
  }

private:
  MsgT topic_;
  const char* name_;
  micrortps::Participant_t* node_;
  micrortps::Publisher_t publisher_;
};


} // namespace ros2



#endif /* ROS2_PUBLISHER_HPP_ */
