#include <ros2arduino.h>

#define RTPS_SERIAL  Serial


struct ucdrBuffer;

/* String topic information */
typedef struct String_
{
    char data[255];
} String_;

bool String_serialize_topic(struct ucdrBuffer* writer, const String_* topic);
bool String_deserialize_topic(struct ucdrBuffer* reader, String_* topic);
uint32_t String_size_of_topic(const String_* topic, uint32_t size);

/* Bool topic information */
typedef struct Bool
{
    bool data;
} Bool;

bool Bool_serialize_topic(struct ucdrBuffer* writer, const Bool* topic);
bool Bool_deserialize_topic(struct ucdrBuffer* reader, Bool* topic);
uint32_t Bool_size_of_topic(const Bool* topic, uint32_t size);

/* Callback when reader is received topic */
void onTopicUserCallback(uint16_t reader_id, void* msgs, void* args);


xrcedds::Transport_t transport;
xrcedds::Participant_t participant;
xrcedds::Publisher_t publisher;
xrcedds::Subscriber_t subscriber;
xrcedds::DataWriter_t data_writer;
xrcedds::DataReader_t data_reader;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  RTPS_SERIAL.begin(1000000);
   
  while (!RTPS_SERIAL); 

  /* Init transport(Serial) & create session */
  transport.type = xrcedds::XRCE_DDS_COMM_USB;
  xrcedds::init(0);
  xrcedds::initTransportAndSession(&transport, (void*)onTopicUserCallback, NULL);
  
  /* Create participant */
  xrcedds::createParticipant(&participant, "default_xrce_participant");
  
  /* Register topics */
  xrcedds::registerTopic(&participant, "String", "std_msgs::msg::dds_::String_");
  xrcedds::registerTopic(&participant, "Bool", "std_msgs::msg::dds_::Bool_");
  
  /* Create pub & sub */
  xrcedds::createPublisher(&participant, &publisher);
  xrcedds::createSubscriber(&participant, &subscriber);

  xrcedds::createDataWriter(&publisher, &data_writer, (char*)"rt/string", "std_msgs::msg::dds_::String_");
  xrcedds::createDataReader(&subscriber, &data_reader, (char*)"rt/bool", "std_msgs::msg::dds_::Bool_");

  /* Start subscribe */
  xrcedds::read(&data_reader);
}

void loop() 
{
  static uint32_t pre_time = millis();
  static String_ string_topic;
  static uint32_t cnt = 0;

  if(millis() - pre_time > 500)
  {
    pre_time = millis();  
    
    ucdrBuffer buffer;
    xrcedds::write(&data_writer, (void*)&buffer, String_size_of_topic(&string_topic, 0));
    sprintf(string_topic.data, "HELLO ros2arduino %d", (int)cnt++);
    String_serialize_topic(&buffer, &string_topic);
  }
  xrcedds::runCommunication(1);
}



void onTopicUserCallback(uint16_t reader_id, void* msgs, void* args)
{
  (void)(args);

  if(reader_id == data_reader.id)
  {
    Bool topic;
    Bool_deserialize_topic((ucdrBuffer*)msgs, &topic);
    digitalWrite(LED_BUILTIN, topic.data);
  }
}



bool String_serialize_topic(ucdrBuffer* writer, const String_* topic)
{
    (void) ucdr_serialize_string(writer, topic->data);
    return !writer->error;
}

bool String_deserialize_topic(ucdrBuffer* reader, String_* topic)
{
    (void) ucdr_deserialize_string(reader, topic->data, 255);
    return !reader->error;
}

uint32_t String_size_of_topic(const String_* topic, uint32_t size)
{
    uint32_t previousSize = size;
    size += ucdr_alignment(size, 4) + 4 + (uint32_t)strlen(topic->data) + 1;
    return size - previousSize;
}



bool Bool_serialize_topic(ucdrBuffer* writer, const Bool* topic)
{
    (void) ucdr_serialize_bool(writer, topic->data);

    return !writer->error;
}

bool Bool_deserialize_topic(ucdrBuffer* reader, Bool* topic)
{
    (void) ucdr_deserialize_bool(reader, &topic->data);

    return !reader->error;
}

uint32_t Bool_size_of_topic(const Bool* topic, uint32_t size)
{
    (void)(topic);
    uint32_t previousSize = size;
    size += ucdr_alignment(size, 1) + 1;

    return size - previousSize;
}