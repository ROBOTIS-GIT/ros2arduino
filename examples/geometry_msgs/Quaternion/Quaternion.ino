#include <ros2arduino.h>

#include "geometry_msgs/Quaternion.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB


#define QUATERNION_PUBLISH_FREQUENCY 2 //hz

void publishQuaternion(geometry_msgs::Quaternion* msg);
void subscribeQuaternion(geometry_msgs::Quaternion* msg);


class QuaternionPubSub : public ros2::Node
{
public:
  QuaternionPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<geometry_msgs::Quaternion>("Quaternion");
    this->createWallFreq(QUATERNION_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishQuaternion, NULL, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Quaternion : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<geometry_msgs::Quaternion>("Quaternion", (ros2::CallbackFunc)subscribeQuaternion, NULL);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Quaternion : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }


private:  
  ros2::Publisher<geometry_msgs::Quaternion>* publisher_;
  ros2::Subscriber<geometry_msgs::Quaternion>* subscriber_;
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
  static QuaternionPubSub QuaternionNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&QuaternionNode);
}



void publishQuaternion(geometry_msgs::Quaternion* msg, void* arg)
{
  (void)(arg);

  msg->x = micros()%128;
  msg->y = micros()%128;
  msg->z = micros()%128;
  msg->w = micros()%180;
}


void subscribeQuaternion(geometry_msgs::Quaternion* msg, void* arg)
{
  (void)(arg);

  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Quaternion(x,y,z,w): ");
    DEBUG_SERIAL.print(msg->x); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->y); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->z); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->w);
}