//
//  agent_test_consolidator.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/18/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "AgentConsolidator.hpp"
#include "AgentServerLog.hpp"
#include "agent_test_consolidator.hpp"
#include <iostream>
#include <functional>
#include <string>


TEST_F(AgentConsolidatorTest, add) {
    AgentConsolidatorHandle *handle;
    agent::SubscriptionRequest request;
    agent::Path *path;
    
    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");
    
    // Add it to the consolidator
    handle = cons->addRequest(std::string("test1"), &request);
    EXPECT_TRUE(handle != NULL);
    EXPECT_TRUE(cons->getSystemRequestCount() == 2);
    EXPECT_TRUE(cons->getAddCount() == 1);
    EXPECT_TRUE(cons->getErrors() == 0);
    
    // Remove it now
    cons->removeRequest(handle);
    EXPECT_TRUE(cons->getSystemRequestCount() == 0);
    EXPECT_TRUE(cons->getRemCount() == 1);
    EXPECT_TRUE(cons->getErrors() == 0);
}

TEST_F(AgentConsolidatorTest, add_multiple) {
    AgentConsolidatorHandle *handle1, *handle2;
    agent::SubscriptionRequest request;
    agent::Path *path;
    
    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");
    
    // Add it to the consolidator
    handle1 = cons->addRequest(std::string("test1"), &request);
    EXPECT_TRUE(handle1 != NULL);
    
    // Add it to the consolidator
    handle2 = cons->addRequest(std::string("test2"), &request);
    EXPECT_TRUE(handle2 != NULL);

    // Only two system requests should have been created
    EXPECT_TRUE(cons->getSystemRequestCount() == 2);
    EXPECT_TRUE(cons->getAddCount() == 2);
    EXPECT_TRUE(cons->getErrors() == 0);
    
    // Remove it now
    cons->removeRequest(handle1);
    EXPECT_TRUE(cons->getSystemRequestCount() == 2);
    
    cons->removeRequest(handle2);
    EXPECT_TRUE(cons->getSystemRequestCount() == 0);
    EXPECT_TRUE(cons->getRemCount() == 2);
    EXPECT_TRUE(cons->getErrors() == 0);
    
}

TEST_F(AgentConsolidatorTest, remove_bogus) {
    cons->removeRequest(NULL);
    EXPECT_TRUE(cons->getErrors() == 1);
}

TEST_F(AgentConsolidatorTest, add_multiple_frequency) {
    AgentConsolidatorHandle *handle1, *handle2;
    agent::SubscriptionRequest request1, request2;
    agent::Path *path;
    
    // Build a request
    path = request1.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(10);
    
    path = request1.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(100);
    
    // Add it to the consolidator
    handle1 = cons->addRequest(std::string("test1"), &request1);
    EXPECT_TRUE(handle1 != NULL);
    EXPECT_TRUE(cons->getSystemRequestCount() == 2);
    
    // Add another one with the same footprint
    path = request2.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(10);
    handle2 = cons->addRequest(std::string("test2"), &request2);
    EXPECT_TRUE(handle2 != NULL);
    EXPECT_TRUE(cons->getSystemRequestCount() == 2);
    
    // Remove the first request
    cons->removeRequest(handle1);
    EXPECT_TRUE(cons->getSystemRequestCount() == 1);
    
    // Remove the second request
    cons->removeRequest(handle2);
    EXPECT_TRUE(cons->getSystemRequestCount() == 0);
}

TEST_F(AgentConsolidatorTest, add_concurrent) {
    // Create N concurrent threads to create subscriptions
    int                    n = 30;
    pthread_t              tid[30];
    TestArgs               *targs[30];
    
    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        targs[i] = new TestArgs(cons, i);
        pthread_create(&tid[i], NULL, AgentConsolidatorTest::create, (void *)(targs[i]));
    }
    
    // Wait for all of them to finish
    for (int i = 0; i < n; i++) {
        pthread_join(tid[i], NULL);
    }
    
    // Make sure there were no errrors
    EXPECT_TRUE(cons->getAddCount() == n);
    EXPECT_TRUE(cons->getRemCount() == n);
    EXPECT_TRUE(cons->getSystemRequestCount() == 0);
    EXPECT_TRUE(cons->getErrors() == 0);
}

void *
AgentConsolidatorTest::create (void *args)
{
    TestArgs *test_args = (TestArgs *) args;
    AgentConsolidator *cons = test_args->cons;
    AgentConsolidatorHandle *handle;
    agent::SubscriptionRequest request;
    agent::Path *path;
    
    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");
    
    // Add it to the consolidator
    std::string request_str = "test";
    request_str += std::to_string(test_args->id);
    handle = cons->addRequest(request_str, &request);
    cons->removeRequest(handle);
    
    return NULL;
  }
