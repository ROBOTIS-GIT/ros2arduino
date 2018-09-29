#include <ros2.hpp>

#include "std_msgs/Bool.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB

#define BOOL_PUBLISH_FREQUENCY 2 //hz

void publishBool(std_msgs::Bool* msg);
void subscribeBool(std_msgs::Bool* msg);



class BoolPubSub : public ros2::Node
{
public:
  BoolPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<std_msgs::Bool>("Bool");
    this->createWallFreq(BOOL_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishBool, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Bool : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<std_msgs::Bool>("Bool", (ros2::CallbackFunc)subscribeBool);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Bool : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<std_msgs::Bool>* publisher_;
  ros2::Subscriber<std_msgs::Bool>* subscriber_;
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
  static BoolPubSub BoolNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&BoolNode);
}



void publishBool(std_msgs::Bool* msg)
{
  static bool data_ = !data_;
  msg->data = data_;
}


void subscribeBool(std_msgs::Bool* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Bool: ");
    DEBUG_SERIAL.println(msg->data);
}
