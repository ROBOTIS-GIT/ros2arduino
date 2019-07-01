#include <ros2arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#define SSID       "YOUR_SSID"
#define SSID_PW    "YOUR_SSID_PASSWORD"
#define AGENT_IP   "AGENT_IP_ADDRESS"
#define AGENT_PORT 2018 //AGENT port number

#define PUBLISH_FREQUENCY 2 //hz

void publishString(std_msgs::String* msg, void* arg)
{
  (void)(arg);

  static int cnt = 0;
  sprintf(msg->data, "Hello ros2arduino %d", cnt++);
}

class StringPub : public ros2::Node
{
public:
  StringPub()
  : Node("ros2arduino_pub_node")
  {
    ros2::Publisher<std_msgs::String>* publisher_ = this->createPublisher<std_msgs::String>("arduino_chatter");
    this->createWallFreq(PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishString, nullptr, publisher_);
  }
};

WiFiClient client;

void setup() 
{
  WiFi.begin(SSID, SSID_PW);
  while(WiFi.status() != WL_CONNECTED);

  ros2::init(&client, AGENT_IP, AGENT_PORT);
}

void loop() 
{
  static StringPub StringNode;

  ros2::spin(&StringNode);
}