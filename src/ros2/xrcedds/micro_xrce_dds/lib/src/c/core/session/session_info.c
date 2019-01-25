#include <uxr/client/core/session/session_info.h>
#include <uxr/client/core/session/object_id.h>

#include "submessage_internal.h"
#include "../serialization/xrce_protocol_internal.h"
#include "../serialization/xrce_header_internal.h"

#include <string.h>

// Remove when Microcdr supports size_of functions
#define CREATE_CLIENT_PAYLOAD_SIZE 26
#define DELETE_CLIENT_PAYLOAD_SIZE 4

#define VENDOR_ID_EPROSIMA (XrceVendorId){{0x01, 0x0F}}

#define INITIAL_REQUEST_ID 10

static uint16_t generate_request_id(uxrSessionInfo* info);

static void process_create_session_status(uxrSessionInfo* info, uint8_t status, AGENT_Representation* agent);
static void process_delete_session_status(uxrSessionInfo* info, uint8_t status);

//==================================================================
//                             PUBLIC
//==================================================================

void uxr_init_session_info(uxrSessionInfo* info, uint8_t id, uint32_t key)
{
    info->id = id;
    info->key[0] = (uint8_t)(key >> 24);
    info->key[1] = (uint8_t)((key << 8) >> 24);
    info->key[2] = (uint8_t)((key << 16) >> 24);
    info->key[3] = (uint8_t)((key << 24) >> 24);
    info->last_request_id = INITIAL_REQUEST_ID;
    info->last_requested_status = UXR_STATUS_NONE;
}

void uxr_buffer_create_session(const uxrSessionInfo* info, ucdrBuffer* mb, int64_t nanoseconds)
{
    CREATE_CLIENT_Payload payload;
    payload.base.request_id = (RequestId){{0x00, UXR_REQUEST_LOGIN}};
    payload.base.object_id = OBJECTID_CLIENT;
    payload.client_representation.xrce_cookie = XRCE_COOKIE;
    payload.client_representation.xrce_version = XRCE_VERSION;
    payload.client_representation.xrce_vendor_id = VENDOR_ID_EPROSIMA;
    payload.client_representation.client_timestamp.seconds = (int32_t)(nanoseconds / 1000000000);
    payload.client_representation.client_timestamp.nanoseconds = (uint32_t)nanoseconds % 1000000000;
    payload.client_representation.client_key.data[0] = info->key[0];
    payload.client_representation.client_key.data[1] = info->key[1];
    payload.client_representation.client_key.data[2] = info->key[2];
    payload.client_representation.client_key.data[3] = info->key[3];
    payload.client_representation.session_id = info->id;
    payload.client_representation.optional_properties = false;

    (void) uxr_buffer_submessage_header(mb, SUBMESSAGE_ID_CREATE_CLIENT, CREATE_CLIENT_PAYLOAD_SIZE, 0);
    (void) uxr_serialize_CREATE_CLIENT_Payload(mb, &payload);
}

void uxr_buffer_delete_session(const uxrSessionInfo* info, ucdrBuffer* mb)
{
    (void) info;
    DELETE_Payload payload;
    payload.base.request_id = (RequestId){{0x00, UXR_REQUEST_LOGOUT}};
    payload.base.object_id = OBJECTID_CLIENT;

    (void) uxr_buffer_submessage_header(mb, SUBMESSAGE_ID_DELETE, DELETE_CLIENT_PAYLOAD_SIZE, 0);
    (void) uxr_serialize_DELETE_Payload(mb, &payload);
}

void uxr_read_create_session_status(uxrSessionInfo* info, ucdrBuffer* mb)
{
    STATUS_AGENT_Payload payload;
    if(uxr_deserialize_STATUS_AGENT_Payload(mb, &payload))
    {
        process_create_session_status(info, payload.base.result.status, &payload.agent_info);
    }
}

void uxr_read_delete_session_status(uxrSessionInfo* info, ucdrBuffer* mb)
{
    STATUS_Payload payload;
    if(uxr_deserialize_STATUS_Payload(mb, &payload))
    {
        process_delete_session_status(info, payload.base.result.status);
    }
}

void uxr_stamp_create_session_header(const uxrSessionInfo* info, uint8_t* buffer)
{
    ucdrBuffer mb;
    ucdr_init_buffer(&mb, buffer, UXR_MAX_HEADER_SIZE);

    uxr_serialize_message_header(&mb, info->id & UXR_SESSION_ID_WITHOUT_CLIENT_KEY, 0, 0, info->key);
}

void uxr_stamp_session_header(const uxrSessionInfo* info, uint8_t stream_id_raw, uxrSeqNum seq_num, uint8_t* buffer)
{
    ucdrBuffer mb;
    ucdr_init_buffer(&mb, buffer, UXR_MAX_HEADER_SIZE);

    uxr_serialize_message_header(&mb, info->id, stream_id_raw, seq_num, info->key);
}

bool uxr_read_session_header(const uxrSessionInfo* info, ucdrBuffer* mb, uint8_t* stream_id_raw, uxrSeqNum* seq_num)
{
    bool must_be_read = ucdr_buffer_remaining(mb) > UXR_MAX_HEADER_SIZE;
    if(must_be_read)
    {
        uint8_t session_id; uint8_t key[UXR_CLIENT_KEY_SIZE];
        uxr_deserialize_message_header(mb, &session_id, stream_id_raw, seq_num, key);

        must_be_read = session_id == info->id;
        if(must_be_read)
        {
            if (UXR_SESSION_ID_WITHOUT_CLIENT_KEY > info->id)
            {
                must_be_read = (0 == memcmp(key, info->key, UXR_CLIENT_KEY_SIZE));
            }
        }
    }

    return must_be_read;
}

uint8_t uxr_session_header_offset(const uxrSessionInfo* info)
{
    return (UXR_SESSION_ID_WITHOUT_CLIENT_KEY > info->id) ? UXR_MAX_HEADER_SIZE : UXR_MIN_HEADER_SIZE;
}

uint16_t uxr_init_base_object_request(uxrSessionInfo* info, uxrObjectId object_id, BaseObjectRequest* base)
{
    uint16_t request_id = generate_request_id(info);

    base->request_id.data[0] = (uint8_t) (request_id >> 8);
    base->request_id.data[1] = (uint8_t) request_id;
    uxr_object_id_to_raw(object_id, base->object_id.data);

    return request_id;
}

void uxr_parse_base_object_request(const BaseObjectRequest* base, uxrObjectId* object_id, uint16_t* request_id)
{
    *object_id = uxr_object_id_from_raw(base->object_id.data);
    *request_id = (uint16_t)((((uint16_t) base->request_id.data[0]) << 8)
                            + base->request_id.data[1]);
}

//==================================================================
//                            PRIVATE
//==================================================================
inline uint16_t generate_request_id(uxrSessionInfo* session)
{
    uint16_t last_request_id = (UINT16_MAX == session->last_request_id)
        ? INITIAL_REQUEST_ID
        : session->last_request_id;

    session->last_request_id = (uint16_t)(last_request_id + 1);
    return last_request_id;
}

inline void process_create_session_status(uxrSessionInfo* info, uint8_t status, AGENT_Representation* agent)
{
    (void) agent;
    info->last_requested_status = status;
}

inline void process_delete_session_status(uxrSessionInfo* info, uint8_t status)
{
    info->last_requested_status = status;
}
