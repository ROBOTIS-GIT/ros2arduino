#include <ros2arduino.h>

#define RTPS_SERIAL  Serial

const uint8_t string_topic_id = 1;
const uint8_t bool_topic_id = 2;

/* XML */
const char* participant_xml = "<dds>"
                                  "<participant>"
                                      "<rtps>"
                                          "<name>default_xrce_participant</name>"
                                      "</rtps>"
                                  "</participant>"
                              "</dds>";

const char* string_topic_xml =  "<dds>"
                                  "<topic>"
                                    "<name>string</name>"
                                    "<dataType>std_msgs::msg::dds_::String_</dataType>"
                                  "</topic>"
                                "</dds>";

const char* bool_topic_xml =  "<dds>"
                                "<topic>"
                                  "<name>bool</name>"
                                  "<dataType>std_msgs::msg::dds_::Bool_</dataType>"
                                "</topic>"
                              "</dds>";                                

const char* datawriter_xml = "<dds>"
                                 "<data_writer>"
                                     "<topic>"
                                         "<kind>NO_KEY</kind>"
                                         "<name>rt/string</name>"
                                         "<dataType>std_msgs::msg::dds_::String_</dataType>"
                                     "</topic>"
                                 "</data_writer>"
                             "</dds>";

const char* datareader_xml = "<dds>"
                                 "<data_reader>"
                                     "<topic>"
                                         "<kind>NO_KEY</kind>"
                                         "<name>rt/bool</name>"
                                         "<dataType>std_msgs::msg::dds_::Bool_</dataType>"
                                     "</topic>"
                                 "</data_reader>"
                             "</dds>";


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
void onTopicCallback(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, ucdrBuffer* mb, void* args);


rtps::Transport_t transport;
rtps::Participant_t participant;
rtps::Publisher_t publisher;
rtps::Subscriber_t subscriber;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  RTPS_SERIAL.begin(1000000);
  
  while (!RTPS_SERIAL); 

  /* Init transport(Serial) & create session */
  transport.type = 0;
  rtps::initTransportAndSession(&transport, onTopicCallback, NULL);
  
  /* Create participant */
  participant.is_init = false;
  rtps::createParticipant(&participant, participant_xml);
  
  /* Register topics */
  rtps::registerTopic(&participant, (char*)string_topic_xml, string_topic_id);
  rtps::registerTopic(&participant, (char*)bool_topic_xml, bool_topic_id);
  
  /* Create pub & sub */
  rtps::createPublisher(&participant, &publisher, (char*)"", (char*)datawriter_xml);
  rtps::createSubscriber(&participant, &subscriber, (char*)"", (char*)datareader_xml);

  /* Start subscribe */
  rtps::subscribe(&subscriber);
  /* Communicate to agent */
  rtps::runCommunication(1000);
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
    rtps::publish(&publisher, &buffer, String_size_of_topic(&string_topic, 0));
    sprintf(string_topic.data, "HELLO ros2arduino %d", (int)cnt++);
    String_serialize_topic(&buffer, &string_topic);
  }
  rtps::runCommunication(1);
}



void onTopicCallback(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, ucdrBuffer* mb, void* args)
{
  (void) session; (void) object_id; (void) request_id; (void) stream_id; (void) args;

  if(object_id.id == bool_topic_id)
  {
    Bool topic;
    Bool_deserialize_topic(mb, &topic);
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