/*
 * replier.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_REPLIER_HPP_
#define ROS2_REPLIER_HPP_

#include <stdio.h>
#include "xrcedds/xrcedds.hpp"
#include "node_handle.hpp"
#include "topic.hpp"


namespace ros2
{

class Node;

template <
    typename MsgST, typename MsgPT>
class Replier : public ReplierHandle {

  public:
    Replier(xrcedds::Participant_t* participant, const char* name, CallbackReplFunc callback, void* callback_arg)
          : ReplierHandle()
      {
        name_ = name;
        participant_ = participant;
        this->callback = callback;
        this->callback_arg = callback_arg;
        this->recreate();
    }

    void subscribe(void)
    {
      if (is_registered_ == false) {
        return;
      }

      xrcedds::readReplierData(&replier_);
      request_id_ = replier_.request_id;
    }

    void runCallback(void* serialized_msg, void* sample_id)
    {
      if (this->callback != nullptr) {
        uint8_t reply_buffer[topic_response_.size_of_topic(&topic_response_, 0)] = { 0 };
        ucdrBuffer mb;

        topic_request_.deserialize((ucdrBuffer*)serialized_msg, &topic_request_);
        ucdr_init_buffer(&mb, reply_buffer, topic_response_.size_of_topic(&topic_response_, 0));

        this->callback(&topic_request_, &topic_response_, this->callback_arg);

        topic_response_.serialize(&mb, &topic_response_);
        xrcedds::replyData(&replier_, sample_id, reply_buffer, topic_response_.size_of_topic(&topic_response_, 0));
      }
    }

    void recreate()
    {
#if (UXR_CREATE_ENTITIES_USING_REF)
      is_registered_ = xrcedds::createReplier(participant_, &replier_, nullptr, (char*)name_,
          nullptr, nullptr, topic_request_.type_, topic_response_.type_);
#else
      char profile_name[64];
      char service_name[64];
      char reader_name[64];
      char writer_name[64];
      sprintf(profile_name, "%s_requester", name_); // this doesn't seem to have any effect on ros
      sprintf(service_name, "%s_service", name_); // this doesn't seem to have any effect on ros
      sprintf(reader_name, "%s/%sRequest", getPrefixString(SERVICE_REQUEST), name_);
      sprintf(writer_name, "%s/%sReply", getPrefixString(SERVICE_RESPONSE), name_);
      is_registered_ = xrcedds::createReplier(participant_, &replier_, profile_name, service_name,
          reader_name, writer_name, topic_request_.type_, topic_response_.type_);
#endif      
      this->replier_id_ = replier_.id;
    };  

    void destroy(void)
    {
      xrcedds::deleteEntity(&replier_);
    }

  private:
    MsgST topic_request_;
    MsgPT topic_response_;
    xrcedds::Participant_t* participant_;
    xrcedds::Replier_t replier_;
};

} // namespace ros2

#endif /* ROS2_REPLIER_HPP_ */
