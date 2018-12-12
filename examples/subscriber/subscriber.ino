#include <ros2arduino.h>

#include "std_msgs/Bool.hpp"

#define XRCEDDS_PORT  Serial 

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
  XRCEDDS_PORT.begin(1000000);
  ros2::init(&XRCEDDS_PORT);

  while (!XRCEDDS_PORT); 
}

void loop() 
{
  static LedSub LedNode;

  ros2::spin(&LedNode);
}