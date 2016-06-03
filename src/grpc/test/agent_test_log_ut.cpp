//
//  agent_test_log_ut.cpp
//
//  Created: 6/2/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "gtest/gtest.h"
#include "agent_test_log_ut.hpp"
#include "AgentServerLog.hpp"

int
readLines (std::string filename) {
    std::ifstream myfile(filename);
    return (int)std::count(std::istreambuf_iterator<char>(myfile),
                           std::istreambuf_iterator<char>(), '\n');
}

TEST_F(LogTest, create_and_write) {
    AgentServerLog *logger = new AgentServerLog(log_file_name);

    // Enable
    logger->enable();
    EXPECT_EQ(2, readLines(log_file_name));

    // Normal message
    logger->log("Print first line");
    EXPECT_EQ(3, readLines(log_file_name));

    // With filter - matching
    logger->logfilter("Print second line", "line");
    EXPECT_EQ(4, readLines(log_file_name));

    // With filter - not matching
    logger->logfilter("Do not print third line", "lineX");
    EXPECT_EQ(4, readLines(log_file_name));

    // Raw message
    logger->log("Print fourth raw line", true);
    EXPECT_EQ(5, readLines(log_file_name));

    // Disable
    logger->disable();
    EXPECT_EQ(7, readLines(log_file_name));

    // Normal message
    logger->log("Do not print fifth line");
    EXPECT_EQ(7, readLines(log_file_name));

    delete logger;
}

TEST_F(LogTest, create_default_file) {
    AgentServerLog *logger = new AgentServerLog();

    // Enable
    logger->enable();
    EXPECT_EQ(2, readLines(AGENTSERVER_LOGFILE));

    delete logger;
}
