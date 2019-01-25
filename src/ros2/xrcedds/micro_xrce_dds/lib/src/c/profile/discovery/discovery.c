#include <uxr/client/profile/discovery/discovery.h>
#include <uxr/client/core/session/object_id.h>
#include <uxr/client/core/session/stream/seq_num.h>
#include <uxr/client/util/time.h>

#include "../../core/serialization/xrce_protocol_internal.h"
#include "../../core/serialization/xrce_header_internal.h"
#include "../../core/session/submessage_internal.h"
#include "../../core/log/log_internal.h"
#include "transport/udp_transport_datagram_internal.h"

#include <string.h>

#define GET_INFO_MSG_SIZE   8
#define GET_INFO_REQUEST_ID 9

#define MULTICAST_DEFAULT_IP   "239.255.0.2"
#define MULTICAST_DEFAULT_PORT 7400

typedef struct CallbackData
{
    uxrAgentAddress* chosen;
    uxrOnAgentFound on_agent;
    void* args;

} CallbackData;

static void write_get_info_message(ucdrBuffer* mb);
static bool listen_info_message(uxrUDPTransportDatagram* transport, int period, CallbackData* callback);
static bool read_info_headers(ucdrBuffer* mb);
static bool read_info_message(ucdrBuffer* mb, CallbackData* callback);
static bool process_info(CallbackData* callback, Time_t timestamp, TransportLocator* transport);

//==================================================================
//                             PUBLIC
//==================================================================

bool uxr_discovery_agents_multicast(uint32_t attemps, int period, uxrOnAgentFound on_agent_func, void* args, uxrAgentAddress* chosen)
{
    uxrAgentAddress multicast = {MULTICAST_DEFAULT_IP, MULTICAST_DEFAULT_PORT};
    return uxr_discovery_agents_unicast(attemps, period, on_agent_func, args, chosen, &multicast, 1);
}

bool uxr_discovery_agents_unicast(uint32_t attempts, int period, uxrOnAgentFound on_agent_func, void* args, uxrAgentAddress* chosen,
                                 const uxrAgentAddress* agent_list, size_t agent_list_size)
{
    CallbackData callback = {chosen, on_agent_func, args};

    uint8_t output_buffer[UXR_UDP_TRANSPORT_MTU_DATAGRAM];
    ucdrBuffer mb;
    ucdr_init_buffer(&mb, output_buffer, UXR_UDP_TRANSPORT_MTU_DATAGRAM);
    write_get_info_message(&mb);
    size_t message_length = ucdr_buffer_length(&mb);

    bool consumed = false;
    uxrUDPTransportDatagram transport;
    if(uxr_init_udp_transport_datagram(&transport))
    {
        for(uint32_t a = 0; a < attempts && !consumed; ++a)
        {
            for(size_t i = 0; i < agent_list_size && !consumed; ++i)
            {
                (void) uxr_udp_send_datagram_to(&transport, output_buffer, message_length, agent_list[i].ip, agent_list[i].port);
                UXR_DEBUG_PRINT_MESSAGE(UXR_SEND, output_buffer, message_length, 0);
            }

            int64_t timestamp = uxr_millis();
            int poll = period;
            while(0 < poll && !consumed)
            {
                consumed = listen_info_message(&transport, poll, &callback);
                poll -= (int)(uxr_millis() - timestamp);
            }
        }
    }

    return consumed;
}

//==================================================================
//                             INTERNAL
//==================================================================

void write_get_info_message(ucdrBuffer* mb)
{
    GET_INFO_Payload payload;
    payload.base.request_id = (RequestId){{0x00, GET_INFO_REQUEST_ID}};
    payload.base.object_id = OBJECTID_AGENT;
    payload.info_mask = INFO_CONFIGURATION | INFO_ACTIVITY;

    uxr_serialize_message_header(mb, UXR_SESSION_ID_WITHOUT_CLIENT_KEY, 0, 0, 0);
    (void) uxr_buffer_submessage_header(mb, SUBMESSAGE_ID_GET_INFO, GET_INFO_MSG_SIZE, 0);
    (void) uxr_serialize_GET_INFO_Payload(mb, &payload);
}

bool listen_info_message(uxrUDPTransportDatagram* transport, int poll, CallbackData* callback)
{
    bool consumed = false;
    uint8_t* input_buffer; size_t length;

    bool received = uxr_udp_recv_datagram(transport, &input_buffer, &length, poll);
    if(received)
    {
        UXR_DEBUG_PRINT_MESSAGE(UXR_RECV, input_buffer, length, 0);

        ucdrBuffer mb;
        ucdr_init_buffer(&mb, input_buffer, (uint32_t)length);
        if(read_info_headers(&mb))
        {
            consumed = read_info_message(&mb, callback);
        }
    }

    return consumed;
}

bool read_info_headers(ucdrBuffer* mb)
{
    bool valid = false;

    uint8_t session_id; uint8_t stream_id_raw; uxrSeqNum seq_num; uint8_t key[UXR_CLIENT_KEY_SIZE];
    uxr_deserialize_message_header(mb, &session_id, &stream_id_raw, &seq_num, key);

    uint8_t id; uint16_t length; uint8_t flags; uint8_t* payload_it = NULL;
    if(uxr_read_submessage_header(mb, &id, &length, &flags, &payload_it))
    {
        valid = true;
    }

    return valid;
}

bool read_info_message(ucdrBuffer* mb, CallbackData* callback)
{
    bool consumed = false;
    INFO_Payload payload;

    if(uxr_deserialize_INFO_Payload(mb, &payload))
    {
        XrceVersion* version = &payload.object_info.config._.agent.xrce_version;
        TransportLocator* transport = &payload.object_info.activity._.agent.address_seq.data[0];
        Time_t timestamp = payload.object_info.config._.agent.agent_timestamp;

        if(0 == memcmp(version->data, XRCE_VERSION.data, sizeof(XRCE_VERSION.data)))
        {
            consumed = process_info(callback, timestamp, transport);
        }
    }

    return consumed;
}

bool process_info(CallbackData* callback, Time_t timestamp, TransportLocator* locator)
{
    bool consumed = false;
    int64_t nanoseconds = (int64_t)timestamp.seconds + (int64_t)timestamp.nanoseconds * 1000000000L;

    if(locator->format == ADDRESS_FORMAT_MEDIUM)
    {
        callback->chosen->port = locator->_.medium_locator.locator_port;
        uxr_bytes_to_ip(locator->_.medium_locator.address, callback->chosen->ip);

        consumed = callback->on_agent(callback->chosen, nanoseconds, callback->args);
    }

    return consumed;
}

