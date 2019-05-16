#include "transport_arduino_internal.h"
#include <Arduino.h>
#include <Udp.h>
#include <Client.h>

static Stream *p_serial;

bool uxr_initSerialArduino(void *serial_instance)
{
  bool rv = true;

  p_serial = (Stream*) serial_instance;

  return rv;
}

bool uxr_closeSerialArduino()
{
  bool rv = true;

  return rv;
}

size_t uxr_writeSerialDataArduino(uint8_t* buf, size_t len)
{
  size_t tx_len = 0;

  tx_len = p_serial->write(buf, len);
  p_serial->flush();

  return tx_len;
}

size_t uxr_readSerialDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;

  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    rv = p_serial->available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    for (size_t i = 0; i < rv; i++)
    {
      buf[i] = p_serial->read();
    }
  }
  
  return rv;
}


static UDP* p_udp;
static const char* remote_ip_addr;
static uint16_t remote_port;
static const uint16_t local_port = 8888;

#if defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <WiFiUdp.h>
#endif

bool uxr_initUdpArduino(void* udp_instance, const char* ip_address, uint16_t port)
{
  bool rv = false;

  if(udp_instance == nullptr)
  {
    return false;
  }

  p_udp = (UDP*)udp_instance;
  remote_ip_addr = ip_address;
  remote_port = port;
#if defined(ESP_PLATFORM)
  WiFiUDP *p_wifi_udp = (WiFiUDP*)udp_instance;
  p_wifi_udp->begin(WiFi.localIP(), local_port);
#else
  p_udp->begin(local_port);
#endif
  rv = true;
  
  return rv;
}

bool uxr_closeUdpArduino()
{
  p_udp->stop();

  return true;
}

size_t uxr_writeUdpDataArduino(const uint8_t* buf, size_t len)
{
  size_t tx_len = 0;

  p_udp->beginPacket(remote_ip_addr, remote_port);
  tx_len = p_udp->write(buf, len);
  p_udp->endPacket();

  return tx_len;
}

size_t uxr_readUdpDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;
  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    p_udp->parsePacket();
    rv = p_udp->available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    p_udp->read(buf, len);
  }
  
  return rv;
}



static Client* p_client;

bool uxr_connectTcpArduino(void* client_instance, const char* ip_address, uint16_t port)
{
  bool rv = false;

  if(client_instance == nullptr)
  {
    return false;
  }

  p_client = (Client*)client_instance;
  rv = p_client->connect(ip_address, port);

  return rv;
}

bool uxr_closeTcpArduino()
{
  p_client->stop();

  return true;
}

size_t uxr_writeTcpDataArduino(const uint8_t* buf, size_t len)
{
  size_t tx_len = 0;

  if(p_client->connected())
  {
    tx_len = p_client->write(buf, len);
  }

  return tx_len;
}

size_t uxr_readTcpDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;
  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    rv = p_client->available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    p_client->read(buf, len);
  }
  
  return rv;
}

bool uxr_disconnectTcpArduino()
{
  p_client->stop();

  return true;
}
