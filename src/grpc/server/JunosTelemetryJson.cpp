//
//  JunosTelemetryJson.cpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "JunosTelemetryJson.hpp"

// Junos export-profile configuration
std::string export_profile = R"(
{
    "configuration" : {
        "services" : {
            "analytics" : {
                "export-profile" :
                [
                    {
                        "@" : { "operation" : "delete" },
                        "@name@" : "__EXPORT_PROFILE_NAME__",
                         "transport" : "__TRANSPORT__"
                    }
                ]
            }
        }
    }
}
)";

// Junos streaming-server configuration
std::string streaming_server = R"(
{
    "configuration" : {
        "services" : {
            "analytics" : {
                "streaming-server" :
                [
                    {
                        "@" : { "operation" : "delete" },
                        "@name@" : "__STREAMING_SERVER_NAME__",
                        "remote-address" : "__GRPC_UDP_IP__",
                        "remote-port" : "__GRPC_UDP_PORT__"
                    }
                ]
            }
        }
    }
}
)";

// Junos sensor configuration
std::string sensor_config = R"(
{
    "configuration" : {
        "services" : {
            "analytics" : {
                "sensor" :
                [
                    {
                        "@" : { "operation" : "delete" },
                        "@name@" : "__SENSOR_NAME__",
                        "server-name" : "__STREAMING_SERVER_NAME__",
                        "export-name" : "__EXPORT_PROFILE_NAME__",
                        "resource" : "__PATH_path__",
                        "resource-filter" : "__PATH_filter__",
                        "reporting-rate" : "__PATH_sample_frequency__",
                        "subscription-id" : "__INTERNAL_SUBSCRIPTION_ID__"
                    }
                ]
            }
        }
    }
}
)";

void
JunosTelemetryJson::set_json_export_profile (bool add,
                                             std::string export_profile_name,
                                             std::string transport,
                                             Json::Value *json_obj)
{
    (*json_obj)["configuration"]["services"]["analytics"]["export-profile"][0]
               ["@name@"] = export_profile_name;
    if (add) {
        // Modify necessary fields
        (*json_obj)["configuration"]["services"]["analytics"]["export-profile"]
                   [0].removeMember("@");
        (*json_obj)["configuration"]["services"]["analytics"]["export-profile"]
                   [0]["transport"] = transport;
    } else {
        // Remove unwanted fields
        (*json_obj)["configuration"]["services"]["analytics"]["export-profile"]
                   [0].removeMember("transport");
    }
}

void
JunosTelemetryJson::set_json_streaming_server (bool add,
                                              std::string streaming_server_name,
                                              std::string grpc_udp_ip,
                                              uint32_t grpc_udp_port,
                                              Json::Value *json_obj)
{
    (*json_obj)["configuration"]["services"]["analytics"]["streaming-server"][0]
               ["@name@"] = streaming_server_name;
    if (add) {
        // Modify necessary fields
        (*json_obj)["configuration"]["services"]["analytics"]["streaming-server"]
                   [0].removeMember("@");
        (*json_obj)["configuration"]["services"]["analytics"]["streaming-server"]
                   [0]["remote-address"] = grpc_udp_ip;
        (*json_obj)["configuration"]["services"]["analytics"]["streaming-server"]
                   [0]["remote-port"] = std::to_string(grpc_udp_port);
    } else {
        // Remove unwanted fields
        (*json_obj)["configuration"]["services"]["analytics"]["streaming-server"]
                   [0].removeMember("remote-address");
        (*json_obj)["configuration"]["services"]["analytics"]["streaming-server"]
                   [0].removeMember("remote-port");
    }
}

void
JunosTelemetryJson::set_json_sensor_config (bool add,
                                            bool mqtt,
                                            std::string sensor_name,
                                            std::string streaming_server_name,
                                            std::string export_profile_name,
                                            std::string path,
                                            std::string filter,
                                            uint32_t sampling_frequency,
                                            uint32_t internal_subscription_id,
                                            Json::Value *json_obj)
{
    (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
               ["@name@"] = sensor_name;
    if (add) {
        // Modify necessary fields
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   .removeMember("@");
        if (mqtt == false) {
            (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                       ["server-name"] = streaming_server_name;
        } else {
            (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                       .removeMember("server-name");
        }
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   ["export-name"] = export_profile_name;
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   ["resource"] = path;
        if (filter.length() != 0) {
            (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                       ["resource-filter"] = filter;
        } else {
            (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                       .removeMember("resource-filter");
        }
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   ["reporting-rate"] = std::to_string(sampling_frequency);
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   ["subscription-id"] =
                                    std::to_string(internal_subscription_id);
    } else {
        // Remove unwanted fields
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   .removeMember("server-name");
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   .removeMember("export-name");
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   .removeMember("resource");
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   .removeMember("resource-filter");
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   .removeMember("reporting-rate");
        (*json_obj)["configuration"]["services"]["analytics"]["sensor"][0]
                   .removeMember("subscription-id");
    }
}