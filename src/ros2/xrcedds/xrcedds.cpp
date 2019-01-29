/*
 *  xrcedds.cpp
 *
 *  Created on: Nov 14, 2018
 *      Author: Kei
 */

#include <stdlib.h>
#include <string.h>

#include "xrcedds.hpp"

//-- Internal Variables for UXR
//
#include "micro_xrce_dds/micro_xrce_dds.h"
#define STREAM_HISTORY 2
#define BUFFER_SIZE    512 * STREAM_HISTORY //UXR_CONFIG_SERIAL_TRANSPORT_MTU
static uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
static uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
static uxr_session_t g_uxr_session;

//-- External Functions
//
void xrcedds::init(uint8_t xrcedds_product)
{
  (void)(xrcedds_product);
  g_uxr_session.is_init = false;
  g_uxr_session.session_key = 0xAABBCCDD;
}

bool xrcedds::initTransportAndSession(Transport_t* transport_info, void* callback_func, void* args)
{
  if(transport_info == NULL)
  {
    return false;
  }

  switch(transport_info->type)
  {
#ifdef PROFILE_SERIAL_TRANSPORT
    case xrcedds::XRCE_DDS_COMM_SERIAL:

      g_uxr_session.platform_serial.serial_instance = transport_info->comm_instance;
      if(uxr_init_serial_transport(&g_uxr_session.transport_serial, &g_uxr_session.platform_serial, 0, 0, 0) == true)
      {
        g_uxr_session.comm_port = &g_uxr_session.transport_serial.comm;
      }
      break;
#endif

#ifdef PROFILE_UDP_TRANSPORT
    case xrcedds::XRCE_DDS_COMM_UDP:
      g_uxr_session.platform_udp.udp_instance = transport_info->comm_instance;
      if(uxr_init_udp_transport(&g_uxr_session.transport_udp, &g_uxr_session.platform_udp, transport_info->server_ip, transport_info->server_port) == true)
      {
        g_uxr_session.comm_port = &g_uxr_session.transport_udp.comm;
      }
      break;
#endif

#ifdef PROFILE_TCP_TRANSPORT
    case xrcedds::XRCE_DDS_COMM_TCP:
      g_uxr_session.platform_tcp.client_instance = transport_info->comm_instance;
      if(uxr_init_tcp_transport(&g_uxr_session.transport_tcp, &g_uxr_session.platform_tcp, transport_info->server_ip, transport_info->server_port) == true)
      {
        g_uxr_session.comm_port = &g_uxr_session.transport_tcp.comm;
      }
      break;    
#endif

    default:
      return false;
  }

  if(g_uxr_session.comm_port != NULL)
  {
    uxr_init_session(&g_uxr_session.session, g_uxr_session.comm_port, g_uxr_session.session_key);
    uxr_set_topic_callback(&g_uxr_session.session, uxr_onTopicCallback, args);
    uxr_setOnTopicUserCallback((uxr_onTopicUserCallback)callback_func);

    g_uxr_session.is_init = uxr_create_session(&g_uxr_session.session);

    g_uxr_session.output_stream_id = uxr_create_output_reliable_stream(&g_uxr_session.session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    g_uxr_session.input_stream_id  = uxr_create_input_reliable_stream(&g_uxr_session.session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    // g_uxr_session.output_stream_id = uxr_create_output_best_effort_stream(&g_uxr_session.session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    // g_uxr_session.input_stream_id  = uxr_create_input_best_effort_stream(&g_uxr_session.session);
  }

  return g_uxr_session.is_init;
}

void xrcedds::deleteTransportAndSession(Transport_t* transport_info)
{
  uxr_delete_session(&g_uxr_session.session);

  switch(transport_info->type)
  {
#ifdef PROFILE_SERIAL_TRANSPORT
    case XRCE_DDS_COMM_SERIAL:
      uxr_close_serial_transport(&g_uxr_session.transport_serial);
      break;
#endif

#ifdef PROFILE_UDP_TRANSPORT
    case XRCE_DDS_COMM_UDP:
      uxr_close_udp_transport(&g_uxr_session.transport_udp);  
      break;
#endif

#ifdef PROFILE_TCP_TRANSPORT
    case XRCE_DDS_COMM_TCP:
      uxr_close_tcp_transport(&g_uxr_session.transport_tcp);  
      break;
#endif

    default:
      break;
  }
}

bool xrcedds::createParticipant(xrcedds::Participant_t* participant, const char* participant_name)
{
  if(participant == NULL || g_uxr_session.is_init == false || participant_name == NULL)
  {
    return false;
  }
  
  uint8_t status;
  uint16_t participant_req;
  char participant_profile[256];
  static uint8_t participant_id = 0x01;

  participant->id            = participant_id++;
  uxrObjectId part_object_id = uxr_object_id(participant->id, UXR_PARTICIPANT_ID);

  sprintf(participant_profile, UXR_PARTICIPANT_XML, participant_name);
  participant_req = uxr_buffer_create_participant_xml(&g_uxr_session.session, g_uxr_session.output_stream_id, part_object_id, 0, participant_profile, UXR_REPLACE);
  //participant_req = uxr_buffer_create_participant_ref(&g_uxr_session.session, g_uxr_session.output_stream_id, participant->id, 0, participant_profile, UXR_REPLACE);

  participant->is_created = uxr_run_session_until_all_status(&g_uxr_session.session, 1000, &participant_req, &status, 1);

  return participant->is_created;
}

bool xrcedds::registerTopic(xrcedds::Participant_t* participant, const char* topic_name, const char* topic_type)
{
  if(participant == NULL || participant->is_created == false || topic_name == NULL || topic_type == NULL)
  {
    return false;
  }

  bool ret = false;
  uint8_t status;
  uint16_t topic_req;
  char topic_profile[256];
  static uint8_t topic_id = 0x01;
  
  uxrObjectId part_object_id = uxr_object_id(participant->id, UXR_PARTICIPANT_ID);
  uxrObjectId topic_object_id = uxr_object_id(topic_id++, UXR_TOPIC_ID);

  sprintf(topic_profile, UXR_TOPIC_XML, topic_name, topic_type);
  topic_req = uxr_buffer_create_topic_xml(&g_uxr_session.session, g_uxr_session.output_stream_id, topic_object_id, part_object_id, topic_profile, UXR_REUSE);
  ret = uxr_run_session_until_all_status(&g_uxr_session.session, 1000, &topic_req, &status, 1);
  if(ret == false && status == UXR_STATUS_ERR_ALREADY_EXISTS)
  {
    ret = true;
  }

  return ret;
}

bool xrcedds::createPublisher(xrcedds::Participant_t* participant, xrcedds::Publisher_t* publisher)
{
  if(participant == NULL || publisher == NULL ||participant->is_created == false)
  {
    return false;
  }

  uint8_t status;
  uint16_t publisher_req;
  static uint16_t pub_id   = 0x0001;
  publisher->is_created    = false;
  publisher->participant   = participant;
  publisher->id            = pub_id++;

  uxrObjectId part_object_id = uxr_object_id(participant->id, UXR_PARTICIPANT_ID);
  uxrObjectId pub_object_id  = uxr_object_id(publisher->id, UXR_PUBLISHER_ID);

  publisher_req = uxr_buffer_create_publisher_xml(&g_uxr_session.session, g_uxr_session.output_stream_id, pub_object_id, part_object_id, "", UXR_REPLACE);
  publisher->is_created = uxr_run_session_until_all_status(&g_uxr_session.session, 1000, &publisher_req, &status, 1);

  return publisher->is_created;
}

bool xrcedds::createDataWriter(xrcedds::Publisher_t* publisher, xrcedds::DataWriter_t* data_writer, char* writer_name, const char* topic_type)
{
  if(publisher == NULL || data_writer == NULL || writer_name == NULL || topic_type == NULL)
  {
    return false;
  }

  uint8_t status;
  uint16_t datawriter_req;
  char writer_profile[256];
  static uint8_t writer_id = 0x01;

  data_writer->publisher = publisher;
  data_writer->id        = writer_id++;

  uxrObjectId pub_object_id  = uxr_object_id(publisher->id, UXR_PUBLISHER_ID);
  uxrObjectId writer_object_id = uxr_object_id(data_writer->id, UXR_DATAWRITER_ID);

  sprintf(writer_profile, UXR_WRITER_XML, writer_name, topic_type);
  datawriter_req = uxr_buffer_create_datawriter_xml(&g_uxr_session.session, g_uxr_session.output_stream_id, writer_object_id, pub_object_id, writer_profile, UXR_REPLACE);

  data_writer->is_created = uxr_run_session_until_all_status(&g_uxr_session.session, 1000, &datawriter_req, &status, 1);

  return data_writer->is_created;
}

bool xrcedds::createSubscriber(xrcedds::Participant_t* participant, xrcedds::Subscriber_t* subscriber)
{
  if(participant == NULL || subscriber == NULL || participant->is_created == false)
  {
    return false;
  }

  uint8_t status;
  uint16_t subscriber_req;
  static uint8_t sub_id     = 0x01;
  subscriber->is_created    = false;
  subscriber->participant   = participant;
  subscriber->id            = sub_id++;
  
  uxrObjectId part_object_id = uxr_object_id(participant->id, UXR_PARTICIPANT_ID);
  uxrObjectId sub_object_id  = uxr_object_id(subscriber->id, UXR_SUBSCRIBER_ID);

  subscriber_req = uxr_buffer_create_subscriber_xml(&g_uxr_session.session, g_uxr_session.output_stream_id, sub_object_id, part_object_id, "", UXR_REPLACE);
  subscriber->is_created = uxr_run_session_until_all_status(&g_uxr_session.session, 1000, &subscriber_req, &status, 1);

  return subscriber->is_created;
}

bool xrcedds::createDataReader(xrcedds::Subscriber_t* subscriber, xrcedds::DataReader_t* data_reader, char* reader_name, const char* topic_type)
{
  if(subscriber == NULL || data_reader == NULL || reader_name == NULL || topic_type == NULL)
  {
    return false;
  }

  uint8_t status;
  uint16_t datareader_req;
  char reader_profile[256];
  static uint8_t reader_id = 0x01;

  data_reader->subscriber = subscriber;
  data_reader->id         = reader_id++;

  uxrObjectId sub_object_id  = uxr_object_id(subscriber->id, UXR_SUBSCRIBER_ID);
  uxrObjectId reader_object_id = uxr_object_id(data_reader->id, UXR_DATAREADER_ID);

  sprintf(reader_profile, UXR_READER_XML, reader_name, topic_type);
  datareader_req = uxr_buffer_create_datareader_xml(&g_uxr_session.session, g_uxr_session.output_stream_id, reader_object_id, sub_object_id, reader_profile, UXR_REPLACE);

  data_reader->is_created = uxr_run_session_until_all_status(&g_uxr_session.session, 1000, &datareader_req, &status, 1);

  return data_reader->is_created;
}

bool xrcedds::readData(xrcedds::DataReader_t* data_reader)
{
  if(data_reader == NULL || data_reader->is_created == false)
  {
    return false;
  }

  uxrDeliveryControl delivery_control;

  delivery_control.max_bytes_per_second = UXR_MAX_BYTES_PER_SECOND_UNLIMITED;
  delivery_control.max_elapsed_time = UXR_MAX_ELAPSED_TIME_UNLIMITED;
  delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
  delivery_control.min_pace_period = 0;

  uxrObjectId reader_object_id = uxr_object_id(data_reader->id, UXR_DATAREADER_ID);
  data_reader->request_id      = uxr_buffer_request_data(&g_uxr_session.session, g_uxr_session.output_stream_id, reader_object_id, g_uxr_session.input_stream_id, &delivery_control);

  return true;
}

bool xrcedds::writeData(xrcedds::DataWriter_t* data_writer, void* buffer, uint32_t topic_size)
{
  if(data_writer == NULL || buffer == NULL || data_writer->is_created == false)
  {
    return false;
  }

  ucdrBuffer *mb = (ucdrBuffer*) buffer;

  uxrObjectId writer_object_id = uxr_object_id(data_writer->id, UXR_DATAWRITER_ID);
  uxr_prepare_output_stream(&g_uxr_session.session, g_uxr_session.output_stream_id,
      writer_object_id, mb, topic_size);

  return true;
}

bool xrcedds::runCommunication(uint32_t timeout_ms)
{
  uint16_t request_list[20];
  uint8_t status_list[20];
  bool is_connected = uxr_run_session_until_all_status(&g_uxr_session.session, timeout_ms, request_list, status_list, 20);

  return is_connected;
}

void xrcedds::deleteEntity(DataReader_t* data_reader)
{
  if(data_reader == NULL)
    return;

  uxrObjectId obj_id = {data_reader->id, UXR_DATAREADER_ID};
  uxr_buffer_delete_entity(&g_uxr_session.session, g_uxr_session.output_stream_id, obj_id);
}

void xrcedds::deleteEntity(DataWriter_t* data_writer)
{
  if(data_writer == NULL)
    return;

  uxrObjectId obj_id = {data_writer->id, UXR_DATAWRITER_ID};
  uxr_buffer_delete_entity(&g_uxr_session.session, g_uxr_session.output_stream_id, obj_id);
}

