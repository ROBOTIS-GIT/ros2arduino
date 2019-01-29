/*
 * ros2.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: kei
 */

#include "ros2.hpp"


static xrcedds::XrceDdsCommportType g_client_communication_method;
static const char* g_server_ip;
static uint16_t g_server_port;
static void* g_comm_instance;

static builtin_interfaces::Time synced_time_from_remote;
static uint32_t millis_when_synced_time;


bool ros2::init(void* comm_instance)
{
  if(comm_instance == NULL)
  {
    return false;
  }

  g_client_communication_method = xrcedds::XRCE_DDS_COMM_SERIAL;
  g_comm_instance = (void*)comm_instance;

  return true;
}

bool ros2::init(void* comm_instance, const char* p_server_ip, uint16_t server_port, bool enable_tcp)
{
  if(comm_instance == NULL)
  {
    return false;
  }

  if(enable_tcp == false)
  {
    g_client_communication_method = xrcedds::XRCE_DDS_COMM_UDP;
  }
  else
  {
    g_client_communication_method = xrcedds::XRCE_DDS_COMM_TCP;
  }

  g_server_ip = p_server_ip;
  g_server_port = server_port;
  g_comm_instance = (void*)comm_instance;

  return true;
}

void ros2::spin(ros2::Node *node)
{
  if(node == NULL)
    return;

  node->runPubCallback();

  if(xrcedds::runCommunication(1) == false)
  {
    //node->recreate();
  }
}

void ros2::syncTimeFromRemote(builtin_interfaces::Time* msg)
{
  millis_when_synced_time         = dds_getMilliseconds();
  synced_time_from_remote.sec     = msg->sec;
  synced_time_from_remote.nanosec = msg->nanosec;
}

builtin_interfaces::Time ros2::now()
{
  builtin_interfaces::Time ret_time = synced_time_from_remote;
  uint32_t msec_offset = dds_getMilliseconds() - millis_when_synced_time;
  uint32_t remain_msec = msec_offset%1000;

  ret_time.sec += (int32_t)(msec_offset/(uint32_t)1000);
  if((ret_time.nanosec/(uint32_t)1000000 + remain_msec) < 1000)
  {
    ret_time.nanosec += (uint32_t)(remain_msec*(uint32_t)1000000);
  }
  else // >= 1sec
  {
    ret_time.sec += 1;
    ret_time.nanosec = (uint32_t)(ret_time.nanosec + remain_msec*(uint32_t)1000000 - (uint32_t)1000000000);
  }

  return ret_time;
}

const char* ros2::getPrefixString(MessagePrefix prefix)
{
  switch(prefix)
  {
    case ros2::TOPICS_PUBLISH:
      return "rt";

    case ros2::TOPICS_SUBSCRIBE:
      return "rt";

    case ros2::SERVICE_REQUEST:
      return "rq";

    case ros2::SERVICE_RESPONSE:
      return "rr";

    case ros2::SERVICE:
      return "rs";

    case ros2::PARAMETER:
      return "rp";

    case ros2::ACTION:
      return "ra";
  }

  return "";
}


void ros2::runNodeSubUserCallback(uint16_t id, void* msgs, void* args)
{
  ros2::Node* p_node = (ros2::Node*) args;

  if(p_node != NULL)
  {
    p_node->runSubCallback(id, msgs);
  }
}


/* Node class */
ros2::Node::Node()
{
  pub_cnt_ = 0, sub_cnt_ = 0, node_register_state_ = false;
  memset(pub_list_, 0, sizeof(pub_list_));
  memset(sub_list_, 0, sizeof(sub_list_));
  this->recreate();
}

void ros2::Node::recreate()
{
  xrcedds_participant_.is_created = false;
  xrcedds_transport_.type = g_client_communication_method;

  switch (xrcedds_transport_.type)
  {
    case xrcedds::XRCE_DDS_COMM_SERIAL:
      break;

    case xrcedds::XRCE_DDS_COMM_UDP:
    case xrcedds::XRCE_DDS_COMM_TCP:
      xrcedds_transport_.server_ip = g_server_ip;
      xrcedds_transport_.server_port = g_server_port;
      break;

    default:
      return;
  }

  xrcedds_transport_.comm_instance = g_comm_instance;

  xrcedds::init(0);
  xrcedds::initTransportAndSession(&xrcedds_transport_,
      (void*) runNodeSubUserCallback, (void*) this);

  node_register_state_ = xrcedds::createParticipant(&this->xrcedds_participant_,
      "default_xrce_participant");

  xrcedds::createPublisher(&this->xrcedds_participant_,
      &this->xrcedds_publisher_);
  xrcedds::createSubscriber(&this->xrcedds_participant_,
      &this->xrcedds_subscriber_);

  uint8_t i;
  for (i = 0; i < ROS2_PUBLISHER_MAX; i++)
  {
    if (pub_list_[i] != NULL)
    {
      pub_list_[i]->recreate();
    }
  }

  for (i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
  {
    if (sub_list_[i] != NULL)
    {
      sub_list_[i]->recreate();
    }
  }
}

void ros2::Node::createWallTimer(uint32_t msec, CallbackFunc callback, void* callback_arg, PublisherHandle* pub)
{
  if(pub == NULL)
  {
    return;
  }

  pub->setInterval(msec);
  pub->callback = callback;
  pub->callback_arg = callback_arg;
}

void ros2::Node::createWallFreq(uint32_t hz, CallbackFunc callback, void* callback_arg, PublisherHandle* pub)
{
  uint32_t msec;
  if(hz > 1000)
  {
    hz = 1000;
  }
  msec = (uint32_t)(1000/hz);
  this->createWallTimer(msec, callback, callback_arg, pub);
}

void ros2::Node::runPubCallback()
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

void ros2::Node::runSubCallback(uint16_t reader_id, void* serialized_msg)
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

void ros2::Node::deletePublisher(const char* name)
{
  PublisherHandle *publisher;

  for(uint8_t i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
  {
    publisher = pub_list_[i];
    if(publisher != NULL && !strcmp(publisher->name_, name))
    {
      publisher->destroy();
      delete(publisher);
      pub_list_[i] = NULL;
      pub_cnt_--;
      break;
    }
  }
}

void ros2::Node::deletePublisher(uint16_t writer_id)
{
  PublisherHandle *publisher;

  for(uint8_t i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
  {
    publisher = pub_list_[i];
    if(publisher != NULL && publisher->writer_id_ == writer_id)
    {
      publisher->destroy();
      delete(publisher);
      pub_list_[i] = NULL;
      pub_cnt_--;
      break;
    }
  }
}

void ros2::Node::deleteSubscriber(const char* name)
{
  SubscriberHandle *subscriber;

  for(uint8_t i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
  {
    subscriber = sub_list_[i];
    if(subscriber != NULL && !strcmp(subscriber->name_, name))
    {
      subscriber->destroy();
      delete(subscriber);
      sub_list_[i] = NULL;
      sub_cnt_--;
      break;
    }
  }
}

void ros2::Node::deleteSubscriber(uint16_t reader_id)
{
  SubscriberHandle *subscriber;

  for(uint8_t i = 0; i < ROS2_SUBSCRIBER_MAX; i++)
  {
    subscriber = sub_list_[i];
    if(subscriber != NULL && subscriber->reader_id_ == reader_id)
    {
      subscriber->destroy();
      delete(subscriber);
      sub_list_[i] = NULL;
      sub_cnt_--;
      break;
    }
  }
}

