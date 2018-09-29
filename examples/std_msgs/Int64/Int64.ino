#include <ros2.hpp>

#include "std_msgs/Int64.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB

#define INT64_PUBLISH_FREQUENCY 2 //hz

void publishInt64(std_msgs::Int64* msg);
void subscribeInt64(std_msgs::Int64* msg);



class Int64PubSub : public ros2::Node
{
public:
  Int64PubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<std_msgs::Int64>("Int64");
    this->createWallFreq(INT64_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishInt64, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Int64 : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<std_msgs::Int64>("Int64", (ros2::CallbackFunc)subscribeInt64);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Int64 : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<std_msgs::Int64>* publisher_;
  ros2::Subscriber<std_msgs::Int64>* subscriber_;
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
  static Int64PubSub Int64Node;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&Int64Node);
}



void publishInt64(std_msgs::Int64* msg)
{
  static int64_t i = 0;
  msg->data = i++;
}


void subscribeInt64(std_msgs::Int64* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Int64: ");
    DEBUG_SERIAL.print((int32_t)(msg->data >> 32));
    DEBUG_SERIAL.println((int32_t)(msg->data));
}
