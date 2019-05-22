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

#include <ros2_user_config.h>

#define ROS2_PUBLISHER_MAX  USER_ROS2_PUBLISHER_MAX
#define ROS2_SUBSCRIBER_MAX USER_ROS2_SUBSCRIBER_MAX

namespace ros2 {

void runNodeSubUserCallback(uint16_t id, void* msgs, void* args);

class Node
{
  public:
    Node();
    virtual ~Node(){};

    void recreate();
    void createWallTimer(uint32_t msec, CallbackFunc callback, void* callback_arg, PublisherHandle* pub);
    void createWallFreq(uint32_t hz, CallbackFunc callback, void* callback_arg, PublisherHandle* pub);
    void runPubCallback();
    void runSubCallback(uint16_t reader_id, void* serialized_msg);

    void deletePublisher(const char* name);
    void deletePublisher(uint16_t writer_id);
    void deleteSubscriber(const char* name);
    void deleteSubscriber(uint16_t reader_id);

    template <
      typename MsgT>
    Publisher<MsgT>* createPublisher(const char* name)
    {
      bool ret = false;
      ros2::Publisher<MsgT> *p_pub = nullptr;

      if (this->node_register_state_ == false)
      {
        return nullptr;
      }

      if (pub_cnt_ >= ROS2_PUBLISHER_MAX)
      {
        return nullptr;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>();

      if (ret == false)
      {
        return nullptr;
      }

      p_pub = new ros2::Publisher<MsgT>(&this->xrcedds_publisher_, name);

      if (p_pub->is_registered_ == false)
      {
        delete (p_pub);
        return nullptr;
      }

      for (uint8_t i = 0; i < ROS2_PUBLISHER_MAX; i++)
      {
        if (pub_list_[i] == nullptr)
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
      ros2::Subscriber<MsgT> *p_sub = nullptr;

      if(this->node_register_state_ == false)
      {
        return nullptr;
      }

      if(sub_cnt_ >= ROS2_SUBSCRIBER_MAX)
      {
        return nullptr;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>();

      if (ret == false)
      {
        return nullptr;
      }

      p_sub = new ros2::Subscriber<MsgT>(&this->xrcedds_subscriber_, name, callback, callback_arg);

      if(p_sub->is_registered_ == false)
      {
        delete(p_sub);
        return nullptr;
      }

      for(uint8_t i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
      {
        if(sub_list_[i] == nullptr)
        {
          sub_list_[i] = p_sub;
          sub_cnt_++;
          p_sub->subscribe();
          break;
        }
      }

      return p_sub;
    }

  private:
    PublisherHandle*  pub_list_[ROS2_PUBLISHER_MAX];
    SubscriberHandle* sub_list_[ROS2_SUBSCRIBER_MAX];

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

      ret = xrcedds::registerTopic(&this->xrcedds_participant_, topic.name_, topic.type_, topic.id_);

      return ret;
    }
};


bool init(void* serial_dev);
bool init(void* comm_instance, const char* p_server_ip, uint16_t server_port, bool enable_tcp);
void spin(Node *node);
void syncTimeFromRemote(builtin_interfaces::Time* time);
builtin_interfaces::Time now();

} /* namespace ros2 */



#endif /* ROS2_H_ */
