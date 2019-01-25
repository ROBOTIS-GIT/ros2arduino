#include "seq_num_internal.h"
#include "input_reliable_stream_internal.h"
#include "../submessage_internal.h"
#include "../../serialization/xrce_protocol_internal.h"

#include <string.h>

// Remove when Microcdr supports size_of functions
#define ACKNACK_PAYLOAD_SIZE     4
//---

#define INTERNAL_BUFFER_OFFSET  sizeof(size_t)

static void process_heartbeat(uxrInputReliableStream* stream, uint16_t first_seq_num, uint16_t last_seq_num);
static uint16_t compute_nack_bitmap(const uxrInputReliableStream* stream);

static size_t get_input_buffer_length(uint8_t* buffer);
static void set_input_buffer_length(uint8_t* buffer, size_t length);
static uint8_t* get_input_buffer(const uxrInputReliableStream* stream, size_t history_pos);
static size_t get_input_buffer_size(const uxrInputReliableStream* stream);

//==================================================================
//                             PUBLIC
//==================================================================
void uxr_init_input_reliable_stream(uxrInputReliableStream* stream, uint8_t* buffer, size_t size, uint16_t history)
{
    // assert for history (must be 2^)
    stream->buffer = buffer;
    stream->size = size;
    stream->history = history;

    uxr_reset_input_reliable_stream(stream);
}

void uxr_reset_input_reliable_stream(uxrInputReliableStream* stream)
{
    for(size_t i = 0; i < stream->history; i++)
    {
        uint8_t* internal_buffer = get_input_buffer(stream, i);
        set_input_buffer_length(internal_buffer, 0);
    }

    stream->last_handled = UINT16_MAX;
    stream->last_announced = UINT16_MAX;
}

bool uxr_receive_reliable_message(uxrInputReliableStream* stream, uint16_t seq_num, uint8_t* buffer, size_t length)
{
    bool result = false;

    /* Check if the seq_num is valid for the stream state */
    uxrSeqNum last_history = uxr_seq_num_add(stream->last_handled, stream->history);
    if(0 > uxr_seq_num_cmp(stream->last_handled, seq_num) && 0 <= uxr_seq_num_cmp(last_history, seq_num))
    {
        /* Process the message */
        uxrSeqNum next = uxr_seq_num_add(stream->last_handled, 1);
        if(seq_num == next) //TODO (fragment): ... && is not fragment (except last fragment)
        {
            stream->last_handled = next;
            if(0 > uxr_seq_num_cmp(stream->last_announced, stream->last_handled))
            {
                stream->last_announced = stream->last_handled;
            }
            result = true;
        }
        else
        {
            /* Check if the message received is not already received */
            uint8_t* internal_buffer = get_input_buffer(stream, seq_num % stream->history);
            if(0 == get_input_buffer_length(internal_buffer))
            {
                memcpy(internal_buffer, buffer, length);
                set_input_buffer_length(internal_buffer, length);
            }
        }
    }

    if(0 > uxr_seq_num_cmp(stream->last_announced, seq_num))
    {
        stream->last_announced = seq_num;
    }

    return result;
}


bool uxr_next_input_reliable_buffer_available(uxrInputReliableStream* stream, ucdrBuffer* mb)
{
    uxrSeqNum next = uxr_seq_num_add(stream->last_handled, 1);
    uint8_t* internal_buffer = get_input_buffer(stream, next % stream->history);
    size_t length = get_input_buffer_length(internal_buffer);
    bool available_to_read = 0 != length;
    if(available_to_read)
    {
        stream->last_handled = next;
        ucdr_init_buffer(mb, internal_buffer, (uint32_t)length);
        set_input_buffer_length(internal_buffer, 0);
    }

    return available_to_read;
}

void uxr_buffer_acknack(const uxrInputReliableStream* stream, ucdrBuffer* mb) {
    uint16_t nack_bitmap = compute_nack_bitmap(stream);

    ACKNACK_Payload payload;
    payload.first_unacked_seq_num = uxr_seq_num_add(stream->last_handled, 1);
    payload.nack_bitmap[0] = (uint8_t)(nack_bitmap >> 8);
    payload.nack_bitmap[1] = (uint8_t)((nack_bitmap << 8) >> 8);

    (void) uxr_buffer_submessage_header(mb, SUBMESSAGE_ID_ACKNACK, ACKNACK_PAYLOAD_SIZE, 0);
    (void) uxr_serialize_ACKNACK_Payload(mb, &payload);
    (void) stream; (void) mb;
}

void uxr_read_heartbeat(uxrInputReliableStream* stream, ucdrBuffer* payload)
{
    HEARTBEAT_Payload heartbeat;
    uxr_deserialize_HEARTBEAT_Payload(payload, &heartbeat);

    process_heartbeat(stream, heartbeat.first_unacked_seq_nr, heartbeat.last_unacked_seq_nr);
}

bool uxr_is_input_reliable_stream_busy(uxrInputReliableStream* stream)
{
    return stream->last_announced != stream->last_handled;
}

//==================================================================
//                             PRIVATE
//==================================================================
void process_heartbeat(uxrInputReliableStream* stream, uint16_t first_seq_num, uint16_t last_seq_num)
{
    if(0 > uxr_seq_num_cmp(uxr_seq_num_add(stream->last_handled, 1), first_seq_num))
    {
        stream->last_handled = uxr_seq_num_sub(first_seq_num, 1);
    }

    if(0 > uxr_seq_num_cmp(stream->last_announced, last_seq_num))
    {
        stream->last_announced = last_seq_num;
    }
}

uint16_t compute_nack_bitmap(const uxrInputReliableStream* stream)
{
    uint16_t buffers_to_ack = uxr_seq_num_sub(stream->last_announced, stream->last_handled);
    uint16_t nack_bitmap = (buffers_to_ack > 0) ? 1 : 0;

    for(unsigned i = 0; i < (unsigned)buffers_to_ack; i++)
    {
        uxrSeqNum seq_num = uxr_seq_num_add(stream->last_handled, (uint16_t)(i + 1));
        uint8_t* internal_buffer = get_input_buffer(stream, seq_num % stream->history);
        if(0 == get_input_buffer_length(internal_buffer))
        {
            nack_bitmap = (uint16_t)(nack_bitmap | (1 << i));
        }
    }

    return nack_bitmap;
}

inline size_t get_input_buffer_length(uint8_t* buffer)
{
    return (size_t)*(buffer - INTERNAL_BUFFER_OFFSET);
}

inline void set_input_buffer_length(uint8_t* buffer, size_t length)
{
    memcpy(buffer - INTERNAL_BUFFER_OFFSET, &length, sizeof(size_t));
}

inline uint8_t* get_input_buffer(const uxrInputReliableStream* stream, size_t history_pos)
{
    return stream->buffer + history_pos * (stream->size / stream->history) + INTERNAL_BUFFER_OFFSET;
}

inline size_t get_input_buffer_size(const uxrInputReliableStream* stream)
{
    return (stream->size / stream->history) - INTERNAL_BUFFER_OFFSET;
}
