#include <ros2.hpp>

#include "geometry_msgs/Vector3.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB

#define VECTOR3_PUBLISH_FREQUENCY 2 //hz

void publishVector3(geometry_msgs::Vector3* msg);
void subscribeVector3(geometry_msgs::Vector3* msg);


class Vector3PubSub : public ros2::Node
{
public:
  Vector3PubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<geometry_msgs::Vector3>("Vector3");
    this->createWallFreq(VECTOR3_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishVector3, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Vector3 : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<geometry_msgs::Vector3>("Vector3", (ros2::CallbackFunc)subscribeVector3);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Vector3 : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<geometry_msgs::Vector3>* publisher_;
  ros2::Subscriber<geometry_msgs::Vector3>* subscriber_;
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
  static Vector3PubSub Vector3Node;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&Vector3Node);
}




void publishVector3(geometry_msgs::Vector3* msg)
{
    msg->x = millis()%128;
    msg->y = millis()%128;
    msg->z = millis()%128;
}


void subscribeVector3(geometry_msgs::Vector3* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Vector3(x,y,z): ");
    DEBUG_SERIAL.print(msg->x); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->y); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->z);
}
