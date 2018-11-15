/*
 * rtps_time.c
 *
 *  Created on: Nov 14, 2018
 *      Author: kei
 */

#include "rtps.hpp"
#include <Arduino.h>

int64_t uxr_millis(void)
{
  static uint32_t pre_msec = 0, now_msec;
  static uint64_t msec = 0;

  now_msec = millis();
  msec += (uint64_t) (now_msec - pre_msec);
  pre_msec = now_msec;

  return msec;
}
