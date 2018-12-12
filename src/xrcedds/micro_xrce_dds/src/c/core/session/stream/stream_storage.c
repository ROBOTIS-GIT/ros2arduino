
#include "stream_storage_internal.h"
#include "input_best_effort_stream_internal.h"
#include "input_reliable_stream_internal.h"
#include "output_best_effort_stream_internal.h"
#include "output_reliable_stream_internal.h"

//==================================================================
//                             PUBLIC
//==================================================================
void uxr_init_stream_storage(uxrStreamStorage* storage)
{
    storage->output_best_effort_size = 0;
    storage->output_reliable_size = 0;
    storage->input_best_effort_size = 0;
    storage->input_reliable_size = 0;
}

void uxr_reset_stream_storage(uxrStreamStorage* storage)
{
    for(unsigned i = 0; i < storage->output_best_effort_size; ++i)
    {
        uxr_reset_output_best_effort_stream(&storage->output_best_effort[i]);
    }

    for(unsigned i = 0; i < storage->input_best_effort_size; ++i)
    {
        uxr_reset_input_best_effort_stream(&storage->input_best_effort[i]);
    }

    for(unsigned i = 0; i < storage->output_reliable_size; ++i)
    {
        uxr_reset_output_reliable_stream(&storage->output_reliable[i]);
    }

    for(unsigned i = 0; i < storage->input_reliable_size; ++i)
    {
        uxr_reset_input_reliable_stream(&storage->input_reliable[i]);
    }
}

uxrStreamId uxr_add_output_best_effort_buffer(uxrStreamStorage* storage, uint8_t* buffer, size_t size, uint8_t header_offset)
{
    uint8_t index = storage->output_best_effort_size++;
    // assert for index
    uxrOutputBestEffortStream* stream = &storage->output_best_effort[index];
    uxr_init_output_best_effort_stream(stream, buffer, size, header_offset);
    return uxr_stream_id(index, UXR_BEST_EFFORT_STREAM, UXR_OUTPUT_STREAM);
}

uxrStreamId uxr_add_output_reliable_buffer(uxrStreamStorage* storage, uint8_t* buffer, size_t size, uint16_t history, uint8_t header_offset)
{
    uint8_t index = storage->output_reliable_size++;
    // assert for index
    uxrOutputReliableStream* stream = &storage->output_reliable[index];
    uxr_init_output_reliable_stream(stream, buffer, size, history, header_offset);
    return uxr_stream_id(index, UXR_RELIABLE_STREAM, UXR_OUTPUT_STREAM);
}

uxrStreamId uxr_add_input_best_effort_buffer(uxrStreamStorage* storage)
{
    uint8_t index = storage->input_best_effort_size++;
    // assert for index
    uxrInputBestEffortStream* stream = &storage->input_best_effort[index];
    uxr_init_input_best_effort_stream(stream);
    return uxr_stream_id(index, UXR_BEST_EFFORT_STREAM, UXR_INPUT_STREAM);
}

uxrStreamId uxr_add_input_reliable_buffer(uxrStreamStorage* storage, uint8_t* buffer, size_t size, uint16_t history)
{
    uint8_t index = storage->input_reliable_size++;
    // assert for index
    uxrInputReliableStream* stream = &storage->input_reliable[index];
    uxr_init_input_reliable_stream(stream, buffer, size, history);
    return uxr_stream_id(index, UXR_RELIABLE_STREAM, UXR_INPUT_STREAM);
}

uxrOutputBestEffortStream* uxr_get_output_best_effort_stream(uxrStreamStorage* storage, uint8_t index)
{
    if(index < storage->output_best_effort_size)
    {
        return &storage->output_best_effort[index];
    }
    return NULL;
}

uxrOutputReliableStream* uxr_get_output_reliable_stream(uxrStreamStorage* storage, uint8_t index)
{
    if(index < storage->output_reliable_size)
    {
        return &storage->output_reliable[index];
    }
    return NULL;
}

uxrInputBestEffortStream* uxr_get_input_best_effort_stream(uxrStreamStorage* storage, uint8_t index)
{
    if(index < storage->input_best_effort_size)
    {
        return &storage->input_best_effort[index];
    }
    return NULL;
}

uxrInputReliableStream* uxr_get_input_reliable_stream(uxrStreamStorage* storage, uint8_t index)
{
    if(index < storage->input_reliable_size)
    {
        return &storage->input_reliable[index];
    }
    return NULL;
}

bool uxr_prepare_stream_to_write(uxrStreamStorage* storage, uxrStreamId stream_id, size_t size, struct ucdrBuffer* mb)
{
    bool available = false;
    switch(stream_id.type)
    {
        case UXR_BEST_EFFORT_STREAM:
        {
            uxrOutputBestEffortStream* stream = uxr_get_output_best_effort_stream(storage, stream_id.index);
            available = stream && uxr_prepare_best_effort_buffer_to_write(stream, size, mb);
            break;
        }
        case UXR_RELIABLE_STREAM:
        {
            uxrOutputReliableStream* stream = uxr_get_output_reliable_stream(storage, stream_id.index);
            available = stream && uxr_prepare_reliable_buffer_to_write(stream, size, mb);
            break;
        }
        default:
            break;
    }

    return available;
}

bool uxr_output_streams_confirmed(const uxrStreamStorage* storage)
{
    bool busy = false;
    for(unsigned i = 0; i < storage->output_reliable_size && !busy; ++i)
    {
        busy = uxr_is_output_reliable_stream_busy(&storage->output_reliable[i]);
    }
    return !busy;
}
