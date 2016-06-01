//
//  OCTelemetryJson.cpp
//  Telemetry Agent
//
//  Created: 4/22/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OCTelemetryJson.hpp"

// OC sensor group configuration
std::string sensor_groups = R"(
{
    "sensor-groups" : {
        "sensor-group" : {
            "config" : {
                "sensor-group-id": "__SENSOR_GROUP_NAME__"
            }
        },
        "sensor-paths":
        [
            {
                "sensor-path": {
                    "config" : {
                        "path" : "__PATH_path__",
                        "exclude-filter" : "__PATH_filter__"
                    }
                }
            }
        ]
    }
}
)";

// OC destination group configuration
std::string destination_groups = R"(
{
    "destination-groups" : {
        "destination-group" : {
            "config" : {
                "group-id" : "__DESTINATION_GROUP_NAME__"
            },
            "destinations" : {
                "destination" : {
                    "config" : {
                        "destination-address" : "__GRPC_UDP_IP__",
                        "destination-port" : "__GRPC_UDP_PORT__",
                        "destination-protocol" : "UDP"
                    }
                }
            }
        }
    }
}
)";

// OC MQTT subscription configuration which does not need destination group
std::string subscriptions_mqtt = R"(
{
    "subscriptions" : {
        "persistent" : {
            "subscription" : {
                "config" : {
                    "subscription-id" : "__INTERNAL_SUBSCRIPTION_ID__"
                },
                "sensor-profiles" : {
                    "sensor-profile" : {
                        "config" : {
                            "sensor-group" : "__SENSOR_GROUP_NAME__",
                            "sample-interval" : "__PATH_sample_frequency__"
                        }
                    }
                }
            }
        }
    }
}
)";

// OC UDP subscription configuration which needs destination group
std::string subscriptions_udp = R"(
{
    "subscriptions" : {
        "persistent" : {
            "subscription" : {
                "config" : {
                    "subscription-id" : "__INTERNAL_SUBSCRIPTION_ID__"
                },
                "sensor-profiles" : {
                    "sensor-profile" : {
                        "config" : {
                            "sensor-group" : "__SENSOR_GROUP_NAME__",
                            "sample-interval" : "__PATH_sample_frequency__"
                        }
                    }
                },
                "destination-groups" : {
                    "destination-group" : {
                        "config" : {
                            "group-id" : "__DESTINATION_GROUP_NAME__"
                        }
                    }
                }
            }
        }
    }
}
)";

// Only for reference - the original subscription OC configuration
std::string subscriptions_original_OC = R"(
{
    "subscriptions" : {
        "persistent" : {
            "subscription" : {
                "config" : {
                    "subscription-id" : "__INTERNAL_SUBSCRIPTION_ID__",
                    "local-source-address" : "__NOT_NEEDED__",
                    "originated-qos-marking" : "__NOT_NEEDED__"
                },
                "sensor-profiles" : {
                    "sensor-profile" : {
                        "config" : {
                            "sensor-group" : "__SENSOR_GROUP_NAME__",
                            "sample-interval" : "__PATH_sample_frequency__"
                        }
                    }
                },
                "destination-groups" : {
                    "destination-group" : {
                        "config" : {
                            "group-id" : "__DESTINATION_GROUP_NAME__"
                        }
                    }
                }
            }
        }
    }
}
)";

void
OCTelemetryJson::set_json_sensor_group (std::string sensor_group_name,
                                        std::string path,
                                        std::string filter,
                                        Json::Value *json_obj)
{
    (*json_obj)["sensor-groups"]["sensor-group"]["config"]["sensor-group-id"] =
                                                            sensor_group_name;
    (*json_obj)["sensor-groups"]["sensor-paths"][0]["sensor-path"]["config"]
            ["path"] = path;
    if (filter.length() != 0) {
        (*json_obj)["sensor-groups"]["sensor-paths"][0]["sensor-path"]["config"]
                   ["exclude-filter"] = filter;
    } else {
        (*json_obj)["sensor-groups"]["sensor-paths"][0]["sensor-path"]
                   ["config"].removeMember("exclude-filter");
    }
}

void
OCTelemetryJson::set_json_destination_group (std::string dest_group_name,
                                             std::string grpc_udp_ip,
                                             uint32_t grpc_udp_port,
                                             Json::Value *json_obj)
{
    (*json_obj)["destination-groups"]["destination-group"]["config"]
               ["group-id"] = dest_group_name;
    (*json_obj)["destination-groups"]["destination-group"]["destinations"]
               ["destination"]["config"]["destination-address"] = grpc_udp_ip;
    (*json_obj)["destination-groups"]["destination-group"]["destinations"]
               ["destination"]["config"]["destination-port"] = grpc_udp_port;
}

void
OCTelemetryJson::set_json_subscription (bool mqtt,
                                        uint32_t internal_subscription_id,
                                        std::string sensor_group_name,
                                        uint32_t sample_frequency,
                                        std::string destination_group_name,
                                        Json::Value *json_obj)
{
    (*json_obj)["subscriptions"]["persistent"]["subscription"]
       ["config"]["subscription-id"] = std::to_string(internal_subscription_id);
    (*json_obj)["subscriptions"]["persistent"]["subscription"]
       ["sensor-profiles"]["sensor-profile"]["config"]["sensor-group"] =
                                                            sensor_group_name;
    (*json_obj)["subscriptions"]["persistent"]["subscription"]
       ["sensor-profiles"]["sensor-profile"]["config"]["sample-interval"] =
                                        std::to_string(sample_frequency);
    if (mqtt == false) {
        (*json_obj)["subscriptions"]["persistent"]["subscription"]
            ["destination-groups"]["destination-group"]["config"]["group-id"] =
                    destination_group_name;
    }
}