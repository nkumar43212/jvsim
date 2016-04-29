//
//  OCTelemetryJsonGenerator.cpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/25/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OCTelemetryJsonGenerator.hpp"
#include "OCTelemetryJson.hpp"
#include "JsonUtils.hpp"

std::string
OCTelemetryJsonGenerator::generate_json_oc_config (bool mqtt,
                                     id_idx_t internal_subscription_id,
                                     const Telemetry::Path *path)
{
    bool parsingSuccessful;

    // Create sensor_groups
    Json::Value sensor_groups_json;
    parsingSuccessful = JsonUtils::parse_string_to_json_obj(sensor_groups,
                                                            sensor_groups_json);
    if (parsingSuccessful == false) {
        // Mark error and return empty string
        return "";
    }

    // Set values in sensor_groups
    std::string sensor_group_name = "Sensor_" +
                                    std::to_string(internal_subscription_id);
    OCTelemetryJson::set_json_sensor_group(sensor_group_name,
                                           path->path(),
                                           path->filter(),
                                           &sensor_groups_json);

    // Create destination_groups
    Json::Value destination_groups_json;
// TODO ABBAS - For QFX platform
#if 0
    parsingSuccessful =
    JsonUtils::parse_string_to_json_obj(destination_groups,
                                        destination_groups_json);
    if (parsingSuccessful) {
        // Mark error and return empty string
        return "";
    }

    // Set values in destination_groups
    OCTelemetryJson::set_json_destination_group("GRPC_UDP_COLLECTOR",
                                                "1.1.1.1", 10000,
                                                &destination_groups_json);
#endif

    // Create subscriptions
    Json::Value subscriptions_json;
    if (mqtt) {
        parsingSuccessful =
        JsonUtils::parse_string_to_json_obj(subscriptions_mqtt,
                                            subscriptions_json);
    } else {
        parsingSuccessful =
        JsonUtils::parse_string_to_json_obj(subscriptions_udp,
                                            subscriptions_json);
    }
    if (parsingSuccessful == false) {
        // Mark error and return empty string
        return "";
    }

    // Set values in subscriptions
    OCTelemetryJson::set_json_subscription(mqtt, internal_subscription_id,
                                sensor_group_name, path->sample_frequency(),
                                "GRPC_UDP_COLLECTOR",
                                &subscriptions_json);

    // Now merge contents
    Json::Value final_json;
    JsonUtils::merge_json_objects(final_json, sensor_groups_json);
    JsonUtils::merge_json_objects(final_json,destination_groups_json);
    JsonUtils::merge_json_objects(final_json, subscriptions_json);

    return JsonUtils::write_json_obj_to_string(final_json);
}