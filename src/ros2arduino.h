/*
 * ros2arduino.h
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_HPP_
#define ROS2_HPP_

#include <string.h>
#include "publisher.hpp"
#include "subscriber.hpp"
#include "topic.hpp"
#include "msg_list.hpp"
#include <Arduino.h>

#define ROS2_PUBLISHER_MAX  20
#define ROS2_SUBSCRIBER_MAX 20


namespace ros2 {

void onTopicCallback(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, ucdrBuffer* mb, void* args);

extern char* client_communication_method;
extern char* server_ip;
extern uint16_t server_port;
extern Stream* serial_device;

class Node
{
  public:
    uint8_t err_code;

    Node()
    {
      err_code = 0, pub_cnt_ = 0, sub_cnt_ = 0, node_register_state_ = false;
      memset(pub_list_, 0, sizeof(pub_list_));
      memset(sub_list_, 0, sizeof(sub_list_));
      this->recreate();
    }

    virtual ~Node(){};

    void recreate()
    {
      err_code = 0;
      participant_.is_init = false;
      if(strcmp(client_communication_method, "Serial") == 0)
      {
        transport_.type = 0;
        transport_.serial_device = (void*) serial_device;
      }
      else
      {
        transport_.type = 1;
        transport_.server_ip = (const char*)server_ip;
        transport_.server_port = server_port;
      }

      rtps::initTransportAndSession(&transport_, onTopicCallback, (void*)this);

//      const char* participant_xml = "<dds>"
//                                        "<participant>"
//                                            "<rtps>"
//                                                "<name>default_xrce_participant</name>"
//                                            "</rtps>"
//                                        "</participant>"
//                                    "</dds>";

      node_register_state_ = rtps::createParticipant(&this->participant_, "default_xrce_participant_profile");

      uint8_t i;
      for(i = 0; i < ROS2_PUBLISHER_MAX; i++)
      {
        if(pub_list_[i] != NULL)
        {
          pub_list_[i]->recreate();
        }
      }

      for(i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
      {
        if(sub_list_[i] != NULL)
        {
          sub_list_[i]->recreate();
        }
      }
    }

    template <
      typename MsgT>
    Publisher<MsgT>* createPublisher(const char* name)
    {
      bool ret = false;
      ros2::Publisher<MsgT> *p_pub = NULL;

      if(this->node_register_state_ == false)
      {
        err_code = 1;
        return NULL;
      }

      if(pub_cnt_ >= ROS2_PUBLISHER_MAX)
      {
        return NULL;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>(name, TOPICS_PUBLISH);

      if (ret == false)
      {
        err_code = 2;
        return NULL;
      }

      p_pub = new ros2::Publisher<MsgT>(&this->participant_, name);

      if(p_pub->is_registered_ == false)
      {
        err_code = 3;
        delete(p_pub);
        return NULL;
      }

      for(uint8_t i = 0; i < ROS2_PUBLISHER_MAX; i++)
      {
        if(pub_list_[i] == NULL)
        {
          pub_list_[i] = p_pub;
          pub_cnt_++;
          break;
        }
      }

      err_code = 0;

      return p_pub;
    }

    template <
      typename MsgT>
    Subscriber<MsgT>* createSubscriber(const char* name, CallbackFunc callback, void* callback_arg)
    {
      bool ret = false;
      ros2::Subscriber<MsgT> *p_sub = NULL;
      
      if(this->node_register_state_ == false)
      {
        err_code = 10 + 1;
        return NULL;
      }

      if(sub_cnt_ >= ROS2_SUBSCRIBER_MAX)
      {
        return NULL;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>(name, TOPICS_SUBSCRIBE);

      if (ret == false)
      {
        err_code = 10 + 2;
        return NULL;
      }

      p_sub = new ros2::Subscriber<MsgT>(&this->participant_, name, callback, callback_arg);

      if(p_sub->is_registered_ == false)
      {
        err_code = 10 + 3;
        delete(p_sub);
        return NULL;
      }

      for(uint8_t i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
      {
        if(sub_list_[i] == NULL)
        {
          sub_list_[i] = p_sub;
          sub_cnt_++;
          p_sub->subscribe();
          break;
        }
      }

      err_code = 0;

      return p_sub;
    }

    void deleteWriter(uint8_t writer_id)
    {
      for(uint8_t i = 0; i < ROS2_PUBLISHER_MAX; i++)
      {
        if(pub_list_[i] != NULL && pub_list_[i]->writer_id_ == writer_id)
        {
          uxrObjectId obj_id = {pub_list_[i]->writer_id_, UXR_DATAWRITER_ID};
          uxr_buffer_delete_entity(participant_.session, participant_.output_stream_id, obj_id);
          delete(pub_list_[i]);
          pub_list_[i] = NULL;
          pub_cnt_--;
          break;
        }
      }
    }

    void deleteReader(uint8_t reader_id)
    {
      for(uint8_t i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
      {
        if(sub_list_[i] != NULL && sub_list_[i]->reader_id_ == reader_id)
        {
          uxrObjectId obj_id = {sub_list_[i]->reader_id_, UXR_DATAREADER_ID};
          uxr_buffer_delete_entity(participant_.session, participant_.output_stream_id, obj_id);
          delete(sub_list_[i]);
          sub_list_[i] = NULL;
          sub_cnt_--;
          break;
        }
      }
    }

    void createWallTimer(uint32_t msec, CallbackFunc callback, void* callback_arg, PublisherHandle* pub)
    {
      if(pub == NULL)
      {
        return;
      }

      pub->setInterval(msec);
      pub->callback = callback;
      pub->callback_arg = callback_arg;
    }

    void createWallFreq(uint32_t hz, CallbackFunc callback, void* callback_arg, PublisherHandle* pub)
    {
      uint32_t msec;
      if(hz > 1000)
      {
        hz = 1000;
      }
      msec = (uint32_t)(1000/hz);
      this->createWallTimer(msec, callback, callback_arg, pub);
    }

    void runPubCallback()
    {
      uint8_t i;
      ros2::PublisherHandle *p_pub;
      for(i = 0; i < ROS2_PUBLISHER_MAX; i++)
      {
        p_pub = pub_list_[i];
        if(p_pub != NULL && p_pub->is_registered_ && p_pub->isTimeToPublish())
        {
          p_pub->publish();
          break;
        }
      }
    }

    void runSubCallback(uint8_t reader_id, void* serialized_msg)
    {
      uint8_t i;
      ros2::SubscriberHandle *p_sub;
      for(i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
      {
        p_sub = sub_list_[i];
        if(p_sub != NULL && p_sub->is_registered_ && p_sub->reader_id_ == reader_id)
        {
          p_sub->runCallback(serialized_msg);
          //p_sub->subscribe();
        }
      }
    }

    PublisherHandle*  pub_list_[ROS2_PUBLISHER_MAX];
    SubscriberHandle* sub_list_[ROS2_SUBSCRIBER_MAX];


  private:
    bool node_register_state_;
    rtps::Participant_t participant_;
    rtps::Transport_t transport_;
    uint8_t pub_cnt_;
    uint8_t sub_cnt_;

    template <
      typename MsgT>
    bool registerTopic(const char* name, MessagePrefix prefix)
    {
      bool ret;
      MsgT topic;

      if(this->node_register_state_ == false)
      {
        return false;
      }

      char topic_profile[256] = {0, };
      sprintf(topic_profile, DEFAULT_TOPIC_XML, getPrefixString(prefix), name, topic.type_);
      ret = rtps::registerTopic(&this->participant_, topic_profile, topic.id_);

      return ret;
    }
};


bool init(Stream* serial_dev);
bool init(const char* p_server_ip, uint16_t server_port);
void spin(Node *node);
void syncTimeFromRemote(builtin_interfaces::Time* time);
builtin_interfaces::Time now();


} /* namespace ros2 */



#endif /* ROS2_HPP_ */
