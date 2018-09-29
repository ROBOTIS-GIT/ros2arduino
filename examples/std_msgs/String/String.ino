#include <ros2.hpp>

#include "std_msgs/String.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB

#define STRING_PUBLISH_FREQUENCY 2 //hz

void publishString(std_msgs::String* msg);
void subscribeString(std_msgs::String* msg);


class StringPubSub : public ros2::Node
{
public:
  StringPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<std_msgs::String>("String");
    this->createWallFreq(STRING_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishString, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /String : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<std_msgs::String>("String", (ros2::CallbackFunc)subscribeString);
    DEBUG_SERIAL.print(" [Subscriber Create]  /String : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<std_msgs::String>* publisher_;
  ros2::Subscriber<std_msgs::String>* subscriber_;
};



void setup() 
{
  DEBUG_SERIAL.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  while (!RTPS_SERIAL);

  ros2::init();
}

void loop() 
{
  static uint32_t pre_time = millis();
  static bool led_state = false;
  static StringPubSub StringNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&StringNode);
}


void publishString(std_msgs::String* msg)
{
  static int cnt = 0;
  sprintf(msg->data, "String %d", cnt++);
}


void subscribeString(std_msgs::String* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" String: ");
    DEBUG_SERIAL.println(msg->data);
}
