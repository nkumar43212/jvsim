//
//  JsonUtils.cpp
//  Telemetry Agent
//
//  Created: 4/29/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
///

#include "JsonUtils.hpp"

// Convert string to Json object
bool
JsonUtils::parse_string_to_json_obj (std::string &str,
                                     Json::Value &json_obj)
{
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(str, json_obj);
    return parsingSuccessful;
}

// Convert Json object to string
std::string
JsonUtils::write_json_obj_to_string (Json::Value &json_obj)
{
    Json::FastWriter fw;
    fw.omitEndingLineFeed();
    return fw.write(json_obj);
}

// Merges sub into super (merges two json objects)
void
JsonUtils::merge_json_objects (Json::Value& super, Json::Value& sub)
{
    // Merge into null too
    // if (!super.isObject() || !sub.isObject()) return;

    // Merge all members of sub into super
    for (const auto& key : sub.getMemberNames()) {
        if (super[key].type() == Json::objectValue &&
            sub[key].type() == Json::objectValue) {
            merge_json_objects(super[key], sub[key]);
        } else {
            super[key] = sub[key];
        }
    }
}
