//
//  OCTelemetryJson.hpp
//  Telemetry Agent
//
//  Created: 4/22/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OCTelemetryJson_hpp
#define OCTelemetryJson_hpp

#include <string>
#include "json/json.h"

extern std::string sensor_groups;
extern std::string destination_groups;
extern std::string subscriptions_mqtt;
extern std::string subscriptions_udp;

// OpenConfig configuration methods for setting various Json configuration
class OCTelemetryJson {
public:
    static void set_json_sensor_group(std::string sensor_group_name,
                                      std::string path,
                                      std::string filter,
                                      Json::Value *json_obj);

    static void set_json_destination_group(std::string dest_group_name,
                                           std::string grpc_udp_ip,
                                           uint32_t grpc_udp_port,
                                           Json::Value *json_obj);

    static void set_json_subscription(bool mqtt,
                                      uint32_t internal_subscription_id,
                                      std::string sensor_group_name,
                                      uint32_t sample_frequency,
                                      std::string destination_group_name,
                                      Json::Value *json_obj);
};

#endif /* OCTelemetryJson_hpp */