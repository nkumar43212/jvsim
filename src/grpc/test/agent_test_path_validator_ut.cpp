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

void
_valid_path_test (PathValidator *path_validator,
                  uint32_t sample_frequency_value,
                  u_int32_t sample_frequency_expected)
{
    // Create path and check accepted_path
    telemetry::Path path, accepted_path;

    // Create path
    path.set_path("/junos/system/linecard/cpu/memory/");
    path.set_max_silent_interval(2);
    path.set_sample_frequency(sample_frequency_value);

    // Copy path to accepted path
    accepted_path.CopyFrom(path);

    // Now validate
    path_validator->validate_path(accepted_path);

    EXPECT_STREQ(path.path().c_str(), accepted_path.path().c_str());
    EXPECT_STREQ(path.filter().c_str(), accepted_path.filter().c_str());
    EXPECT_EQ(0, accepted_path.max_silent_interval());
    EXPECT_EQ(sample_frequency_expected, accepted_path.sample_frequency());
    EXPECT_GT(path.ByteSize(), accepted_path.ByteSize());
}

TEST_F(PathValidatorTest, good_path) {
    _valid_path_test(path_validator, 2600, 3000);
}

TEST_F(PathValidatorTest, bad_path) {
    // Create path and check accepted_path
    telemetry::Path path, accepted_path;

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

TEST_F(PathValidatorTest, sample_frequency_min) {
    _valid_path_test(path_validator, 600, 2000);
}

TEST_F(PathValidatorTest, sample_frequency_max) {
    _valid_path_test(path_validator, 3600001, 3600000);
}

AgentServerLog *
_init_logger (std::string root_path)
{
    // Initialize logger
    std::string path_validator_log = root_path + "/logs/temp";
    AgentServerLog *temp_logger = new AgentServerLog(path_validator_log);
    temp_logger->enable();

    return temp_logger;
}

PathValidator *
_init_path_validator (std::string root_path, AgentServerLog *temp_logger)
{
    // Initialize path validator
    PathValidator *path_validator = new PathValidator(temp_logger);

    return path_validator;
}

TEST_F(PathValidatorTest, filter_unsupported) {
    AgentServerLog *temp_logger = _init_logger(root_path);
    PathValidator *path_validator_temp = _init_path_validator(root_path,
                                                              temp_logger);

    // Provide invalid json file
    bool status  = path_validator_temp->build_path_information_db(root_path +
        "/config/test_config/PathValidator/ocpaths_filter_unsupported.json");
    EXPECT_TRUE(status);

    // Create path and check accepted_path
    telemetry::Path path, accepted_path;

    // Create path
    path.set_path("/junos/system/linecard/fabric/");
    path.set_filter("*.*");

    // Copy path to accepted path
    accepted_path.CopyFrom(path);

    // Now validate
    path_validator_temp->validate_path(accepted_path);

    EXPECT_STREQ(path.path().c_str(), accepted_path.path().c_str());
    EXPECT_STREQ("", accepted_path.filter().c_str());
    EXPECT_EQ(2000, accepted_path.sample_frequency());

    delete path_validator_temp;
    delete temp_logger;
}

void
_various_invalid_parsing_errors (std::string root_path, std::string filename)
{
    AgentServerLog *temp_logger = _init_logger(root_path);
    PathValidator *path_validator_temp = _init_path_validator(root_path,
                                                              temp_logger);

    // Provide invalid json file
    bool status  = path_validator_temp->build_path_information_db(root_path +
                                                                  filename);
    EXPECT_FALSE(status);

    delete path_validator_temp;
    delete temp_logger;
}

TEST_F(PathValidatorTest, file_parsing_error) {
    _various_invalid_parsing_errors(root_path, "/config/agent_server.ini");
}

TEST_F(PathValidatorTest, valid_paths_missing) {
    _various_invalid_parsing_errors(root_path,
        "/config/test_config/PathValidator/ocpaths_valid_paths_missing.json");
}

TEST_F(PathValidatorTest, path_keyword_missing) {
    _various_invalid_parsing_errors(root_path,
        "/config/test_config/PathValidator/ocpaths_path_keyword_missing.json");
}

TEST_F(PathValidatorTest, invalid_format_list) {
    _various_invalid_parsing_errors(root_path,
        "/config/test_config/PathValidator/ocpaths_invalid_format_list.json");
}
