#include <ros2arduino.h>

#include "std_msgs/Bool.hpp"

#define RTPS_SERIAL  Serial 

void subscribeLed(std_msgs::Bool* msg, void* arg)
{
  (void)(arg);

  digitalWrite(LED_BUILTIN, msg->data);
}

class LedSub : public ros2::Node
{
public:
  LedSub()
  : Node()
  {
    this->createSubscriber<std_msgs::Bool>("arduino_led", (ros2::CallbackFunc)subscribeLed, NULL);
  }
};

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  RTPS_SERIAL.begin(1000000);
  ros2::init((Stream*)&RTPS_SERIAL);

  while (!RTPS_SERIAL); 
}

void loop() 
{
  static LedSub LedNode;

  ros2::spin(&LedNode);
}