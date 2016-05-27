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
    bool status = GlobalConfig::parse(global_config_ini, global_config);
    EXPECT_TRUE(status);
    EXPECT_STREQ(TOPIC_INTERNAL_SUB_ID,
                 global_config.subscribe_topic_name.c_str());
    EXPECT_STREQ(SYSTEM_MODE_FILE, global_config.system_mode.c_str());
    EXPECT_STREQ("/tmp", global_config.grpc_log_file_path.c_str());
}

TEST_F(GlobalConfigTest, subscribe_topic_name)
{
    bool status;
    status = GlobalConfig::is_valid_subscription_topic_name(
                                                        TOPIC_INTERNAL_SUB_ID);
    EXPECT_TRUE(status);

    status = GlobalConfig::is_valid_subscription_topic_name(TOPIC_PATH);
    EXPECT_TRUE(status);

    status = GlobalConfig::is_valid_subscription_topic_name("junk");
    EXPECT_FALSE(status);
}

TEST_F(GlobalConfigTest, system_mode)
{
    bool status;
    status = GlobalConfig::is_valid_system_mode(SYSTEM_MODE_NULL);
    EXPECT_TRUE(status);

    status = GlobalConfig::is_valid_system_mode(SYSTEM_MODE_FILE);
    EXPECT_TRUE(status);

    status = GlobalConfig::is_valid_system_mode(SYSTEM_MODE_PROC);
    EXPECT_TRUE(status);

    status = GlobalConfig::is_valid_system_mode("junk");
    EXPECT_FALSE(status);
}

TEST_F(GlobalConfigTest, running_mode)
{
    bool status;
    status = GlobalConfig::is_valid_running_mode(RUNNING_MODE_OFF_BOX);
    EXPECT_TRUE(status);

    status = GlobalConfig::is_valid_running_mode(RUNNING_MODE_ON_BOX);
    EXPECT_TRUE(status);

    status = GlobalConfig::is_valid_running_mode(SYSTEM_MODE_PROC);
    EXPECT_FALSE(status);
}

TEST_F(GlobalConfigTest, parse_failure)
{
    bool status = GlobalConfig::parse("invalidfile", global_config);
    EXPECT_FALSE(status);
}

TEST_F(GlobalConfigTest, force_validate_ocpath_true)
{
    global_config.validate_ocpaths = true;
    std::cout << global_config;
    EXPECT_TRUE(global_config.validate_ocpaths);
}
