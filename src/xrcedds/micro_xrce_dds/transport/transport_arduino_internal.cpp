#include "transport_arduino_internal.h"
#include <Arduino.h>

static Stream *rtps_serial;

bool uxr_initSerialArduino(void *serial_device)
{
  bool rv = true;

  rtps_serial = (Stream*) serial_device;

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

  tx_len = rtps_serial->write(buf, len);
  rtps_serial->flush();

  return tx_len;
}

size_t uxr_readSerialDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;

  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    rv = rtps_serial->available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    for (size_t i = 0; i < rv; i++)
    {
      buf[i] = rtps_serial->read();
    }
  }
  
  return rv;
}

#if defined(ESP_PLATFORM)
#include <WiFi.h>
#include <WiFiUdp.h>
static WiFiUDP udp;
static const char* remote_ip_addr;
static uint16_t remote_port;
static const uint16_t local_port = 8888;
#endif

bool uxr_initUdpArduino(const char* ip_address, uint16_t port)
{
  bool rv = false;

#if defined(ESP_PLATFORM)
  remote_ip_addr = ip_address;
  remote_port = port;
  udp.begin(WiFi.localIP(), local_port);
  rv = true;
#else
  (void)(ip_address); (void)(port);
#endif

  return rv;
}

bool uxr_closeUdpArduino()
{
  bool rv = false;

#if defined(ESP_PLATFORM)
  udp.stop();
  rv = true;
#endif

  return rv;
}

size_t uxr_writeUdpDataArduino(const uint8_t* buf, size_t len)
{
  size_t tx_len = 0;

#if defined(ESP_PLATFORM)
  udp.beginPacket(remote_ip_addr, remote_port);
  tx_len = udp.write(buf, len);
  udp.endPacket();
#else
  (void)(buf); (void)(len);
#endif

  return tx_len;
}

size_t uxr_readUdpDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;

#if defined(ESP_PLATFORM)
  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    udp.parsePacket();
    rv = udp.available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    udp.read(buf, len);
  }
#else
  (void)(buf); (void)(len); (void)(timeout);
#endif    
  
  return rv;
}


#if defined(ESP_PLATFORM)
#include <WiFi.h>
#include <WiFiClient.h>
static WiFiClient tcp_client;
#endif

bool uxr_connectTcpArduino(const char* ip_address, uint16_t port)
{
  bool rv = false;

#if defined(ESP_PLATFORM)
  IPAddress ip_addr;
  ip_addr.fromString(ip_address);
  tcp_client.connect(ip_addr, port);
  rv = true;
#else
  (void)(ip_address); (void)(port);
#endif

  return rv;
}

bool uxr_closeTcpArduino()
{
  bool rv = false;

#if defined(ESP_PLATFORM)
  tcp_client.stop();
  rv = true;
#endif

  return rv;
}

size_t uxr_writeTcpDataArduino(const uint8_t* buf, size_t len)
{
  size_t tx_len = 0;

#if defined(ESP_PLATFORM)
  if(tcp_client.connected())
  {
    tx_len = tcp_client.write(buf, len);
  }
#else
  (void)(buf); (void)(len);
#endif

  return tx_len;
}

size_t uxr_readTcpDataArduino(uint8_t* buf, size_t len, int timeout)
{
  size_t rv = 0;

#if defined(ESP_PLATFORM)
  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    rv = tcp_client.available();
  }

  if(rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    tcp_client.read(buf, len);
  }
#else
  (void)(buf); (void)(len); (void)(timeout);
#endif
  
  return rv;
}

bool uxr_disconnectTcpArduino()
{
  size_t rv = false;

#if defined(ESP_PLATFORM)
  tcp_client.stop();
  rv = true;
#endif

  return rv;
}
