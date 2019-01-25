/*
 * ros2.hpp
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

#define ROS2_PUBLISHER_MAX  10
#define ROS2_SUBSCRIBER_MAX 10

namespace ros2 {

extern xrcedds::XrceDdsCommportType client_communication_method;
extern const char* server_ip;
extern uint16_t server_port;
extern void* comm_instance;

void runNodeSubUserCallback(uint16_t id, void* msgs, void* args);

class Node
{
  public:
    Node()
    {
      pub_cnt_ = 0, sub_cnt_ = 0, node_register_state_ = false;
      memset(pub_list_, 0, sizeof(pub_list_));
      memset(sub_list_, 0, sizeof(sub_list_));
      this->recreate();
    }

    virtual ~Node(){};

    void recreate()
    {
      xrcedds_participant_.is_created = false;
      xrcedds_transport_.type = client_communication_method;

      switch(xrcedds_transport_.type)
      {
        case xrcedds::XRCE_DDS_COMM_SERIAL:
          break;

        case xrcedds::XRCE_DDS_COMM_UDP:
        case xrcedds::XRCE_DDS_COMM_TCP:
          xrcedds_transport_.server_ip = server_ip;
          xrcedds_transport_.server_port = server_port;
          break;                    

        default:
          return;   
      }

      xrcedds_transport_.comm_instance = comm_instance;

      xrcedds::init(0);
      xrcedds::initTransportAndSession(&xrcedds_transport_, (void*)runNodeSubUserCallback, (void*)this);

      node_register_state_ = xrcedds::createParticipant(&this->xrcedds_participant_, "default_xrce_participant");

      xrcedds::createPublisher(&this->xrcedds_participant_, &this->xrcedds_publisher_);
      xrcedds::createSubscriber(&this->xrcedds_participant_, &this->xrcedds_subscriber_);

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
        return NULL;
      }

      if(pub_cnt_ >= ROS2_PUBLISHER_MAX)
      {
        return NULL;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>();

      if (ret == false)
      {
        return NULL;
      }

      p_pub = new ros2::Publisher<MsgT>(&this->xrcedds_publisher_, name);

      if(p_pub->is_registered_ == false)
      {
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
        return NULL;
      }

      if(sub_cnt_ >= ROS2_SUBSCRIBER_MAX)
      {
        return NULL;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>();

      if (ret == false)
      {
        return NULL;
      }

      p_sub = new ros2::Subscriber<MsgT>(&this->xrcedds_subscriber_, name, callback, callback_arg);

      if(p_sub->is_registered_ == false)
      {
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

      return p_sub;
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

    void runSubCallback(uint16_t reader_id, void* serialized_msg)
    {
      uint8_t i;
      ros2::SubscriberHandle *p_sub;
      for(i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
      {
        p_sub = sub_list_[i];
        if(p_sub != NULL && p_sub->is_registered_ && p_sub->reader_id_ == reader_id)
        {
          p_sub->runCallback(serialized_msg);
        }
      }
    }

    PublisherHandle*  pub_list_[ROS2_PUBLISHER_MAX];
    SubscriberHandle* sub_list_[ROS2_SUBSCRIBER_MAX];


  private:
    bool node_register_state_;
    xrcedds::Transport_t xrcedds_transport_;
    xrcedds::Participant_t xrcedds_participant_;
    xrcedds::Publisher_t xrcedds_publisher_;
    xrcedds::Subscriber_t xrcedds_subscriber_;
    uint8_t pub_cnt_;
    uint8_t sub_cnt_;

    template <
      typename MsgT>
    bool registerTopic()
    {
      bool ret;
      MsgT topic;

      if(this->node_register_state_ == false)
      {
        return false;
      }

      ret = xrcedds::registerTopic(&this->xrcedds_participant_, topic.name_, topic.type_);

      return ret;
    }
};


bool init(void* serial_dev);
bool init(void* comm_instance, const char* p_server_ip, uint16_t server_port, bool enable_tcp);
void spin(Node *node);
void syncTimeFromRemote(builtin_interfaces::Time* time);
builtin_interfaces::Time now();

} /* namespace ros2 */



#endif /* ROS2ARDUINO_H_ */
