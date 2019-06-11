#include <uxr/client/core/session/write_access.h>

#include "session_internal.h"
#include "session_info_internal.h"
#include "submessage_internal.h"
#include "../serialization/xrce_protocol_internal.h"

#define WRITE_DATA_PAYLOAD_SIZE 4

//==================================================================
//                             PUBLIC
//==================================================================
bool uxr_prepare_output_stream(uxrSession* session, uxrStreamId stream_id, uxrObjectId datawriter_id,
                               ucdrBuffer* ub, uint32_t topic_size)
{
    size_t payload_size = WRITE_DATA_PAYLOAD_SIZE + topic_size;
    ub->error = !uxr_prepare_stream_to_write_submessage(session, stream_id, payload_size, ub, SUBMESSAGE_ID_WRITE_DATA, FORMAT_DATA);
    if(!ub->error)
    {
        WRITE_DATA_Payload_Data payload;
        uxr_init_base_object_request(&session->info, datawriter_id, &payload.base);
        (void) uxr_serialize_WRITE_DATA_Payload_Data(ub, &payload);

        ub->last_data_size = 0; //reset alignment (as if we were created a new ucdrBuffer)
    }

    return !ub->error;
}

