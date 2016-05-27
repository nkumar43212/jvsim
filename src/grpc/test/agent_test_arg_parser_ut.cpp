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
TEST(args_parser, no_args) {
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
    std::string arg1("test"), arg2("-l"), arg3("log_file");

    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();
    argv[2] = (char *) arg3.c_str();
    opts.parseArgs(3, argv);
    EXPECT_STREQ("log_file", opts.getLogFile());
}

TEST(args_parser, null_args) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-n");

    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();
    opts.parseArgs(2, argv);
    EXPECT_EQ(true, opts.isSystemModeNull());
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
    EXPECT_STREQ("test_file", opts.getSystemFileName());
}

TEST(args_parser, file_args_no_file) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-f");
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();

    bool status = opts.parseArgs(2, argv);
    EXPECT_EQ(false, status);
    EXPECT_EQ(false, opts.isSystemModeFile());
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

TEST(args_parser, ini_config_test) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-c"), arg3("ini_config_file");
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();
    argv[2] = (char *) arg3.c_str();

    opts.parseArgs(3, argv);
    EXPECT_STREQ("ini_config_file", opts.getIniConfigFile());
}

TEST(args_parser, invalid_arg) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-x");
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();

    bool status = opts.parseArgs(2, argv);
    EXPECT_EQ(false, status);
}

TEST(args_parser, invalid_arg_non_print) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string nonprint = "\xa0";
    std::string arg1("test"), arg2(nonprint);
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();

    bool status = opts.parseArgs(2, argv);
    EXPECT_EQ(false, status);
}

TEST(args_parser, usage) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-u");
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();

    opts.parseArgs(2, argv);
}
