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

bool uxr_initSerialArduino(uint32_t baudrate);
bool uxr_closeSerialArduino();
size_t uxr_writeSerialDataArduino(uint8_t* buf, size_t len);
size_t uxr_readSerialDataArduino(uint8_t* buf, size_t len, int timeout);

#ifdef __cplusplus
}
#endif

#endif //_SRC_C_PROFILE_TRANSPORT_TRANSPORT_ARDUINO_INTERNAL_H_
