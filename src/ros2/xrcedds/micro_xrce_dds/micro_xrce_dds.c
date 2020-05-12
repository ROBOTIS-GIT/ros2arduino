/*
 * micro_xrce_dds.c
 *
 *  Created on: Dec 14, 2018
 *      Author: kei
 */

#include "micro_xrce_dds.h"


static uxr_onTopicUserCallback userCallback;

void uxr_onTopicCallback(struct uxrSession* session, uxrObjectId object_id, 
  uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer* ub, 
  uint16_t length, void* args)
{
  (void)(session); (void)(request_id); (void)(stream_id); (void)(length);

  if(userCallback != NULL)
  {
    userCallback(object_id.id, (void*)ub, args);
  }
}

void uxr_setOnTopicUserCallback(uxr_onTopicUserCallback callback_func)
{
  userCallback = callback_func;
}