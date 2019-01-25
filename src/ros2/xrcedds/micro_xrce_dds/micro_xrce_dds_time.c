/*
 * micro_xrce_dds_time.c
 *
 *  Created on: Nov 14, 2018
 *      Author: kei
 */


#include "../time.h"

int64_t uxr_millis(void)
{
  static uint32_t pre_msec = 0, now_msec;
  static uint64_t msec = 0;

  now_msec = dds_getMilliseconds();
  msec += (uint64_t) (now_msec - pre_msec);
  pre_msec = now_msec;

  return msec;
}
