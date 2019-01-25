#include <uxr/client/profile/session/write_access.h>

#include "../../core/session/stream/stream_storage_internal.h"
#include "../../core/session/session_info_internal.h"
#include "../../core/session/submessage_internal.h"
#include "../../core/serialization/xrce_protocol_internal.h"

#define WRITE_DATA_PAYLOAD_SIZE 8

//==================================================================
//                             PUBLIC
//==================================================================
bool uxr_prepare_output_stream(uxrSession* session, uxrStreamId stream_id, uxrObjectId datawriter_id,
                              struct ucdrBuffer* mb_topic, uint32_t topic_size)
{
    ucdrBuffer mb;
    size_t submessage_size = SUBHEADER_SIZE + WRITE_DATA_PAYLOAD_SIZE + topic_size;
    if(uxr_prepare_stream_to_write(&session->streams, stream_id, submessage_size, &mb))
    {
        uint16_t payload_size = (uint16_t)(WRITE_DATA_PAYLOAD_SIZE + topic_size);
        (void) uxr_buffer_submessage_header(&mb, SUBMESSAGE_ID_WRITE_DATA, payload_size, FORMAT_DATA);

        WRITE_DATA_Payload_Data payload;
        uxr_init_base_object_request(&session->info, datawriter_id, &payload.base);
        (void) uxr_serialize_WRITE_DATA_Payload_Data(&mb, &payload);
        (void) ucdr_serialize_uint32_t(&mb, topic_size); //REMOVE: when topics have not a previous size in the agent.

        ucdr_init_buffer(mb_topic, mb.iterator, topic_size);
    }
    else
    {
        mb_topic->error = true;
    }

    return !mb_topic->error;
}

