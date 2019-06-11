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
    uxrOnAgentFound on_agent;
    void* args;

} CallbackData;

static void write_get_info_message(ucdrBuffer* ub);
static bool listen_info_message(uxrUDPTransportDatagram* transport, int period, CallbackData* callback);
static bool read_info_headers(ucdrBuffer* ub);
static bool read_info_message(ucdrBuffer* ub, CallbackData* callback);
static bool process_info(CallbackData* callback, TransportLocator* transport);

//==================================================================
//                             PUBLIC
//==================================================================

void uxr_discovery_agents_default(
        uint32_t attempts,
        int period,
        uxrOnAgentFound on_agent_func,
        void* args)
{
    uxrAgentAddress multicast = {MULTICAST_DEFAULT_IP, MULTICAST_DEFAULT_PORT};
    uxr_discovery_agents(attempts, period, on_agent_func, args, &multicast, 1);
}

void uxr_discovery_agents(
        uint32_t attempts,
        int period,
        uxrOnAgentFound on_agent_func,
        void* args,
        const uxrAgentAddress* agent_list,
        size_t agent_list_size)
{
    CallbackData callback;
    callback.on_agent = on_agent_func;
    callback.args = args;

    uint8_t output_buffer[UXR_UDP_TRANSPORT_MTU_DATAGRAM];
    ucdrBuffer ub;
    ucdr_init_buffer(&ub, output_buffer, UXR_UDP_TRANSPORT_MTU_DATAGRAM);
    write_get_info_message(&ub);
    size_t message_length = ucdr_buffer_length(&ub);

    uxrUDPTransportDatagram transport;
    if(uxr_init_udp_transport_datagram(&transport))
    {
        for(uint32_t a = 0; a < attempts; ++a)
        {
            for(size_t i = 0; i < agent_list_size; ++i)
            {
                (void) uxr_udp_send_datagram_to(&transport, output_buffer, message_length, agent_list[i].ip, agent_list[i].port);
                UXR_DEBUG_PRINT_MESSAGE(UXR_SEND, output_buffer, message_length, 0);
            }

            int64_t timestamp = uxr_millis();
            int poll = period;
            while(0 < poll)
            {
                (void) listen_info_message(&transport, poll, &callback);
                poll -= (int)(uxr_millis() - timestamp);
            }
        }
    }
}

//==================================================================
//                             INTERNAL
//==================================================================

void write_get_info_message(ucdrBuffer* ub)
{
    GET_INFO_Payload payload;
    payload.base.request_id = (RequestId){{0x00, GET_INFO_REQUEST_ID}};
    payload.base.object_id = OBJECTID_AGENT;
    payload.info_mask = INFO_CONFIGURATION | INFO_ACTIVITY;

    uxr_serialize_message_header(ub, SESSION_ID_WITHOUT_CLIENT_KEY, 0, 0, 0);
    (void) uxr_buffer_submessage_header(ub, SUBMESSAGE_ID_GET_INFO, GET_INFO_MSG_SIZE, 0);
    (void) uxr_serialize_GET_INFO_Payload(ub, &payload);
}

bool listen_info_message(
        uxrUDPTransportDatagram* transport,
        int poll,
        CallbackData* callback)
{
    uint8_t* input_buffer; size_t length;

    bool received = uxr_udp_recv_datagram(transport, &input_buffer, &length, poll);
    if(received)
    {
        UXR_DEBUG_PRINT_MESSAGE(UXR_RECV, input_buffer, length, 0);

        ucdrBuffer ub;
        ucdr_init_buffer(&ub, input_buffer, (uint32_t)length);
        if(read_info_headers(&ub))
        {
            (void) read_info_message(&ub, callback);
        }
    }

    return received;
}

bool read_info_headers(ucdrBuffer* ub)
{
    uint8_t session_id; uint8_t stream_id_raw; uxrSeqNum seq_num; uint8_t key[CLIENT_KEY_SIZE];
    uxr_deserialize_message_header(ub, &session_id, &stream_id_raw, &seq_num, key);

    uint8_t id; uint16_t length; uint8_t flags;
    return uxr_read_submessage_header(ub, &id, &length, &flags);
}

bool read_info_message(
        ucdrBuffer* ub,
        CallbackData* callback)
{
    bool well_read = false;
    INFO_Payload payload;

    if(uxr_deserialize_INFO_Payload(ub, &payload))
    {
        XrceVersion* version = &payload.object_info.config._.agent.xrce_version;
        TransportLocator* transport = &payload.object_info.activity._.agent.address_seq.data[0];

        if(0 == memcmp(version->data, XRCE_VERSION.data, sizeof(XRCE_VERSION.data)))
        {
            (void) process_info(callback, transport);
            well_read = true;
        }
    }

    return well_read;
}

bool process_info(
        CallbackData* callback,
        TransportLocator* locator)
{
    bool processed = false;

    if(locator->format == ADDRESS_FORMAT_MEDIUM)
    {
        char ip[16];
        uxr_bytes_to_ip(locator->_.medium_locator.address, ip);

        uxrAgentAddress address = {ip, locator->_.medium_locator.locator_port};

        callback->on_agent(&address, callback->args);

        processed = true;
    }

    return processed;
}

