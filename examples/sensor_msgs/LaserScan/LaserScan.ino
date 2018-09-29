#include <ros2.hpp>

#include "sensor_msgs/LaserScan.hpp"


#define DEBUG_SERIAL SerialBT2  
#define RTPS_SERIAL  Serial   //OpenCR USB


#define LASER_SCAN_PUBLISH_FREQUENCY 200 //hz


void publishLaserScan(sensor_msgs::LaserScan* msg);
void subscribeLaserScan(sensor_msgs::LaserScan* msg);


class LaserScanPubSub : public ros2::Node
{
public:
  LaserScanPubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<sensor_msgs::LaserScan>("LaserScan");
    this->createWallFreq(LASER_SCAN_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishLaserScan, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /LaserScan : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<sensor_msgs::LaserScan>("LaserScan", (ros2::CallbackFunc)subscribeLaserScan);
    DEBUG_SERIAL.print(" [Subscriber Create]  /LaserScan : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }

private:  
  ros2::Publisher<sensor_msgs::LaserScan>* publisher_;
  ros2::Subscriber<sensor_msgs::LaserScan>* subscriber_;
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
  static LaserScanPubSub LaserScanNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&LaserScanNode);
}



void publishLaserScan(sensor_msgs::LaserScan* msg)
{
  float ranges_data_[8];
  float intensities_data_[8];

  strcpy(msg->header.frame_id, "OpenCR LaserScan");
  msg->header.stamp     = ros2::now();
      
  msg->angle_min        = 1;
  msg->angle_max        = 2;
  msg->angle_increment  = 3;
  msg->time_increment   = 4;
  msg->scan_time        = 5;
  msg->range_min        = 6;
  msg->range_max        = 7;

  msg->ranges           = ranges_data_;
  msg->ranges_size      = sizeof(ranges_data_)/sizeof(float);
  for(uint32_t i = 0; i < msg->ranges_size; i++)
  {
    msg->ranges[i]      = (float)(micros()%128);
  }

  msg->intensities      = intensities_data_;
  msg->intensities_size = sizeof(intensities_data_)/sizeof(float);
  for(uint32_t i = 0; i < msg->intensities_size; i++)
  {
    msg->intensities[i] = (float)(micros()%128);
  }
}

void subscribeLaserScan(sensor_msgs::LaserScan* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Header(frameID,sec,nanosec): "); 
    DEBUG_SERIAL.print(msg->header.frame_id); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->header.stamp.sec); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->header.stamp.nanosec);
  DEBUG_SERIAL.print(" Angle(min,max,inc): ");
    DEBUG_SERIAL.print(msg->angle_min); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->angle_max); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->angle_increment);
  DEBUG_SERIAL.print(" Time(inc,scan): ");
    DEBUG_SERIAL.print(msg->time_increment); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->scan_time);
  DEBUG_SERIAL.print(" Range(min,max): ");
    DEBUG_SERIAL.print(msg->range_min); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->range_max);
  DEBUG_SERIAL.print(" Range(data:size): ");
    for(uint32_t i = 0; i < msg->ranges_size; i++)
    {
      DEBUG_SERIAL.print(msg->ranges[i]); DEBUG_SERIAL.print(" ");
    }
  DEBUG_SERIAL.print(": ");
    DEBUG_SERIAL.println(msg->ranges_size);
  DEBUG_SERIAL.print(" Intensity(data:size): ");
    for(uint32_t i = 0; i < msg->intensities_size; i++)
    { 
      DEBUG_SERIAL.print(msg->intensities[i]); DEBUG_SERIAL.print(" ");
    }
  DEBUG_SERIAL.print(": ");
    DEBUG_SERIAL.println(msg->intensities_size);
}
