#include "xrce_header_internal.h"

//==================================================================
//                             PUBLIC
//==================================================================
void uxr_serialize_message_header(ucdrBuffer* mb, uint8_t session_id, uint8_t stream_id, uint16_t seq_num, const uint8_t* key)
{
    (void) ucdr_serialize_uint8_t(mb, session_id);
    (void) ucdr_serialize_uint8_t(mb, stream_id);
    (void) ucdr_serialize_endian_uint16_t(mb, UCDR_LITTLE_ENDIANNESS, seq_num);
    if(UXR_SESSION_ID_WITHOUT_CLIENT_KEY > session_id)
    {
        (void) ucdr_serialize_array_uint8_t(mb, key, UXR_CLIENT_KEY_SIZE);
    }
}

void uxr_deserialize_message_header(ucdrBuffer* mb, uint8_t* session_id, uint8_t* stream_id, uint16_t* seq_num, uint8_t* key)
{
    (void) ucdr_deserialize_uint8_t(mb, session_id);
    (void) ucdr_deserialize_uint8_t(mb, stream_id);
    (void) ucdr_deserialize_endian_uint16_t(mb, UCDR_LITTLE_ENDIANNESS, seq_num);
    if(UXR_SESSION_ID_WITHOUT_CLIENT_KEY > *session_id)
    {
        (void) ucdr_deserialize_array_uint8_t(mb, key, UXR_CLIENT_KEY_SIZE);
    }
}
