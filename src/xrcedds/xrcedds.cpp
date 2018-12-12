/*
 *  xrcedds.cpp
 *
 *  Created on: Nov 14, 2018
 *      Author: Kei
 */

#include <stdlib.h>
#include <string.h>

#include "xrcedds.hpp"


//-- Internal Variables
//
#define STREAM_HISTORY  2

#if defined(UXR_CONFIG_SERIAL_TRANSPORT_MTU)
#define BUFFER_SIZE     UXR_CONFIG_SERIAL_TRANSPORT_MTU * STREAM_HISTORY
#elif defined(UXR_CONFIG_UDP_TRANSPORT_MTU)
#define BUFFER_SIZE     UXR_CONFIG_UDP_TRANSPORT_MTU * STREAM_HISTORY
#elif defined(UXR_CONFIG_TCP_TRANSPORT_MTU)
#define BUFFER_SIZE     UXR_CONFIG_TCP_TRANSPORT_MTU * STREAM_HISTORY
#endif

bool             g_is_xrcedds_init_done = false;
uint32_t         g_session_key = 0xAABBCCDD;
uxrSession       g_xrcedds_session;
uxrCommunication *p_xrcedds_comm = NULL;

uint8_t      output_reliable_stream_buffer[BUFFER_SIZE];
uint8_t      input_reliable_stream_buffer[BUFFER_SIZE];

#if defined(PROFILE_SERIAL_TRANSPORT)
static uxrSerialTransport transport;
static uxrSerialPlatform platform;
#endif

#if defined(PROFILE_UDP_TRANSPORT)
static uxrUDPTransport transport;
static uxrUDPPlatform platform;
#endif

#if defined(PROFILE_TCP_TRANSPORT)
static uxrTCPTransport transport;
static uxrTCPPlatform platform;
#endif
//-- Internal Functions
//


void xrcedds::init(uint8_t xrcedds_product)
{
  (void)(xrcedds_product);
}

bool xrcedds::initTransportAndSession(Transport_t* transport_info, uxrOnTopicFunc callback_func, void* callback_args)
{
  g_is_xrcedds_init_done = false;

  switch(transport_info->type)
  {
#if defined(PROFILE_SERIAL_TRANSPORT)
    case 0:
      if(uxr_init_serial_transport(&transport, &platform, 0, 0, 0) == true)
      {
        p_xrcedds_comm = &transport.comm;
      }
      break;
#endif

#if defined(PROFILE_UDP_TRANSPORT)
    case 1:
      if(uxr_init_udp_transport(&transport, &platform,
          transport_info->server_ip, transport_info->server_port) == true)
      {
        p_xrcedds_comm = &transport.comm;
      }
      break;
#endif

#if defined(PROFILE_TCP_TRANSPORT)
    case 2:
      if(uxr_init_tcp_transport(&transport, &platform,
          transport_info->server_ip, transport_info->server_port) == true)
      {
        p_xrcedds_comm = &transport.comm;
      }
      break;
#endif

    default:
      break;
  }

  if(p_xrcedds_comm != NULL)
  {
    uxr_init_session(&g_xrcedds_session, p_xrcedds_comm, g_session_key);
    uxr_set_topic_callback(&g_xrcedds_session, callback_func, callback_args);

    g_is_xrcedds_init_done = uxr_create_session(&g_xrcedds_session);
  }

  return g_is_xrcedds_init_done;
}

void xrcedds::deleteTransportAndSession(void)
{
  uxr_delete_session(&g_xrcedds_session);

#if defined(PROFILE_SERIAL_TRANSPORT)
  uxr_close_serial_transport(&transport);
#endif

#if defined(PROFILE_UDP_TRANSPORT)
  uxr_close_udp_transport(&transport);
#endif

#if defined(PROFILE_TCP_TRANSPORT)
  uxr_close_tcp_transport(&transport);
#endif
}

bool xrcedds::createParticipant(xrcedds::Participant_t* participant, const char* participant_profile)
{
  if(g_is_xrcedds_init_done == false)
  {
    return false;
  }

  static uint8_t object_id = 0x01;

  participant->output_stream_id = uxr_create_output_reliable_stream(&g_xrcedds_session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
  participant->input_stream_id  = uxr_create_input_reliable_stream(&g_xrcedds_session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
  //participant->output_stream_id = uxr_create_output_best_effort_stream(&g_xrcedds_session, output_reliable_stream_buffer, BUFFER_SIZE);
  //participant->input_stream_id  = uxr_create_input_best_effort_stream(&g_xrcedds_session);
  participant->session = &g_xrcedds_session;

  participant->id = uxr_object_id(object_id++, UXR_PARTICIPANT_ID);
  uint16_t participant_req = uxr_buffer_create_participant_xml(&g_xrcedds_session, participant->output_stream_id, participant->id, 0, participant_profile, UXR_REPLACE);
  //uint16_t participant_req = uxr_buffer_create_participant_ref(&g_xrcedds_session, participant->output_stream_id, participant->id, 0, participant_profile, UXR_REPLACE);

  uint8_t status;
  participant->is_init = uxr_run_session_until_all_status(participant->session, 1000, &participant_req, &status, 1);

  return participant->is_init;
}


bool xrcedds::registerTopic(xrcedds::Participant_t* participant, char* topic_profile, uint8_t topic_id)
{
  if(participant->is_init == false)
  {
    return false;
  }

  uint8_t status;
  bool ret = false;

  uxrObjectId object_id = uxr_object_id(topic_id, UXR_TOPIC_ID);

  uint16_t topic_req = uxr_buffer_create_topic_xml(participant->session, participant->output_stream_id, object_id, participant->id, topic_profile, UXR_REUSE);
  ret = uxr_run_session_until_all_status(participant->session, 1000, &topic_req, &status, 1);
  if(ret == false && status == UXR_STATUS_ERR_ALREADY_EXISTS)
  {
    ret = true;
  }

  return ret;
}


bool xrcedds::createPublisher(xrcedds::Participant_t* participant, xrcedds::Publisher_t* publisher, char* publisher_profile, char* writer_profile)
{
  if(participant->is_init == false)
  {
    return false;
  }

  bool ret                 = false;
  static uint8_t pub_id    = 0x01;
  publisher->is_init       = false;
  publisher->id            = uxr_object_id(pub_id++, UXR_PUBLISHER_ID);
  publisher->participant   = participant;

  uint8_t status;

  uint16_t publisher_req = uxr_buffer_create_publisher_xml(participant->session, participant->output_stream_id, publisher->id, participant->id, publisher_profile, UXR_REPLACE);
  ret = uxr_run_session_until_all_status(participant->session, 1000, &publisher_req, &status, 1);

  if (ret == true)
  {
    // Create Writer
    static uint8_t writer_id = 0x01;
    publisher->writer_id = uxr_object_id(writer_id++, UXR_DATAWRITER_ID);
    uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(participant->session, participant->output_stream_id, publisher->writer_id, publisher->id, writer_profile, UXR_REPLACE);

    publisher->is_init = uxr_run_session_until_all_status(participant->session, 1000, &datawriter_req, &status, 1);
  }

  return publisher->is_init;
}


bool xrcedds::createSubscriber(xrcedds::Participant_t* participant, xrcedds::Subscriber_t* subscriber, char* subscriber_profile, char* reader_profile)
{
  if(participant->is_init == false)
  {
    return false;
  }

  bool ret                  = false;
  static uint8_t sub_id     = 0x01;
  subscriber->is_init       = false;
  subscriber->id            = uxr_object_id(sub_id++, UXR_SUBSCRIBER_ID);
  subscriber->participant   = participant;

  uint8_t status;
  uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(participant->session, participant->output_stream_id, subscriber->id, participant->id, subscriber_profile, UXR_REPLACE);
  ret = uxr_run_session_until_all_status(participant->session, 1000, &subscriber_req, &status, 1);

  if (ret == true)
  {
    // Create Reader
    static uint8_t reader_id = 0x01;
    subscriber->reader_id = uxr_object_id(reader_id++, UXR_DATAREADER_ID);
    uint16_t datareader_req = uxr_buffer_create_datareader_xml(participant->session, participant->output_stream_id, subscriber->reader_id, subscriber->id, reader_profile, UXR_REPLACE);

    subscriber->is_init = uxr_run_session_until_all_status(participant->session, 1000, &datareader_req, &status, 1);
  }

  return subscriber->is_init;
}


void xrcedds::subscribe(xrcedds::Subscriber_t* subscriber)
{
  if(subscriber->is_init == false)
  {
    return;
  }

  subscriber->delivery_control.max_bytes_per_second = UXR_MAX_BYTES_PER_SECOND_UNLIMITED;
  subscriber->delivery_control.max_elapsed_time = UXR_MAX_ELAPSED_TIME_UNLIMITED;
  subscriber->delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
  subscriber->delivery_control.min_pace_period = 0;

  subscriber->read_req = uxr_buffer_request_data(subscriber->participant->session, subscriber->participant->output_stream_id, subscriber->reader_id, subscriber->participant->input_stream_id, &subscriber->delivery_control);
}

void xrcedds::publish(xrcedds::Publisher_t* publisher, void* buffer, uint32_t topic_size)
{
  ucdrBuffer *mb = (ucdrBuffer*) buffer;

  uxr_prepare_output_stream(&g_xrcedds_session, publisher->participant->output_stream_id,
      publisher->writer_id, mb, topic_size);
}

bool xrcedds::runCommunication(uint32_t timeout_ms)
{
  uint16_t request_list[20];
  uint8_t status_list[20];
  //bool is_connected = uxr_run_session_until_confirm_delivery(&g_xrcedds_session, timeout_ms);
  bool is_connected = uxr_run_session_until_all_status(&g_xrcedds_session, timeout_ms, request_list, status_list, 20);

  return is_connected;
}


