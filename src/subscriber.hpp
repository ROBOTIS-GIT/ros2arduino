/*
 * subscriber.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_SUBSCRIBER_HPP_
#define ROS2_SUBSCRIBER_HPP_

#include <stdio.h>
#include "rtps/rtps.hpp"
#include "node_handle.hpp"
#include "topic.hpp"

#ifdef PROFILE_CREATE_ENTITIES_XML
//#define DEFAULT_READER_XML ("<profiles><subscriber profile_name=\"default_xrce_subscriber_profile\"><topic><kind>NO_KEY</kind><name>%s%s</name><dataType>%s</dataType><historyQos><kind>KEEP_LAST</kind><depth>1</depth></historyQos><durability><kind>TRANSIENT_LOCAL</kind></durability></topic></subscriber></profiles>")
#define DEFAULT_READER_XML ("<dds><data_reader><topic><kind>NO_KEY</kind><name>%s%s</name><dataType>%s</dataType></topic></data_reader></dds>")
#endif


namespace ros2
{

class Node;

template <typename MsgT>
class Subscriber:public SubscriberHandle
{

  public:
    Subscriber(rtps::Participant_t* node, const char* name, CallbackFunc callback, void* callback_arg)
      : SubscriberHandle()
    {
      MsgT topic;

      node_ = node;
      name_ = name;
      this->callback = callback;
      this->callback_arg = callback_arg;
      this->recreate();
    }

    void subscribe(void)
    {
      if(is_registered_ ==  false)
      {
        return;
      }

      rtps::subscribe(&subscriber_);
      request_id_ = subscriber_.read_req;
    }

    void runCallback(void* serialized_msg)
    {
      if(this->callback != NULL)
      {
        MsgT msg;
        msg.deserialize((ucdrBuffer*)serialized_msg, &msg);

        this->callback(&msg, this->callback_arg);
      }
    }

    void recreate()
    {
      MsgT topic;

      // char subscriber_profile[100] = {0, };
      // sprintf(subscriber_profile, "<subscriber name=\"%s\"", name_);
      const char* subscriber_profile = "";

      char reader_profile[512] = {0, };
      sprintf(reader_profile, DEFAULT_READER_XML, getPrefixString(TOPICS_SUBSCRIBE), name_, topic.type_);
      is_registered_ = rtps::createSubscriber(node_, &subscriber_, (char*)subscriber_profile, reader_profile);
      this->reader_id_ = subscriber_.reader_id.id;
    };  

  private:
    const char* name_;
    rtps::Participant_t* node_;
    rtps::Subscriber_t subscriber_;
};

} // namespace ros2

#endif /* ROS2_SUBSCRIBER_HPP_ */
