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
    subscriber_ = this->createSubscriber<std_msgs::Bool>("Led", (ros2::CallbackFunc)subscribeLed, NULL);
  }

private:  
  ros2::Subscriber<std_msgs::Bool>* subscriber_;
};

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  ros2::init((Stream*)&RTPS_SERIAL);

  while (!RTPS_SERIAL); 
}

void loop() 
{
  static LedSub LedNode;

  ros2::spin(&LedNode);
}