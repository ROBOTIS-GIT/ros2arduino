#include <ros2.hpp>

#include "std_msgs/Header.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB

#define HEADER_PUBLISH_FREQUENCY 2 //hz

void publishHeader(std_msgs::Header* msg);
void subscribeHeader(std_msgs::Header* msg);


class HeaderPubSub : public ros2::Node
{
public:
  HeaderPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<std_msgs::Header>("Header");
    this->createWallFreq(HEADER_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishHeader, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Header : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<std_msgs::Header>("Header", (ros2::CallbackFunc)subscribeHeader);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Header : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));  
  }

private:  
  ros2::Publisher<std_msgs::Header>* publisher_;
  ros2::Subscriber<std_msgs::Header>* subscriber_;
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
  static HeaderPubSub HeaderNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&HeaderNode);
}


void publishHeader(std_msgs::Header* msg)
{
  msg->stamp    = ros2::now();
  strcpy(msg->frame_id, "OpenCR Frame ID");
}


void subscribeHeader(std_msgs::Header* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Header(frame_id,sec,nanosec): ");
    DEBUG_SERIAL.print(msg->frame_id); DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.print(msg->stamp.sec); DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(msg->stamp.nanosec); 
}
