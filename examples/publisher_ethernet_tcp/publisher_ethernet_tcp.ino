#include <ros2arduino.h>

#include <Ethernet.h>
#define AGENT_IP   "AGENT_IP_ADDRESS"
#define AGENT_PORT 2018 //AGENT port number

#define PUBLISH_FREQUENCY 2 //hz

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac_address[6] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

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

EthernetClient client;

void setup() 
{
  Ethernet.begin(mac_address);
  while(Ethernet.linkStatus() != LinkON);

  ros2::init(&client, AGENT_IP, AGENT_PORT);
}

void loop() 
{
  static StringPub StringNode;

  ros2::spin(&StringNode);
}