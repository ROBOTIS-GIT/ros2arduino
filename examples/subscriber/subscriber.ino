#include <ros2arduino.h>

#ifndef LED_BUILTIN // To support some boards (eg. some esp32 boards)
#define LED_BUILTIN 13
#endif 

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
  : Node("ros2arduino_sub_node")
  {
    this->createSubscriber<std_msgs::Bool>("arduino_led", (ros2::CallbackFunc)subscribeLed, nullptr);
  }
};

void setup() 
{
  XRCEDDS_PORT.begin(115200);
  while (!XRCEDDS_PORT); 

  ros2::init(&XRCEDDS_PORT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  static LedSub LedNode;

  ros2::spin(&LedNode);
}