//
//  agent_test_consolidator_proc.cpp
//
//  Created: 3/23/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include <functional>
#include <string>
#include "gtest/gtest.h"
#include "agent_test_consolidator_proc_ut.hpp"

TEST_F(AgentConsolidatorProcTest, create) {
    EXPECT_TRUE(sys_handle != nullptr);
}

TEST_F(AgentConsolidatorProcTest, get) {
    AgentConsolidatorHandle *handle;
    SubscriptionRequest request;
    SubscriptionRequest system_accepted_request;
    Telemetry::Path *path;

    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");

    // Add it to the consolidator
    handle = cons->addRequest(101, &request, &system_accepted_request);
    EXPECT_TRUE(handle != NULL);
    EXPECT_EQ(2, cons->getSystemRequestCount());
    EXPECT_EQ(1, cons->getAddCount());
    EXPECT_EQ(0, cons->getErrors());
    for (int i = 0; i < request.path_list_size(); i++) {
        EXPECT_STREQ(request.path_list(i).path().c_str(),
                     system_accepted_request.path_list(i).path().c_str());
    }

    // Retrieve the request back and check whether it is the same
    SubscriptionRequest *ptr;
    ptr = cons->getRequest(handle);

    // Make sure that the requests match.
    for (int j = 0; j < request.path_list_size(); j++) {
        bool found = false;
        std::string original_path = request.path_list(j).path();

        for (int i = 0; i < ptr->path_list_size(); i++) {
            Telemetry::Path path = ptr->path_list(i);
            if (path.path() == original_path) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    // Remove it now
    cons->removeRequest(handle);
    EXPECT_EQ(0, cons->getSystemRequestCount());
    EXPECT_EQ(1, cons->getRemCount());
    EXPECT_EQ(0, cons->getErrors());
}

TEST_F(AgentConsolidatorProcTest, getSystem) {
    AgentConsolidatorHandle *handle;
    SubscriptionRequest request;
    SubscriptionRequest system_accepted_request;
    Telemetry::Path *path;

    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");

    // Add it to the consolidator
    handle = cons->addRequest(101, &request, &system_accepted_request);
    EXPECT_TRUE(handle != NULL);
    for (int i = 0; i < request.path_list_size(); i++) {
        EXPECT_STREQ(request.path_list(i).path().c_str(),
                     system_accepted_request.path_list(i).path().c_str());
    }

    // Retrieve the request back and check whether it is the same
    SubscriptionRequest *ptr;
    ptr = cons->getRequest(handle, false);

    // Make sure that the requests match.
    for (int j = 0; j < request.path_list_size(); j++) {
        bool found = false;
        std::string original_path = request.path_list(j).path();

        for (int i = 0; i < ptr->path_list_size(); i++) {
            Telemetry::Path path = ptr->path_list(i);
            if (path.path() == original_path) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    // Remove it now
    cons->removeRequest(handle);
    EXPECT_EQ(0, cons->getSystemRequestCount());
    EXPECT_EQ(0, cons->getErrors());
}
