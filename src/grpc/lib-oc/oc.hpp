//
//  oc.hpp
//  agent-jv
//
//  Helper utilities for OC related code
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>

#define oc_set_prefix(datap, base_str, name_str, oc_attr) do {      \
    Telemetry::KeyValue *kv;                                        \
    kv = datap->add_kv();                                           \
    kv->set_key("__prefix__");                                      \
                                                                    \
    std::string prefix_str;                                         \
    std::string START_DELIM("[name='");                             \
    std::string END_DELIM("']/");                                   \
    prefix_str = std::string(base_str + START_DELIM + name_str +    \
                             END_DELIM + oc_attr + "/");            \
    kv->set_str_value(prefix_str);                                  \
} while (0)

#define oc_set_prefix_no_attr(datap, base_str, name_str) do {       \
    Telemetry::KeyValue *kv;                                        \
    kv = datap->add_kv();                                           \
    kv->set_key("__prefix__");                                      \
                                                                    \
    std::string prefix_str;                                         \
    std::string START_DELIM("[name='");                             \
    std::string END_DELIM("']/");                                   \
    prefix_str = std::string(base_str + START_DELIM + name_str +    \
                             END_DELIM);                            \
    kv->set_str_value(prefix_str);                                  \
} while (0)
