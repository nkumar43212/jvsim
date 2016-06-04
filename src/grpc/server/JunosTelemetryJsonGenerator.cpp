//
//  JunosTelemetryJsonGenerator.cpp
//  Telemetry Agent
//
//  Created: 4/29/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "JunosTelemetryJsonGenerator.hpp"
#include "JunosTelemetryJson.hpp"
#include "JsonUtils.hpp"
#include "AgentUtils.hpp"

#define MILLISECS_TO_SECS(msec)     (msec/1000)
#define GRPC_UDP_COLLECTOR_NAME     "grpc_udp_collector"

std::string
JunosTelemetryJsonGenerator::generate_json_junos_config (bool add,
                                        bool udp,
                                        std::string udp_server_ip,
                                        uint32_t udp_server_port,
                                        id_idx_t internal_subscription_id,
                                        const Telemetry::Path *path)
{
    bool parsingSuccessful;

    // Create export-profile
    Json::Value export_profile_json;
    parsingSuccessful = JsonUtils::parse_string_to_json_obj(export_profile,
                                                          export_profile_json);
    if (parsingSuccessful == false) {
        // Mark error and return empty string
        return "";
    }

    // Set values in export-profile
    std::string export_profile_name = "export_" +
                                      std::to_string(internal_subscription_id);
    JunosTelemetryJson::set_json_export_profile(add,
                                                export_profile_name,
                                                udp ? "udp" : "grpc",
                                                &export_profile_json);

    // Create streaming-server
    Json::Value streaming_server_json;
    if (udp) {
        parsingSuccessful = JsonUtils::parse_string_to_json_obj(streaming_server,
                                                        streaming_server_json);
        if (parsingSuccessful == false) {
            // Mark error and return empty string
            return "";
        }

        // Set values in streaming-server
        JunosTelemetryJson::set_json_streaming_server(add,
                                                GRPC_UDP_COLLECTOR_NAME,
                                                udp_server_ip,
                                                udp_server_port,
                                                &streaming_server_json);

        // This is a small hack, we never delete udp streaming server ever
        // In future, we can configure this one time and never come here
        // TODO ABBAS
        if (!add) {
            streaming_server_json = Json::nullValue;
        }
    }

    // Create sensor
    Json::Value sensor_config_json;
    parsingSuccessful = JsonUtils::parse_string_to_json_obj(sensor_config,
                                                            sensor_config_json);
    if (parsingSuccessful == false) {
        // Mark error and return empty string
        return "";
    }

    // Set values in sensor
    std::string sensor_name = "sensor_" +
                              std::to_string(internal_subscription_id);
    JunosTelemetryJson::set_json_sensor_config(add,
                                               udp,
                                               sensor_name,
                                               GRPC_UDP_COLLECTOR_NAME,
                                               export_profile_name,
                                               path->path(),
                                               path->filter(),
                                               MILLISECS_TO_SECS(
                                                    path->sample_frequency()),
                                               internal_subscription_id,
                                               &sensor_config_json);

    // Now merge contents
    Json::Value final_json;
    JsonUtils::merge_json_objects(final_json, export_profile_json);
    JsonUtils::merge_json_objects(final_json, streaming_server_json);
    JsonUtils::merge_json_objects(final_json, sensor_config_json);

    // Config string
    // ABBAS SPECIAL FIX --- Due to Junos Json implementation limitations,
    // replace "%name% with "name" in final stage
    std::string final_str = JsonUtils::write_json_obj_to_string(final_json);
    AgentUtils::SearchNReplaceString(final_str, "@name@", "name");

    // ABBAS SPECIAL FIX --- Prepend and Append this special tag
    final_str = "<configuration-json>" + final_str + "</configuration-json>";
    return final_str;
}
