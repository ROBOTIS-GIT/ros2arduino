#include "common_create_entities_internal.h"
#include "../../core/session/stream/stream_storage_internal.h"
#include "../../core/session/session_info_internal.h"
#include "../../core/session/submessage_internal.h"
#include "../../core/serialization/xrce_protocol_internal.h"

//==================================================================
//                              PUBLIC
//==================================================================
uint16_t uxr_buffer_delete_entity(uxrSession* session, uxrStreamId stream_id, uxrObjectId object_id)
{
    uint16_t request_id = UXR_INVALID_REQUEST_ID;

    DELETE_Payload payload;

    // Change this when microcdr supports size_of function.
    uint16_t payload_length = 0; //DELETE_Payload_size(&payload);
    payload_length = (uint16_t)(payload_length + 4); // delete payload (request id + object_id), no padding.

    ucdrBuffer mb;
    if(uxr_prepare_stream_to_write(&session->streams, stream_id, (uint16_t)(payload_length + SUBHEADER_SIZE), &mb))
    {
        (void) uxr_buffer_submessage_header(&mb, SUBMESSAGE_ID_DELETE, payload_length, 0);

        request_id = uxr_init_base_object_request(&session->info, object_id, &payload.base);
        (void) uxr_serialize_DELETE_Payload(&mb, &payload);
    }

    return request_id;
}

uint16_t uxr_common_create_entity(uxrSession* session, uxrStreamId stream_id,
                                  uxrObjectId object_id, uint16_t xml_ref_size, uint8_t mode,
                                  CREATE_Payload* payload)
{
    uint16_t request_id = UXR_INVALID_REQUEST_ID;

    // Change this when microcdr supports size_of function. Currently, DOMAIN_ID is not supported.
    uint16_t payload_length = 0; //CREATE_Payload_size(&payload);
    payload_length = (uint16_t)(payload_length + 4); // base
    payload_length = (uint16_t)(payload_length + 1); // objk type
    payload_length = (uint16_t)(payload_length + 1); // base3 type => xml
    payload_length = (uint16_t)(payload_length + 2); // padding
    payload_length = (uint16_t)(payload_length + 4); // xml length
    payload_length = (uint16_t)(payload_length + xml_ref_size); // xml data (note: compiler executes strlen one time this function)
    payload_length = (uint16_t)(payload_length + ((object_id.type == OBJK_PARTICIPANT && payload_length % 2 != 0) ? 1 : 0)); // necessary padding
    payload_length = (uint16_t)(payload_length + 2); //object id ref

    ucdrBuffer mb;
    if(uxr_prepare_stream_to_write(&session->streams, stream_id, (uint16_t)(payload_length + SUBHEADER_SIZE), &mb))
    {
        request_id = uxr_init_base_object_request(&session->info, object_id, &payload->base);
        (void) uxr_buffer_submessage_header(&mb, SUBMESSAGE_ID_CREATE, payload_length, mode);
        (void) uxr_serialize_CREATE_Payload(&mb, payload);
    }

    return request_id;
}

