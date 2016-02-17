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

TEST(allocate, id) {
    AgentServerIdManager mgr;
    
    uint32_t i = mgr.allocate();
    EXPECT_TRUE(i == 1);
    
    mgr.deallocate(i);
    i = mgr.allocate();
    EXPECT_TRUE(i == 1);
    
    i = mgr.allocate();
    EXPECT_TRUE(i == 2);
}

TEST(allocate_all, id) {
    AgentServerIdManager mgr;
    uint32_t j = 0;
    
    for (int i = 0; i < ID_SPACE_SIZE - 1; i++) {
        j = mgr.allocate();
    }
    EXPECT_TRUE(j == (ID_SPACE_SIZE - 1));
    mgr.description();
}

TEST(deallocate_all, id) {
    AgentServerIdManager mgr;
    uint32_t j = 0;
    
    for (int i = 0; i < ID_SPACE_SIZE - 1; i++) {
        j = mgr.allocate();
        mgr.deallocate(j);
    }
}