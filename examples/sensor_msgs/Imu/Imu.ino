#include <ros2.hpp>

#include "sensor_msgs/Imu.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB

#define IMU_PUBLISH_FREQUENCY 200 //hz

void publishImu(sensor_msgs::Imu* msg);
void subscribeImu(sensor_msgs::Imu* msg);


class ImuPubSub : public ros2::Node
{
public:
  ImuPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<sensor_msgs::Imu>("Imu");
    this->createWallFreq(IMU_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishImu, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /Imu : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<sensor_msgs::Imu>("Imu", (ros2::CallbackFunc)subscribeImu);
    DEBUG_SERIAL.print(" [Subscriber Create]  /Imu : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<sensor_msgs::Imu>* publisher_;
  ros2::Subscriber<sensor_msgs::Imu>* subscriber_;
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
  static ImuPubSub ImuNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&ImuNode);
}



void publishImu(sensor_msgs::Imu* msg)
{
    strcpy(msg->header.frame_id, "OpenCR IMU");
    msg->header.stamp          = ros2::now();
    
    msg->orientation.x         = 1;
    msg->orientation.y         = 2;
    msg->orientation.z         = 3;
    msg->orientation.w         = 4;
    msg->angular_velocity.x    = 5;
    msg->angular_velocity.y    = 6;
    msg->angular_velocity.z    = 7;
    msg->linear_acceleration.x = 8;
    msg->linear_acceleration.y = 9;
    msg->linear_acceleration.z = 10;
}


void subscribeImu(sensor_msgs::Imu* msg)
{
  DEBUG_SERIAL.println(); 
  DEBUG_SERIAL.print(" Header(frameID,sec,nanosec): "); 
    DEBUG_SERIAL.print(msg->header.frame_id); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->header.stamp.sec); DEBUG_SERIAL.print(","); 
  DEBUG_SERIAL.println(msg->header.stamp.nanosec);
  DEBUG_SERIAL.print(" Orientation(x,y,z,w): ");
    DEBUG_SERIAL.print(msg->orientation.x); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->orientation.y); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->orientation.z); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->orientation.w); 
  DEBUG_SERIAL.print(" Angular velocity(x,y,z): ");
    DEBUG_SERIAL.print(msg->angular_velocity.x); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->angular_velocity.y); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->angular_velocity.z); 
  DEBUG_SERIAL.print(" Linear acceleration(x,y,z): ");
    DEBUG_SERIAL.print(msg->linear_acceleration.x); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->linear_acceleration.y); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->linear_acceleration.z); 
}