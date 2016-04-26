//
//  agent_test_server.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 2/16/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "agent_test_server.hpp"
#include "AgentServerIdManager.hpp"
#include "AgentServerCmdOptions.hpp"

// Initialize a space manager, and ensure that we don't have bogus values
TEST(id_manager, init) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        EXPECT_EQ(false, mgr.present((id_idx_t)i));
    }
    EXPECT_EQ(0, mgr.count());
    EXPECT_TRUE(mgr.freeCount() == mgr.getSize());
}

// Initialize a space manager, and ensure that we don't have bogus values
// With initial minimum id set to some value
TEST(id_manager, init_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                    mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        EXPECT_EQ(false, mgr.present((id_idx_t)i));
    }
    EXPECT_EQ(0, mgr.count());
    EXPECT_TRUE(mgr.freeCount() == mgr.getSize());
}

// Simple allocations
TEST(id_manager, allocate) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    bool status;

    // allocate once
    id_idx_t i = mgr.allocate();
    EXPECT_EQ(1, i);
    EXPECT_EQ(1, mgr.count());

    // deallocate last allocation
    mgr.deallocate(i, &status);
    EXPECT_EQ(false, status);
    EXPECT_EQ(0, mgr.count());

    // allocate again - 1
    i = mgr.allocate();
    EXPECT_EQ(1, i);

    // allocate again - 2
    i = mgr.allocate();
    EXPECT_EQ(2, i);

    EXPECT_EQ(2, mgr.count());
}

// Simple allocations
// With initial minimum id set to some value
TEST(id_manager, allocate_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                    mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    bool status;
    
    // allocate once
    id_idx_t i = mgr.allocate();
    EXPECT_EQ(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN, i);
    EXPECT_EQ(1, mgr.count());
    
    // deallocate last allocation
    mgr.deallocate(i, &status);
    EXPECT_EQ(false, status);
    EXPECT_EQ(0, mgr.count());
    
    // allocate again - 1
    i = mgr.allocate();
    EXPECT_EQ(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN, i);
    
    // allocate again - 2
    i = mgr.allocate();
    EXPECT_EQ(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN+1, i);
    
    EXPECT_EQ(2, mgr.count());
}

// Allocate all the IDs
TEST(id_manager, allocate_all) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    id_idx_t j = 0;
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        j = mgr.allocate();
    }
    EXPECT_EQ(end, j);
    EXPECT_TRUE(mgr.count() == ((end - start) + 1));
}

// Allocate all the IDs
// With initial minimum id set to some value
TEST(id_manager, allocate_all_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                    mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    id_idx_t j = 0;
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        j = mgr.allocate();
    }
    EXPECT_EQ(end, j);
    EXPECT_TRUE(mgr.count() == ((end - start) + 1));
}

// Allocate and Free
TEST(id_manager, deallocate_all) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    id_idx_t j = 0;
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        bool was_corrupted;

        j = mgr.allocate();
        mgr.deallocate(j, &was_corrupted);
        EXPECT_TRUE(was_corrupted == false);
    }
}

// Allocate and Free
// With initial minimum id set to some value
TEST(id_manager, deallocate_all_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                    mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    id_idx_t j = 0;

    size_t start, end;
    
    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        bool was_corrupted;
        
        j = mgr.allocate();
        mgr.deallocate(j, &was_corrupted);
        EXPECT_TRUE(was_corrupted == false);
    }
}

// Randomly allocate N Ids
TEST(id_manager, allocate_random) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    size_t start, end;
    mgr.getBounds(&start, &end);
    
    size_t size = end - start + 1;
    srand((unsigned)time(0));
    id_idx_t count = rand() % size;
    
    for (id_idx_t i = 0; i < count; i++) {
        mgr.allocate();
    }
    EXPECT_EQ(count, mgr.count());
    EXPECT_TRUE(mgr.freeCount() == (size - count));
}

// Randomly allocate N Ids
// With initial minimum id set to some value
TEST(id_manager, allocate_random_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                    mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    size_t start, end;
    mgr.getBounds(&start, &end);

    size_t size = end - start + 1;
    srand((unsigned)time(0));
    id_idx_t count = rand() % size;

    for (id_idx_t i = 0; i < count; i++) {
        mgr.allocate();
    }
    EXPECT_EQ(count, mgr.count());
    EXPECT_TRUE(mgr.freeCount() == (size - count));
}

// Exhaust the space and then check whether allocations fail
TEST(id_manager, exhaust) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    size_t start, end;
    id_idx_t last = 0;

    // Allocate all
    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        last = mgr.allocate();
    }

    // and then some
    id_idx_t id = mgr.allocate();
    EXPECT_TRUE(id == mgr.getNullIdentifier());

    // Free an ID
    mgr.deallocate(last);

    // Now allocation must pass
    id = mgr.allocate();
    EXPECT_TRUE(id != mgr.getNullIdentifier());
}

// Exhaust the space and then check whether allocations fail
// With initial minimum id set to some value
TEST(id_manager, exhaust_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                    mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    size_t start, end;
    id_idx_t last = 0;

    // Allocate all
    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        last = mgr.allocate();
    }

    // and then some
    id_idx_t id = mgr.allocate();
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