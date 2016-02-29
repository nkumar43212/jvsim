//
//  agent_test_server.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/16/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "AgentClient.hpp"
#include "agent_test.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include "agent_test_server.hpp"
#include "AgentServerIdManager.hpp"
#include "AgentServerCmdOptions.hpp"

// Initialize a space manager, and ensure that we don't have bogus values
TEST(id_manager, init) {
    AgentServerIdManager mgr;
    uint32_t start, end;
    
    mgr.getBounds(&start, &end);
    for (int i = start; i <= end; i++) {
        EXPECT_TRUE(mgr.present(i) == false);
    }
    EXPECT_TRUE(mgr.count() == 0);
    EXPECT_TRUE(mgr.freeCount() == mgr.getSize());
}

// Simple allocations
TEST(id_manager, allocate) {
    AgentServerIdManager mgr;
    bool status;
    
    uint32_t i = mgr.allocate();
    EXPECT_TRUE(i == 1);
    EXPECT_TRUE(mgr.count() == 1);
    
    mgr.deallocate(i, &status);
    EXPECT_TRUE(status == false);
    EXPECT_TRUE(mgr.count() == 0);

    i = mgr.allocate();
    EXPECT_TRUE(i == 1);
    
    i = mgr.allocate();
    EXPECT_TRUE(i == 2);
    
    EXPECT_TRUE(mgr.count() == 2);
}

// Allocate all the IDs
TEST(id_manager, allocate_all) {
    AgentServerIdManager mgr;
    uint32_t j = 0;
    uint32_t start, end;
    
    mgr.getBounds(&start, &end);
    
    for (int i = start; i <= end; i++) {
        j = mgr.allocate();
    }
    EXPECT_TRUE(j == end);
    EXPECT_TRUE(mgr.count() == ((end - start) + 1));
}

// Allocate and Free
TEST(id_manager, deallocate_all) {
    AgentServerIdManager mgr;
    uint32_t j = 0;
    uint32_t start, end;

    mgr.getBounds(&start, &end);
    for (int i = start; i <= end; i++) {
        bool status;
        
        j = mgr.allocate();
        mgr.deallocate(j, &status);
        EXPECT_TRUE(status == false);
    }
}

// Randomly allocate N Ids
TEST(id_manager, allocate_random) {
    
    AgentServerIdManager mgr;
    uint32_t start, end;
    mgr.getBounds(&start, &end);
    
    uint32_t size = end - start + 1;
    srand((unsigned)time(0));
    uint32_t count = rand() % size;
    
    for (int i = 0; i < count; i++) {
        mgr.allocate();
    }
    EXPECT_TRUE(mgr.count() == count);
    EXPECT_TRUE(mgr.freeCount() == (size - count));
}

// Exhaust the space and then check whether allocations fail
TEST(id_manager, exhaust) {
    AgentServerIdManager mgr;
    uint32_t start, end;
    uint32_t last = 0;
    
    // Allocate all
    mgr.getBounds(&start, &end);
    for (int i = start; i <= end; i++) {
        last = mgr.allocate();
    }
    
    // and then some
    uint32_t id = mgr.allocate();
    EXPECT_TRUE(id == mgr.getNullIdentifier());
    
    // Free an ID
    mgr.deallocate(last);
    
    // Now allocation must pass
    id = mgr.allocate();
    EXPECT_TRUE(id != mgr.getNullIdentifier());
}

// Command Lineparsing
TEST(args_parser, null_args) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test");
    argv[0] = (char *) arg1.c_str();
    opts.parseArgs(1, argv);
    EXPECT_TRUE(opts.isSystemModeFile() == false);
    EXPECT_TRUE(opts.isSystemModeProc() == false);
    EXPECT_TRUE(opts.isSystemModeNull() == false);
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
    EXPECT_TRUE(opts.isSystemModeFile() == true);
    EXPECT_TRUE(std::string(opts.getSystemFileName()) == "test_file");
}

TEST(args_parser, proc_args) {
    AgentServerCmdOptions opts;
    char *argv[10];
    std::string arg1("test"), arg2("-p");
    argv[0] = (char *) arg1.c_str();
    argv[1] = (char *) arg2.c_str();
    
    opts.parseArgs(2, argv);
    EXPECT_TRUE(opts.isSystemModeFile() == false);
    EXPECT_TRUE(opts.isSystemModeProc() == true);
    EXPECT_TRUE(opts.isSystemModeNull() == false);
}



