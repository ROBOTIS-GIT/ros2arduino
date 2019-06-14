/*
 * micro_xrce_dds_time.c
 *
 *  Created on: Nov 14, 2018
 *      Author: kei
 */


#include "../time.h"

int64_t uxr_nanos(void)
{
  static uint32_t pre_usec = 0, now_usec;
  static int64_t nsec = 0;

  now_usec = dds_getMicroseconds();
  nsec += (int64_t)(now_usec - pre_usec) * (int64_t)1000;
  pre_usec = now_usec;

  return nsec;
}

int64_t uxr_millis(void)
{
  return uxr_nanos() / 1000000;
}