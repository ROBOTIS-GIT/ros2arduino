/*
 * ros2.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: kei
 */


#include "ros2arduino.h"

namespace ros2 {
  xrcedds::XrceDdsCommportType client_communication_method;
  const char* server_ip;
  uint16_t server_port;
  void* comm_instance;

  static builtin_interfaces::Time synced_time_from_remote;
  static uint32_t millis_when_synced_time;
}

bool ros2::init(void* comm_instance)
{
  if(comm_instance == NULL)
  {
    return false;
  }

  ros2::client_communication_method = xrcedds::XRCE_DDS_COMM_SERIAL;
  ros2::comm_instance = (void*)comm_instance;

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
    ros2::client_communication_method = xrcedds::XRCE_DDS_COMM_UDP;
  }
  else
  {
    ros2::client_communication_method = xrcedds::XRCE_DDS_COMM_TCP;
  }

  ros2::server_ip = p_server_ip;
  ros2::server_port = server_port;
  ros2::comm_instance = (void*)comm_instance;

  return true;
}

void ros2::spin(ros2::Node *node)
{
  node->runPubCallback();

  if(xrcedds::runCommunication(1) == false)
  {
    //node->recreate();
  }
}

void ros2::syncTimeFromRemote(builtin_interfaces::Time* msg)
{
  ros2::millis_when_synced_time         = dds_getMilliseconds();
  ros2::synced_time_from_remote.sec     = msg->sec;
  ros2::synced_time_from_remote.nanosec = msg->nanosec;
}

builtin_interfaces::Time ros2::now()
{
  builtin_interfaces::Time ret_time = synced_time_from_remote;
  uint32_t msec_offset = dds_getMilliseconds() - ros2::millis_when_synced_time;
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