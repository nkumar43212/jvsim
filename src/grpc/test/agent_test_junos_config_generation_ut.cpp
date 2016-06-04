//
//  agent_test_junos_config_generation_ut.cpp
//
//  Created: 6/3/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <algorithm>
#include "gtest/gtest.h"
#include "agent_test_junos_config_generation_ut.hpp"
#include "AgentServerIdManager.hpp"
#include "JunosTelemetryJsonGenerator.hpp"

// Remove space and new line
void
_remove_space_and_new_line (std::string &str) {
    str.erase(std::remove(str.begin(), str.end(), ' '),str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'),str.end());
}

std::string mx_add_server_config = R"(
    <configuration-json>
    {
        "configuration" : {
            "services" : {
                "analytics" : {
                    "export-profile" :
                    [
                        {
                            "name" : "export_1000",
                            "transport" : "grpc"
                        }
                    ],
                    "sensor" :
                    [
                        {
                            "name" : "sensor_1000",
                            "export-name" : "export_1000",
                            "reporting-rate" : "2",
                            "resource" : "/junos/system/linecard/npu/memory",
                            "resource-filter" : "*.*",
                            "subscription-id" : "1000"
                        }
                    ]
                }
            }
        }
    }
    </configuration-json>
)";

std::string mx_delete_server_config = R"(
    <configuration-json>
    {
        "configuration" : {
            "services" : {
                "analytics" : {
                    "export-profile" :
                    [
                        {
                            "@" : { "operation" : "delete" },
                            "name" : "export_1000"
                        }
                    ],
                    "sensor" :
                    [
                        {
                            "@" : { "operation" : "delete" },
                            "name" : "sensor_1000"
                        }
                    ]
                }
            }
        }
    }
    </configuration-json>
)";

std::string qfx_add_server_config = R"(
    <configuration-json>
    {
        "configuration" : {
            "services" : {
                "analytics" : {
                    "export-profile" :
                    [
                        {
                            "name" : "export_1000",
                            "transport" : "udp"
                        }
                    ],
                    "sensor" :
                    [
                        {
                            "name" : "sensor_1000",
                            "export-name" : "export_1000",
                            "reporting-rate" : "2",
                            "resource" : "/junos/system/linecard/npu/memory",
                            "resource-filter" : "*.*",
                            "server-name" : "grpc_udp_collector",
                            "subscription-id" : "1000"
                        }
                    ],
                    "streaming-server" :
                    [
                        {
                            "name" : "grpc_udp_collector",
                            "remote-address" : "1.1.1.1",
                            "remote-port" : "10000"
                        }
                    ]
                }
            }
        }
    }
    </configuration-json>
)";

std::string qfx_delete_server_config = R"(
    <configuration-json>
    {
        "configuration" : {
            "services" : {
                "analytics" : {
                    "export-profile" :
                    [
                        {
                            "@" : { "operation" : "delete" },
                            "name" : "export_1000"
                        }
                    ],
                    "sensor" :
                    [
                        {
                            "@" : { "operation" : "delete" },
                            "name" : "sensor_1000"
                        }
                    ]
                }
            }
        }
    }
    </configuration-json>
)";

TEST(JsonConfigGeneration, mx_add_sensor_config) {
    bool add = true;

    bool udp = false;
    std::string udp_server_ip = "1.1.1.1";
    uint32_t udp_server_port = 10000;
    id_idx_t subscription_id = 1000;
    Telemetry::Path *path = new Telemetry::Path();
    path->set_path("/junos/system/linecard/npu/memory");
    path->set_filter("*.*");
    path->set_sample_frequency(2000);

    // Generate config
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
            (add, udp, udp_server_ip, udp_server_port, subscription_id, path);

    _remove_space_and_new_line(mx_add_server_config);
    EXPECT_STREQ(mx_add_server_config.c_str(), config.c_str());
}

TEST(JsonConfigGeneration, mx_delete_sensor_config) {
    bool add = false;

    bool udp = false;
    std::string udp_server_ip = "1.1.1.1";
    uint32_t udp_server_port = 10000;
    id_idx_t subscription_id = 1000;
    Telemetry::Path *path = new Telemetry::Path();
    path->set_path("/junos/system/linecard/npu/memory");
    path->set_filter("*.*");
    path->set_sample_frequency(2000);

    // Generate config
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
        (add, udp, udp_server_ip, udp_server_port, subscription_id, path);

    _remove_space_and_new_line(mx_delete_server_config);
    EXPECT_STREQ(mx_delete_server_config.c_str(), config.c_str());
}

TEST(JsonConfigGeneration, qfx_add_sensor_config) {
    bool add = true;

    bool udp = true;
    std::string udp_server_ip = "1.1.1.1";
    uint32_t udp_server_port = 10000;
    id_idx_t subscription_id = 1000;
    Telemetry::Path *path = new Telemetry::Path();
    path->set_path("/junos/system/linecard/npu/memory");
    path->set_filter("*.*");
    path->set_sample_frequency(2000);

    // Generate config
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
        (add, udp, udp_server_ip, udp_server_port, subscription_id, path);

    _remove_space_and_new_line(qfx_add_server_config);
    EXPECT_STREQ(qfx_add_server_config.c_str(), config.c_str());
}

TEST(JsonConfigGeneration, qfx_delete_sensor_config) {
    bool add = false;

    bool udp = false;
    std::string udp_server_ip = "1.1.1.1";
    uint32_t udp_server_port = 10000;
    id_idx_t subscription_id = 1000;
    Telemetry::Path *path = new Telemetry::Path();
    path->set_path("/junos/system/linecard/npu/memory");
    path->set_filter("*.*");
    path->set_sample_frequency(2000);

    // Generate config
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
        (add, udp, udp_server_ip, udp_server_port, subscription_id, path);

    _remove_space_and_new_line(qfx_delete_server_config);
    EXPECT_STREQ(qfx_delete_server_config.c_str(), config.c_str());
}
