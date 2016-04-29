//
//  JsonUtils.hpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef JsonUtils_hpp
#define JsonUtils_hpp

#include <string>
#include "json/json.h"

// Common Json Utility functions
class JsonUtils {
public:
    static bool parse_string_to_json_obj(std::string &str,
                                         Json::Value &json_obj);
    static std::string write_json_obj_to_string(Json::Value &json_obj);
    static void merge_json_objects(Json::Value& super, Json::Value& sub);
};

#endif /* JsonUtils_hpp */