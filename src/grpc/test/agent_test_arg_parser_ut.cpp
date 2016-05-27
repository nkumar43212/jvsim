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
    char *argv[] = {"test", NULL};

    opts.parseArgs(1, argv);

    EXPECT_EQ(false, opts.isSystemModeFile());
    EXPECT_EQ(false, opts.isSystemModeProc());
    EXPECT_EQ(false, opts.isSystemModeNull());
    EXPECT_TRUE(opts.getLogFile() == NULL);
}

TEST(args_parser, log_args) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-l", "log_file", NULL};

    opts.parseArgs(3, argv);
    EXPECT_STREQ("log_file", opts.getLogFile());
}

TEST(args_parser, null_args) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-n", NULL};

    opts.parseArgs(2, argv);

    EXPECT_EQ(true, opts.isSystemModeNull());
}

TEST(args_parser, file_args) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-f", "test_file", NULL};

    opts.parseArgs(3, argv);

    EXPECT_EQ(true, opts.isSystemModeFile());
    EXPECT_STREQ("test_file", opts.getSystemFileName());
}

TEST(args_parser, file_args_no_file) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-f", NULL};

    bool status = opts.parseArgs(2, argv);
    EXPECT_EQ(false, status);
    EXPECT_EQ(false, opts.isSystemModeFile());
}

TEST(args_parser, proc_args) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-p", NULL};

    opts.parseArgs(2, argv);

    EXPECT_EQ(false, opts.isSystemModeFile());
    EXPECT_EQ(false, opts.isSystemModeNull());
    EXPECT_EQ(true, opts.isSystemModeProc());
}

TEST(args_parser, ini_config_test) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-c", "ini_config_file", NULL};

    opts.parseArgs(3, argv);

    EXPECT_STREQ("ini_config_file", opts.getIniConfigFile());
}

TEST(args_parser, invalid_arg) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-x", NULL};

    bool status = opts.parseArgs(2, argv);

    EXPECT_EQ(false, status);
}

TEST(args_parser, invalid_arg_non_print) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-\xa0", NULL};

    bool status = opts.parseArgs(2, argv);

    EXPECT_EQ(false, status);
}

TEST(args_parser, usage) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-u", NULL};

    opts.parseArgs(2, argv);
}

TEST(args_parser, multiple_args) {
    AgentServerCmdOptions opts;
    char *argv[] = {"test", "-u", NULL};

    opts.parseArgs(2, argv);
}

