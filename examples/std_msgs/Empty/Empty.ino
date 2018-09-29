#include <ros2.hpp>

#include "std_msgs/Empty.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB

#define EMPTY_PUBLISH_FREQUENCY 2 //hz

void publishEmpty(std_msgs::Empty* msg);
void subscribeEmpty(std_msgs::Empty* msg);



class EmptyPubSub : public ros2::Node
{
public:
  EmptyPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<std_msgs::Empty>("Empty");
    this->createWallFreq(EMPTY_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishEmpty, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Empty : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<std_msgs::Empty>("Empty", (ros2::CallbackFunc)subscribeEmpty);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Empty : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<std_msgs::Empty>* publisher_;
  ros2::Subscriber<std_msgs::Empty>* subscriber_;
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
  static EmptyPubSub EmptyNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&EmptyNode);
}



void publishEmpty(std_msgs::Empty* msg)
{
  (void)(msg);
}


void subscribeEmpty(std_msgs::Empty* msg)
{
  (void)(msg);
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Empty !!");
}
