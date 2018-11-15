#include <ros2arduino.h>

#include "sensor_msgs/BatteryState.hpp"


#define DEBUG_SERIAL SerialBT2
#define RTPS_SERIAL  Serial    //OpenCR USB

#define BATTERY_STATE_PUBLISH_FREQUENCY 2 //hz


void publishBatteryState(sensor_msgs::BatteryState* msg);
void subscribeBatteryState(sensor_msgs::BatteryState* msg);


class BatteryStatePubSub : public ros2::Node
{
public:
  BatteryStatePubSub()
  : Node()
  {
    DEBUG_SERIAL.println();
    publisher_ = this->createPublisher<sensor_msgs::BatteryState>("BatteryState");
    this->createWallFreq(BATTERY_STATE_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishBatteryState, NULL, publisher_);
    DEBUG_SERIAL.print(" [Publisher Create]   /BatteryState : "); DEBUG_SERIAL.println((publisher_!=NULL?"Success":"Fail"));
    subscriber_ = this->createSubscriber<sensor_msgs::BatteryState>("BatteryState", (ros2::CallbackFunc)subscribeBatteryState, NULL);
    DEBUG_SERIAL.print(" [Subscriber Create]  /BatteryState : "); DEBUG_SERIAL.println((subscriber_!=NULL?"Success":"Fail"));
  }


private:
  ros2::Publisher<sensor_msgs::BatteryState>* publisher_;
  ros2::Subscriber<sensor_msgs::BatteryState>* subscriber_;
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
  static BatteryStatePubSub BatteryStateNode;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  ros2::spin(&BatteryStateNode);
}

void publishBatteryState(sensor_msgs::BatteryState* msg)
{
  static float cell_voltage_data_[3];

  strcpy(msg->header.frame_id, "OpenCR BatteryState");
  msg->header.stamp             = ros2::now();

  msg->voltage                  = 1;
  msg->current                  = 2;
  msg->charge                   = 3;
  msg->capacity                 = 4;
  msg->design_capacity          = 5;
  msg->percentage               = 6;

  msg->power_supply_status      = 0;
  msg->power_supply_health      = 1;
  msg->power_supply_technology  = 2;

  msg->present                  = true;
  msg->cell_voltage             = cell_voltage_data_;
  msg->cell_voltage_size        = sizeof(cell_voltage_data_)/sizeof(float);
  for(uint32_t i = 0; i < msg->cell_voltage_size; i++)
  {
    msg->cell_voltage[i]        = 3.7;
  }

  strcpy(msg->location, "Seoul");
  strcpy(msg->serial_number, "123-456-789");
}


void subscribeBatteryState(sensor_msgs::BatteryState* msg)
{
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print(" Header(frameID,sec,nanosec): "); 
    DEBUG_SERIAL.print(msg->header.frame_id); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->header.stamp.sec); DEBUG_SERIAL.print("."); 
    DEBUG_SERIAL.println(msg->header.stamp.nanosec);
  DEBUG_SERIAL.print(" Value(voltage,current,charge,capacity,design capacity,percentage): ");
    DEBUG_SERIAL.print(msg->voltage); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->current); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->charge); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->capacity); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->design_capacity); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->percentage);
  DEBUG_SERIAL.print(" Info(status,health,technology,present): ");
    DEBUG_SERIAL.print(msg->power_supply_status); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->power_supply_health); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.print(msg->power_supply_technology); DEBUG_SERIAL.print(","); 
    DEBUG_SERIAL.println(msg->present);
  DEBUG_SERIAL.print(" CellInfo(cell voltage:size): ");
  for(uint32_t i = 0; i < msg->cell_voltage_size; i++)
  {
    DEBUG_SERIAL.print(msg->cell_voltage[i]); DEBUG_SERIAL.print(" "); 
  }
  DEBUG_SERIAL.print(": "); DEBUG_SERIAL.println(msg->cell_voltage_size);
  DEBUG_SERIAL.print(" Location: "); DEBUG_SERIAL.println(msg->location);
  DEBUG_SERIAL.print(" Serial Number "); DEBUG_SERIAL.println(msg->serial_number);
}
