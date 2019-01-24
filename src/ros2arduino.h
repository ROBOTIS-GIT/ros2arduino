/*
 * ros2arduino.h
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2ARDUINO_H_
#define ROS2ARDUINO_H_

#include <Arduino.h>
#include <Udp.h>
#include <Client.h>

#include <ros2/ros2.hpp>

namespace ros2 {

bool init(Stream* serial_dev);
bool init(UDP* comm_instance, const char* p_server_ip, uint16_t server_port);
bool init(Client* comm_instance, const char* p_server_ip, uint16_t server_port);

} /* namespace ros2 */



#endif /* ROS2ARDUINO_H_ */
