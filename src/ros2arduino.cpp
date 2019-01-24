/*
 * ros2arduino.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: kei
 */



#include "ros2arduino.h"

bool ros2::init(Stream* comm_instance)
{
  return ros2::init((void*)comm_instance);
}

bool ros2::init(UDP* comm_instance, const char* p_server_ip, uint16_t server_port)
{
  return ros2::init((void*)comm_instance, p_server_ip, server_port, false);
}

bool ros2::init(Client* comm_instance, const char* p_server_ip, uint16_t server_port)
{
  return ros2::init((void*)comm_instance, p_server_ip, server_port, true);
}
