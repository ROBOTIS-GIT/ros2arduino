/*
 * transport_arduino_internal.h
 *
 *  Created on: Nov 14, 2018
 *      Author: Kei
 */


#ifndef _SRC_C_PROFILE_TRANSPORT_TRANSPORT_ARDUINO_INTERNAL_H_
#define _SRC_C_PROFILE_TRANSPORT_TRANSPORT_ARDUINO_INTERNAL_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

bool uxr_initSerialArduino(void *serial_instance);
bool uxr_closeSerialArduino();
size_t uxr_writeSerialDataArduino(uint8_t* buf, size_t len);
size_t uxr_readSerialDataArduino(uint8_t* buf, size_t len, int timeout);

bool uxr_initUdpArduino(void* udp_instance, const char* ip_address, uint16_t port);
bool uxr_closeUdpArduino();
size_t uxr_writeUdpDataArduino(const uint8_t* buf, size_t len);
size_t uxr_readUdpDataArduino(uint8_t* buf, size_t len, int timeout);

bool uxr_connectTcpArduino(void* client_instance, const char* ip_address, uint16_t port);
bool uxr_closeTcpArduino();
size_t uxr_writeTcpDataArduino(const uint8_t* buf, size_t len);
size_t uxr_readTcpDataArduino(uint8_t* buf, size_t len, int timeout);
bool uxr_disconnectTcpArduino();

#ifdef __cplusplus
}
#endif

#endif //_SRC_C_PROFILE_TRANSPORT_TRANSPORT_ARDUINO_INTERNAL_H_
