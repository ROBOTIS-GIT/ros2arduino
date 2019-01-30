/*
 * subscriber.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_SUBSCRIBER_HPP_
#define ROS2_SUBSCRIBER_HPP_

#include <stdio.h>
#include "xrcedds/xrcedds.hpp"
#include "node_handle.hpp"
#include "topic.hpp"


namespace ros2
{

class Node;

template <typename MsgT>
class Subscriber:public SubscriberHandle
{

  public:
    Subscriber(xrcedds::Subscriber_t* subscriber, const char* name, CallbackFunc callback, void* callback_arg)
      : SubscriberHandle()
    {
      name_ = name;
      subscriber_ = subscriber;
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

      xrcedds::readData(&data_reader_);
      request_id_ = data_reader_.request_id;
    }

    void runCallback(void* serialized_msg)
    {
      if(this->callback != NULL)
      {
        topic_.deserialize((ucdrBuffer*)serialized_msg, &topic_);

        this->callback(&topic_, this->callback_arg);
      }
    }

    void recreate()
    {
      char reader_name[64];
      sprintf(reader_name, "%s/%s", getPrefixString(TOPICS_SUBSCRIBE), name_);
      is_registered_ = xrcedds::createDataReader(subscriber_, &data_reader_, reader_name, topic_.type_);
      this->reader_id_ = data_reader_.id;
    };  

    void destroy(void)
    {
      xrcedds::deleteEntity(&data_reader_);
    }

  private:
    MsgT topic_;
    xrcedds::Subscriber_t* subscriber_;
    xrcedds::DataReader_t data_reader_;
};

} // namespace ros2

#endif /* ROS2_SUBSCRIBER_HPP_ */
