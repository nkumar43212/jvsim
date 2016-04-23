//
//  OCTelemetryJson.hpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/22/16.
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
    static bool parse_string_to_json_obj(std::string &str,
                                         Json::Value &json_obj);
    
    static std::string write_json_obj_to_string(Json::Value &json_obj);
    
    static void set_json_sensor_group(std::string sensor_group_name,
                                      std::string path,
                                      std::string filter,
                                      Json::Value *json_obj);
    
    static void set_json_destination_group(std::string dest_group_name,
                                           std::string grpc_ip,
                                           std::string grpc_port,
                                           Json::Value *json_obj);
    
    static void set_json_subscription(bool mqtt,
                                      uint32_t internal_subscription_id,
                                      std::string sensor_group_name,
                                      uint32_t sampling_frequency,
                                      std::string destination_group_name,
                                      Json::Value *json_obj);
    
    static void merge_json_objects(Json::Value& super, Json::Value& sub);
};

#endif /* OCTelemetryJson_hpp */