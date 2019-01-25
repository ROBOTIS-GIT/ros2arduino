#include <uxr/client/core/session/session.h>
#include <uxr/client/util/time.h>
#include <uxr/client/core/communication/communication.h>
#include <uxr/client/config.h>

#include "submessage_internal.h"
#include "session_info_internal.h"
#include "stream/stream_storage_internal.h"
#include "stream/input_best_effort_stream_internal.h"
#include "stream/input_reliable_stream_internal.h"
#include "stream/output_best_effort_stream_internal.h"
#include "stream/output_reliable_stream_internal.h"
#include "../serialization/xrce_protocol_internal.h"
#include "../log/log_internal.h"

// Autogenerate these defines by the protocol generator tool?
#define HEARTBEAT_MAX_MSG_SIZE 16
#define ACKNACK_MAX_MSG_SIZE 16
#define CREATE_SESSION_MAX_MSG_SIZE 38
#define DELETE_SESSION_MAX_MSG_SIZE 16
//---

static bool listen_message(uxrSession* session, int poll_ms);
static bool listen_message_reliably(uxrSession* session, int poll_ms);

static bool wait_session_status(uxrSession* session, uint8_t* buffer, size_t length, size_t attempts);

static void send_message(const uxrSession* session, uint8_t* buffer, size_t length);
static bool recv_message(const uxrSession* session, uint8_t** buffer, size_t* length, int poll_ms);

static void write_submessage_heartbeat(const uxrSession* session, uxrStreamId stream);
static void write_submessage_acknack(const uxrSession* session, uxrStreamId stream);

static void read_message(uxrSession* session, ucdrBuffer* message);
static void read_stream(uxrSession* session, ucdrBuffer* message, uxrStreamId id, uxrSeqNum seq_num);
static void read_submessage_list(uxrSession* session, ucdrBuffer* submessages, uxrStreamId stream_id);
static void read_submessage(uxrSession* session, ucdrBuffer* submessage,
                            uint8_t submessage_id, uxrStreamId stream_id, uint16_t length, uint8_t flags);

static void read_submessage_fragment(uxrSession* session, ucdrBuffer* submessage, uxrStreamId stream_id, bool last_fragment);
static void read_submessage_status(uxrSession* session, ucdrBuffer* submessage);
static void read_submessage_data(uxrSession* session, ucdrBuffer* submessage, uint16_t length, uxrStreamId stream_id, uint8_t format);
static void read_submessage_heartbeat(uxrSession* session, ucdrBuffer* submessage, uxrStreamId stream_id);
static void read_submessage_acknack(uxrSession* session, ucdrBuffer* submessage, uxrStreamId stream_id);

static void process_status(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uint8_t status);

//==================================================================
//                             PUBLIC
//==================================================================

void uxr_init_session(uxrSession* session, uxrCommunication* comm, uint32_t key)
{
    session->comm = comm;

    session->request_list = NULL;
    session->status_list = NULL;
    session->request_status_list_size = 0;

    session->on_status = NULL;
    session->on_status_args = NULL;
    session->on_topic = NULL;
    session->on_topic_args = NULL;

    uxr_init_session_info(&session->info, 0x81, key);
    uxr_init_stream_storage(&session->streams);
}

void uxr_set_status_callback(uxrSession* session, uxrOnStatusFunc on_status_func, void* args)
{
    session->on_status = on_status_func;
    session->on_status_args = args;
}

void uxr_set_topic_callback(uxrSession* session, uxrOnTopicFunc on_topic_func, void* args)
{
    session->on_topic = on_topic_func;
    session->on_topic_args = args;
}

bool uxr_create_session(uxrSession* session)
{
    uxr_reset_stream_storage(&session->streams);

    uint8_t create_session_buffer[CREATE_SESSION_MAX_MSG_SIZE];
    ucdrBuffer mb;
    ucdr_init_buffer_offset(&mb, create_session_buffer, CREATE_SESSION_MAX_MSG_SIZE, uxr_session_header_offset(&session->info));

    uxr_buffer_create_session(&session->info, &mb, uxr_millis());
    uxr_stamp_create_session_header(&session->info, mb.init);

    bool received = wait_session_status(session, create_session_buffer, ucdr_buffer_length(&mb), UXR_CONFIG_MAX_SESSION_CONNECTION_ATTEMPTS);
    bool created = received && UXR_STATUS_OK == session->info.last_requested_status;
    return created;
}

bool uxr_delete_session(uxrSession* session)
{
    uint8_t delete_session_buffer[DELETE_SESSION_MAX_MSG_SIZE];
    ucdrBuffer mb;
    ucdr_init_buffer_offset(&mb, delete_session_buffer, DELETE_SESSION_MAX_MSG_SIZE, uxr_session_header_offset(&session->info));

    uxr_buffer_delete_session(&session->info, &mb);
    uxr_stamp_session_header(&session->info, 0, 0, mb.init);

    bool received = wait_session_status(session, delete_session_buffer, ucdr_buffer_length(&mb), UXR_CONFIG_MAX_SESSION_CONNECTION_ATTEMPTS);
    return received && UXR_STATUS_OK == session->info.last_requested_status;
}

uxrStreamId uxr_create_output_best_effort_stream(uxrSession* session, uint8_t* buffer, size_t size)
{
    uint8_t header_offset = uxr_session_header_offset(&session->info);
    return uxr_add_output_best_effort_buffer(&session->streams, buffer, size, header_offset);
}

uxrStreamId uxr_create_output_reliable_stream(uxrSession* session, uint8_t* buffer, size_t size, uint16_t history)
{
    uint8_t header_offset = uxr_session_header_offset(&session->info);
    return uxr_add_output_reliable_buffer(&session->streams, buffer, size, history, header_offset);
}

uxrStreamId uxr_create_input_best_effort_stream(uxrSession* session)
{
    return uxr_add_input_best_effort_buffer(&session->streams);
}

uxrStreamId uxr_create_input_reliable_stream(uxrSession* session, uint8_t* buffer, size_t size, uint16_t history)
{
    return uxr_add_input_reliable_buffer(&session->streams, buffer, size, history);
}

bool uxr_run_session_time(uxrSession* session, int timeout_ms)
{
    uxr_flash_output_streams(session);

    bool timeout = false;
    while(!timeout)
    {
         timeout = !listen_message_reliably(session, timeout_ms);
    }

    return uxr_output_streams_confirmed(&session->streams);
}

bool uxr_run_session_until_timeout(uxrSession* session, int timeout_ms)
{
    uxr_flash_output_streams(session);

    return listen_message_reliably(session, timeout_ms);
}

bool uxr_run_session_until_confirm_delivery(uxrSession* session, int timeout_ms)
{
    uxr_flash_output_streams(session);

    bool timeout = false;
    while(!uxr_output_streams_confirmed(&session->streams) && !timeout)
    {
        timeout = !listen_message_reliably(session, timeout_ms);
    }

    return uxr_output_streams_confirmed(&session->streams);
}

bool uxr_run_session_until_all_status(uxrSession* session, int timeout_ms, const uint16_t* request_list, uint8_t* status_list, size_t list_size)
{
    uxr_flash_output_streams(session);

    for(unsigned i = 0; i < list_size; ++i)
    {
        status_list[i] = UXR_STATUS_NONE;
    }

    session->request_list = request_list;
    session->status_list = status_list;
    session->request_status_list_size = list_size;

    bool timeout = false;
    bool status_confirmed = false;
    while(!timeout && !status_confirmed)
    {
        timeout = !listen_message_reliably(session, timeout_ms);
        status_confirmed = true;
        for(unsigned i = 0; i < list_size && status_confirmed; ++i)
        {
            status_confirmed = status_list[i] != UXR_STATUS_NONE
                            || request_list[i] == UXR_INVALID_REQUEST_ID; //CHECK: better give an error? an assert?
        }
    }

    session->request_status_list_size = 0;

    bool status_ok = true;
    for(unsigned i = 0; i < list_size && status_ok; ++i)
    {
        status_ok = status_list[i] == UXR_STATUS_OK || status_list[i] == UXR_STATUS_OK_MATCHED;
    }

    return status_ok;
}

bool uxr_run_session_until_one_status(uxrSession* session, int timeout_ms, const uint16_t* request_list, uint8_t* status_list, size_t list_size)
{
    uxr_flash_output_streams(session);

    for(unsigned i = 0; i < list_size; ++i)
    {
        status_list[i] = UXR_STATUS_NONE;
    }

    session->request_list = request_list;
    session->status_list = status_list;
    session->request_status_list_size = list_size;

    bool timeout = false;
    bool status_confirmed = false;
    while(!timeout && !status_confirmed)
    {
        timeout = !listen_message_reliably(session, timeout_ms);
        for(unsigned i = 0; i < list_size && !status_confirmed; ++i)
        {
            status_confirmed = status_list[i] != UXR_STATUS_NONE
                            || request_list[i] == UXR_INVALID_REQUEST_ID; //CHECK: better give an error? an assert?
        }
    }

    session->request_status_list_size = 0;

    return status_confirmed;
}

void uxr_flash_output_streams(uxrSession* session)
{
    for(uint8_t i = 0; i < session->streams.output_best_effort_size; ++i)
    {
        uxrOutputBestEffortStream* stream = &session->streams.output_best_effort[i];
        uxrStreamId id = uxr_stream_id(i, UXR_BEST_EFFORT_STREAM, UXR_OUTPUT_STREAM);

        uint8_t* buffer; size_t length; uxrSeqNum seq_num;
        if(uxr_prepare_best_effort_buffer_to_send(stream, &buffer, &length, &seq_num))
        {
            uxr_stamp_session_header(&session->info, id.raw, seq_num, buffer);
            send_message(session, buffer, length);
        }
    }

    for(uint8_t i = 0; i < session->streams.output_reliable_size; ++i)
    {
        uxrOutputReliableStream* stream = &session->streams.output_reliable[i];
        uxrStreamId id = uxr_stream_id(i, UXR_RELIABLE_STREAM, UXR_OUTPUT_STREAM);

        uint8_t* buffer; size_t length; uxrSeqNum seq_num;
        while(uxr_prepare_next_reliable_buffer_to_send(stream, &buffer, &length, &seq_num))
        {
            uxr_stamp_session_header(&session->info, id.raw, seq_num, buffer);
            send_message(session, buffer, length);
        }
    }
}

//==================================================================
//                             PRIVATE
//==================================================================
bool listen_message(uxrSession* session, int poll_ms)
{
    uint8_t* data; size_t length;
    bool must_be_read = recv_message(session, &data, &length, poll_ms);
    if(must_be_read)
    {
        ucdrBuffer mb;
        ucdr_init_buffer(&mb, data, (uint32_t)length);
        read_message(session, &mb);
    }

    return must_be_read;
}

bool listen_message_reliably(uxrSession* session, int poll_ms)
{
    bool received = false;
    int32_t poll = (poll_ms >= 0) ? poll_ms : INT32_MAX;
    do
    {
        int64_t next_heartbeat_timestamp = INT64_MAX;
        int64_t timestamp = uxr_millis();
        for(uint8_t i = 0; i < session->streams.output_reliable_size; ++i)
        {
            uxrOutputReliableStream* stream = &session->streams.output_reliable[i];
            uxrStreamId id = uxr_stream_id(i, UXR_RELIABLE_STREAM, UXR_OUTPUT_STREAM);

            if(uxr_update_output_stream_heartbeat_timestamp(stream, timestamp))
            {
                write_submessage_heartbeat(session, id);
            }

            if(stream->next_heartbeat_timestamp < next_heartbeat_timestamp)
            {
                next_heartbeat_timestamp = stream->next_heartbeat_timestamp;
            }
        }

        int32_t poll_to_next_heartbeat = (next_heartbeat_timestamp != INT64_MAX) ? (int32_t)(next_heartbeat_timestamp - timestamp) : poll;
        if(0 == poll_to_next_heartbeat)
        {
            poll_to_next_heartbeat = 1;
        }

        int poll_chosen = (poll_to_next_heartbeat < poll) ? (int)poll_to_next_heartbeat : (int)poll;
        received = listen_message(session, poll_chosen);
        if(!received)
        {
            poll -= poll_chosen;
        }
    }
    while(!received && poll > 0);

    return received;
}

bool wait_session_status(uxrSession* session, uint8_t* buffer, size_t length, size_t attempts)
{
    session->info.last_requested_status = UXR_STATUS_NONE;

    int poll_ms = UXR_CONFIG_MIN_SESSION_CONNECTION_INTERVAL;
    for(size_t i = 0; i < attempts && session->info.last_requested_status == UXR_STATUS_NONE; ++i)
    {
        send_message(session, buffer, length);
        poll_ms = listen_message(session, poll_ms) ? UXR_CONFIG_MIN_SESSION_CONNECTION_INTERVAL : poll_ms * 2;
    }

    return session->info.last_requested_status != UXR_STATUS_NONE;
}

inline void send_message(const uxrSession* session, uint8_t* buffer, size_t length)
{
    (void) session->comm->send_msg(session->comm->instance, buffer, length);
    UXR_DEBUG_PRINT_MESSAGE(UXR_SEND, buffer, length, session->info.key);
}

inline bool recv_message(const uxrSession* session, uint8_t**buffer, size_t* length, int poll_ms)
{
    bool received = session->comm->recv_msg(session->comm->instance, buffer, length, poll_ms);
    if(received)
    {
        UXR_DEBUG_PRINT_MESSAGE(UXR_RECV, *buffer, *length, session->info.key);
    }
    return received;
}

void write_submessage_heartbeat(const uxrSession* session, uxrStreamId id)
{
    uint8_t heartbeat_buffer[HEARTBEAT_MAX_MSG_SIZE];
    ucdrBuffer mb;
    ucdr_init_buffer_offset(&mb, heartbeat_buffer, HEARTBEAT_MAX_MSG_SIZE, uxr_session_header_offset(&session->info));

    const uxrOutputReliableStream* stream = &session->streams.output_reliable[id.index];

    uxr_buffer_heartbeat(stream, &mb);
    uxr_stamp_session_header(&session->info, 0, id.raw, mb.init);
    send_message(session, heartbeat_buffer, ucdr_buffer_length(&mb));
}

void write_submessage_acknack(const uxrSession* session, uxrStreamId id)
{
    uint8_t acknack_buffer[ACKNACK_MAX_MSG_SIZE];
    ucdrBuffer mb;
    ucdr_init_buffer_offset(&mb, acknack_buffer, ACKNACK_MAX_MSG_SIZE, uxr_session_header_offset(&session->info));

    const uxrInputReliableStream* stream = &session->streams.input_reliable[id.index];

    uxr_buffer_acknack(stream, &mb);
    uxr_stamp_session_header(&session->info, 0, id.raw, mb.init);
    send_message(session, acknack_buffer, ucdr_buffer_length(&mb));
}

void read_message(uxrSession* session, ucdrBuffer* mb)
{
    uint8_t stream_id_raw; uxrSeqNum seq_num;
    if(uxr_read_session_header(&session->info, mb, &stream_id_raw, &seq_num))
    {
        uxrStreamId id = uxr_stream_id_from_raw(stream_id_raw, UXR_INPUT_STREAM);
        read_stream(session, mb, id, seq_num);
    }
}

void read_stream(uxrSession* session, ucdrBuffer* mb, uxrStreamId stream_id, uxrSeqNum seq_num)
{
    switch(stream_id.type)
    {
        case UXR_NONE_STREAM:
        {
            stream_id = uxr_stream_id_from_raw((uint8_t)seq_num, UXR_INPUT_STREAM); // The real stream_id is into seq_num
            read_submessage_list(session, mb, stream_id);
            break;
        }
        case UXR_BEST_EFFORT_STREAM:
        {
            uxrInputBestEffortStream* stream = uxr_get_input_best_effort_stream(&session->streams, stream_id.index);
            if(stream && uxr_receive_best_effort_message(stream, seq_num))
            {
                read_submessage_list(session, mb, stream_id);
            }
            break;
        }
        case UXR_RELIABLE_STREAM:
        {
            uxrInputReliableStream* stream = uxr_get_input_reliable_stream(&session->streams, stream_id.index);
            if(stream && uxr_receive_reliable_message(stream, seq_num, mb->iterator, ucdr_buffer_size(mb)))
            {
                read_submessage_list(session, mb, stream_id);
                ucdrBuffer next_mb;
                while(uxr_next_input_reliable_buffer_available(stream, &next_mb))
                {
                    read_submessage_list(session, &next_mb, stream_id);
                }
                write_submessage_acknack(session, stream_id);
            }
            break;
        }
        default:
            break;
    }
}

void read_submessage_list(uxrSession* session, ucdrBuffer* submessages, uxrStreamId stream_id)
{
    uint8_t id; uint16_t length; uint8_t flags; uint8_t* payload_it = NULL;
    while(uxr_read_submessage_header(submessages, &id, &length, &flags, &payload_it))
    {
        read_submessage(session, submessages, id, stream_id, length, flags);
    }
}

void read_submessage(uxrSession* session, ucdrBuffer* submessage, uint8_t submessage_id, uxrStreamId stream_id, uint16_t length, uint8_t flags)
{
    switch(submessage_id)
    {
        case SUBMESSAGE_ID_STATUS_AGENT:
            if(stream_id.type == UXR_NONE_STREAM)
            {
                uxr_read_create_session_status(&session->info, submessage);
            }
            break;

        case SUBMESSAGE_ID_STATUS:
            if(stream_id.type == UXR_NONE_STREAM)
            {
                uxr_read_delete_session_status(&session->info, submessage);
            }
            else
            {
                read_submessage_status(session, submessage);
            }
            break;

        case SUBMESSAGE_ID_DATA:
            read_submessage_data(session, submessage, length, stream_id, flags & FORMAT_MASK);
            break;

        case SUBMESSAGE_ID_FRAGMENT:
            read_submessage_fragment(session, submessage, stream_id, 0 != (flags & FLAG_LAST_FRAGMENT));
            break;

        case SUBMESSAGE_ID_HEARTBEAT:
            read_submessage_heartbeat(session, submessage, stream_id);
            break;

        case SUBMESSAGE_ID_ACKNACK:
            read_submessage_acknack(session, submessage, stream_id);
            break;

        default:
            break;
    }
}

void read_submessage_status(uxrSession* session, ucdrBuffer* submessage)
{
    STATUS_Payload payload;
    uxr_deserialize_STATUS_Payload(submessage, &payload);


    uxrObjectId object_id; uint16_t request_id;
    uxr_parse_base_object_request(&payload.base.related_request, &object_id, &request_id);

    uint8_t status = payload.base.result.status;
    process_status(session, object_id, request_id, status);
}


extern void read_submessage_format(uxrSession* session, ucdrBuffer* data, uint16_t length, uint8_t format,
                                   uxrStreamId stream_id, uxrObjectId object_id, uint16_t request_id);

void read_submessage_data(uxrSession* session, ucdrBuffer* submessage, uint16_t length, uxrStreamId stream_id, uint8_t format)
{
#ifdef PROFILE_READ_ACCESS
    BaseObjectRequest base;
    uxr_deserialize_BaseObjectRequest(submessage, &base);
    length = (uint16_t)(length - 4); //CHANGE: by a future size_of_BaseObjectRequest

    uxrObjectId object_id; uint16_t request_id;
    uxr_parse_base_object_request(&base, &object_id, &request_id);

    process_status(session, object_id, request_id, UXR_STATUS_OK);

    if(session->on_topic != NULL)
    {
        read_submessage_format(session, submessage, length, format, stream_id, object_id, request_id);
    }
#else
    (void) session; (void) submessage; (void) length; (void) stream_id; (void) format;
#endif
}

void read_submessage_fragment(uxrSession* session, ucdrBuffer* submessage, uxrStreamId stream_id, bool last_fragment)
{
    (void) session; (void) submessage; (void) stream_id; (void) last_fragment;
    //TODO
}

void read_submessage_heartbeat(uxrSession* session, ucdrBuffer* submessage, uxrStreamId stream_id)
{
    uxrInputReliableStream* stream = uxr_get_input_reliable_stream(&session->streams, stream_id.index);
    if(stream)
    {
        uxr_read_heartbeat(stream, submessage);
        write_submessage_acknack(session, stream_id);
    }
}

void read_submessage_acknack(uxrSession* session, ucdrBuffer* submessage, uxrStreamId stream_id)
{
    uxrOutputReliableStream* stream = uxr_get_output_reliable_stream(&session->streams, stream_id.index);
    if(stream)
    {
        uxr_read_acknack(stream, submessage);

        uint8_t* buffer; size_t length;
        uxrSeqNum seq_num_it = uxr_begin_output_nack_buffer_it(stream);
        if(uxr_next_reliable_nack_buffer_to_send(stream, &buffer, &length, &seq_num_it))
        {
            send_message(session, buffer, length);
        }
    }
}

void process_status(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uint8_t status)
{
    if(session->on_status != NULL)
    {
        session->on_status(session, object_id, request_id, status, session->on_status_args);
    }

    for(unsigned i = 0; i < session->request_status_list_size; ++i)
    {
        if(request_id == session->request_list[i])
        {
            session->status_list[i] = status;
            break;
        }
    }
}
