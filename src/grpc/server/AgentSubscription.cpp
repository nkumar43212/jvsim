//
//  AgentSubscription.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 1/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <mutex>
#include "AgentSubscription.hpp"
#include "OpenConfig.hpp"
#include "lib_oc.h"

// Store of all active subscriptions
std::map<uint32_t, AgentSubscription *> store;

void
AgentSubscription::on_message(const struct mosquitto_message* mosqmessage)
{
    char *topic     = mosqmessage->topic;
    uint8_t *buffer = (uint8_t *) mosqmessage->payload;

    // Call the base message
    MessageBus::on_message(mosqmessage);

    // Build a telemetry stream
    TelemetryStream *stream = new TelemetryStream;
    if (!stream) {
        _stream_alloc_failures++;
        return;
    }

    // Deserialize the bytes
    std::string data((const char *) buffer, mosqmessage->payloadlen);
    if (!stream->ParseFromString(data)) {
        _stream_parse_failures++;
        delete stream;
        return;
    }

    // Build the OpenConfig format desired towards collector
    Telemetry::OpenConfigData oc_data;

    // Fill in the common header
    oc_data.set_system_id(stream->system_id());
    oc_data.set_component_id(stream->component_id());
    oc_data.set_sub_component_id(stream->sub_component_id());
    // TODO ABBAS --- Change this to extracted path from sensor_name
    oc_data.set_path(topic);
    oc_data.set_sequence_number(stream->sequence_number());
    oc_data.set_timestamp(stream->timestamp());

    // Iterate and convert to data stream
    EnterpriseSensors *EnterpriseSensors_handle;
    JuniperNetworksSensors *JuniperNetworksSensors_handle;
    EnterpriseSensors_handle = stream->mutable_enterprise();
    JuniperNetworksSensors_handle =
            EnterpriseSensors_handle->MutableExtension(juniperNetworks);
    std::string ext_type = get_oc_extension_type(JuniperNetworksSensors_handle);
    // Dispatch based on the extension type
    OpenConfig *oc = OpenConfig::find(ext_type);
    if (!oc) {
        _oc_lookup_failures++;
        return;
    }
    oc->iterate(JuniperNetworksSensors_handle, &oc_data);

    // Send it over to the server via the transport channel
    if (getActive()) {
        _transport->write(&oc_data);
    }

    // Done
    delete stream;
    return;
}
