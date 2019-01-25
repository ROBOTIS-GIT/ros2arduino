#include <ros2arduino.h>

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