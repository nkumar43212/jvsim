//
//  agent_test_arg_parser_ut.cpp
//
//  Created: 2/16/16.
//
//  Authors: ABBAS SAKARWALA
//           NITIN KUMAR
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "agent_test_arg_parser_ut.hpp"
#include "AgentServerCmdOptions.hpp"

// Command Lineparsing
TEST(args_parser, null_args) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test");
    argv[0] = (char *) arg1.c_str();
    opts.parseArgs(1, argv);
    EXPECT_EQ(false, opts.isSystemModeFile());
    EXPECT_EQ(false, opts.isSystemModeProc());
    EXPECT_EQ(false, opts.isSystemModeNull());
    EXPECT_TRUE(opts.getLogFile() == NULL);
}

TEST(args_parser, log_args) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-l"), arg3("test_file");

    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();
    argv[2] = (char *) arg3.c_str();
    opts.parseArgs(3, argv);
    EXPECT_TRUE(std::string(opts.getLogFile()) == "test_file");
}

TEST(args_parser, file_args) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-f"), arg3("test_file");
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();
    argv[2] = (char *) arg3.c_str();

    opts.parseArgs(3, argv);
    EXPECT_EQ(true, opts.isSystemModeFile());
    EXPECT_TRUE(std::string(opts.getSystemFileName()) == "test_file");
}

TEST(args_parser, proc_args) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-p");
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();

    opts.parseArgs(2, argv);
    EXPECT_EQ(false, opts.isSystemModeFile());
    EXPECT_EQ(false, opts.isSystemModeNull());
    EXPECT_EQ(true, opts.isSystemModeProc());
}
