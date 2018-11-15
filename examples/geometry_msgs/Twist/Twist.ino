#include <ros2arduino.h>

#include "geometry_msgs/Twist.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB


#define TWIST_PUBLISH_FREQUENCY 2 //hz

void publishTwist(geometry_msgs::Twist* msg);
void subscribeTwist(geometry_msgs::Twist* msg);


class TwistPubSub : public ros2::Node
{
public:
  TwistPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<geometry_msgs::Twist>("Twist");
    this->createWallFreq(TWIST_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishTwist, NULL, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Twist : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<geometry_msgs::Twist>("Twist", (ros2::CallbackFunc)subscribeTwist, NULL);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Twist : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<geometry_msgs::Twist>* publisher_;
  ros2::Subscriber<geometry_msgs::Twist>* subscriber_;
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
  static TwistPubSub TwistNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&TwistNode);
}



void publishTwist(geometry_msgs::Twist* msg)
{
  msg->linear.x = micros()%128;
  msg->linear.y = micros()%128;
  msg->linear.z = micros()%128;
  msg->angular.x = micros()%180;
  msg->angular.y = micros()%180;
  msg->angular.z = micros()%180;
}


void subscribeTwist(geometry_msgs::Twist* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Linear(x,y,z): ");
    DEBUG_SERIAL.print(msg->linear.x); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->linear.y); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->linear.z);
  DEBUG_SERIAL.print(" Angular(x,y,z): ");
    DEBUG_SERIAL.print(msg->angular.x); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->angular.y); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->angular.z);
}