//
//  agent_test_global_config_ut.cpp
//
//  Created: 5/27/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "agent_test_global_config_ut.hpp"

TEST_F(GlobalConfigTest, init)
{
    EXPECT_STREQ("0.0.0.0", global_config.grpc_server_ip.c_str());
    EXPECT_STREQ(SYSTEM_MODE_PROC, global_config.system_mode.c_str());
}

TEST_F(GlobalConfigTest, parse)
{
    GlobalConfig::parse(global_config_ini, global_config);
    EXPECT_STREQ(TOPIC_INTERNAL_SUB_ID, global_config.subscribe_topic_name.c_str());
    EXPECT_STREQ(SYSTEM_MODE_FILE, global_config.system_mode.c_str());
    EXPECT_STREQ("/tmp", global_config.grpc_log_file_path.c_str());
}
