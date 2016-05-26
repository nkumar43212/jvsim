//
//  agent_test_path_validator_ut.cpp
//
//  Created: 5/25/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "agent_test_path_validator_ut.hpp"

TEST_F(PathValidatorTest, init) {
    EXPECT_TRUE(path_validator != nullptr);
}

TEST_F(PathValidatorTest, good_path) {
    // Create path and check accepted_path
    Telemetry::Path path, accepted_path;

    // Create path
    path.set_path("/junos/system/linecard/cpu/memory/");
    path.set_filter("*.*");
    path.set_max_silent_interval(2);
    path.set_sample_frequency(2600);

    // Copy path to accepted path
    accepted_path.CopyFrom(path);

    // Now validate
    path_validator->validate_path(accepted_path);

    EXPECT_STREQ(path.path().c_str(), accepted_path.path().c_str());
    EXPECT_STREQ(path.filter().c_str(), accepted_path.filter().c_str());
    EXPECT_EQ(0, accepted_path.max_silent_interval());
    EXPECT_EQ(3000, accepted_path.sample_frequency());
    EXPECT_GT(path.ByteSize(), accepted_path.ByteSize());
}

TEST_F(PathValidatorTest, bad_path) {
    // Create path and check accepted_path
    Telemetry::Path path, accepted_path;

    // Create path
    path.set_path("/thisisbadpath");
    path.set_max_silent_interval(2);
    path.set_sample_frequency(2600);

    // Copy path to accepted path
    accepted_path.CopyFrom(path);

    // Now validate
    path_validator->validate_path(accepted_path);

    EXPECT_EQ(0, accepted_path.ByteSize());
}
