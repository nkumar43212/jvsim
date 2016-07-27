//
//  AgentSubscription.cpp
//  Telemetry Agent
//
//  Created: 1/19/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <mutex>
#include "AgentSubscription.hpp"
#include "OpenConfig.hpp"
#include "lib_oc.h"

// Store of all active subscriptions
std::map<id_idx_t, AgentSubscription *> store;

void
AgentSubscription::_count_message (const struct mosquitto_message* mosqmessage)
{
    // Initialize path for the topic
    // Note --- topic is "/<internal-subscription-id>" coming from Junos devices
    std::string curr_path = "not_found";

    // Convert the topic to curr_path
    if (global_config.subscribe_topic_name == TOPIC_PATH) {
        curr_path = std::string(mosqmessage->topic);
    } else if (global_config.subscribe_topic_name == TOPIC_INTERNAL_SUB_ID) {
        std::string isid_str(mosqmessage->topic);
        // Remove the prepending "/"
        isid_str.erase(0, 1);

        // Convert to internal subscription index
        id_idx_t isid;
        try {
            isid = std::stoi(isid_str);
        } catch (const std::exception& e) {
            isid = 0;
        }

        // Grab the consolidator handle for the subsription
        AgentConsolidatorHandle *conHandle = getSystemSubscription();

        // Iterate through the handle
        for (int i = 0; i < conHandle->getHandleCount(); i++) {
            AgentConsolidatorSystemHandlePtr ptr = conHandle->getHandle(i);
            if (!ptr) {
                continue;
            }

            // Return the local state in the consolidator
            id_idx_t curr_isid = ptr->getInternalSubscriptionId();

            // If topci isid and conHandle isid matches
            if (curr_isid == isid) {
                curr_path = ptr->getPath()->path();
                break;
            }
        }
    }

    // Call the base message count
    MessageBus::on_message_count(curr_path, mosqmessage);
}

void
AgentSubscription::on_message (const struct mosquitto_message* mosqmessage)
{
    uint8_t *buffer = (uint8_t *) mosqmessage->payload;

    // Count message
    _count_message(mosqmessage);

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
    telemetry::OpenConfigData oc_data;

    // Fill in the common header
    oc_data.set_system_id(stream->system_id());
    oc_data.set_component_id(stream->component_id());
    oc_data.set_sub_component_id(stream->sub_component_id());

    // Path is sensor name which is
    // "internal_sensor_name:internal_path:external_path:component"
    // e.g.:
    // "sensor_1006:/junos/system/linecard/cpu/memory/:/junos/system/linecard/cpu/memory/:PFE"
    if (global_config.subscribe_topic_name == TOPIC_PATH) {
        oc_data.set_path(mosqmessage->topic);
    } else if (global_config.subscribe_topic_name == TOPIC_INTERNAL_SUB_ID) {
        oc_data.set_path(stream->sensor_name());
    }

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
        delete stream;
        return;
    }
    
    // Insert export timestamp. The timestamp in the agent header corresponds
    // to time at the source. Inserting the current time will give us an 
    // idea of any latencies inside the system
    oc->insertExportTimestamp(&oc_data);

    // Iterate through the internal fields and translate them to OC
    oc->iterate(JuniperNetworksSensors_handle, &oc_data);

    // Send it over to the server via the transport channel
    if (getActive()) {
        _transport->write(&oc_data);
    }

    // Done
    delete stream;
    return;
}

void
AgentSubscription::getOperational (GetOperationalStateReply* operational_reply,
                                   telemetry::VerbosityLevel verbosity)
{
    // Get stats from the message bus interface
    MessageBus::getOperational(operational_reply, verbosity);

    // If verbose mose is not set, we are done
    if (verbosity == telemetry::VerbosityLevel::TERSE) {
        return;
    }

    // Failures
    telemetry::KeyValue *kv;
    kv = operational_reply->add_kv();
    kv->set_key("mqtt-translation_failures");
    kv->set_int_value(_oc_lookup_failures);

    kv = operational_reply->add_kv();
    kv->set_key("mqtt-stream_open_failures");
    kv->set_int_value(_stream_alloc_failures);

    kv = operational_reply->add_kv();
    kv->set_key("mqtt-stream_parse_failures");
    kv->set_int_value(_stream_parse_failures);
}

bool
AgentSubscription::getClientDisconnects (void)
{
    if (_transport) {
        if (_transport->getServerContext()) {
            if (_transport->getServerContext()->IsCancelled()) {
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}

void AgentSubscription::on_connect (int rc)
{
    _logger->log("Subscribing for  " + getName());

    // Add all the corresponding subscriptions
    if (global_config.subscribe_topic_name == TOPIC_PATH) {
        for (PathList::iterator itr = _path_list.begin();
             itr != _path_list.end();
             itr++) {
            Subscribe(*itr);
        }
    } else if (global_config.subscribe_topic_name == TOPIC_INTERNAL_SUB_ID) {
        int total_handles = _system_subscription->getHandleCount();
        for (int i = 0; i < total_handles; i++) {
            AgentConsolidatorSystemHandlePtr csh =
                                        _system_subscription->getHandle(i);
            id_idx_t isubid = csh->getInternalSubscriptionId();
            Subscribe("/" + std::to_string(isubid));
        }
    }
}
