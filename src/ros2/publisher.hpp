/*
 * publisher.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_PUBLISHER_HPP_
#define ROS2_PUBLISHER_HPP_

#include <stdio.h>
#include "xrcedds/xrcedds.hpp"
#include "node_handle.hpp"
#include "topic.hpp"

namespace ros2 {

template <typename MsgT>
class Publisher:public PublisherHandle
{
 
public:
  Publisher(xrcedds::Publisher_t* publisher, const char* name)
    : PublisherHandle()
  {
    name_ = name;
    publisher_ = publisher;
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

    this->publish(&topic_);
  }

  void publish(MsgT *msg)
  {
    if(is_registered_ ==  false)
    {
      return;
    }

    ucdrBuffer mb;
    xrcedds::writeData(&data_writer_, (void*)&mb, msg->size_of_topic(msg, 0));
    msg->serialize(&mb, msg);
  }

  void recreate()
  {
    char writer_name[64];
    sprintf(writer_name, "%s/%s", getPrefixString(TOPICS_PUBLISH), name_);
    is_registered_ = xrcedds::createDataWriter(publisher_, &data_writer_, writer_name, topic_.type_);
  }

  void destroy(void)
  {
    xrcedds::deleteEntity(&data_writer_);
  }

private:
  MsgT topic_;
  xrcedds::Publisher_t* publisher_;
  xrcedds::DataWriter_t data_writer_;
};


} // namespace ros2



#endif /* ROS2_PUBLISHER_HPP_ */
