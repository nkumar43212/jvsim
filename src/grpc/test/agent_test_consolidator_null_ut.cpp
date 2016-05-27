//
//  agent_test_consolidator_ut.cpp
//
//  Created: 2/18/16.
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
#include "agent_test_consolidator_null_ut.hpp"

TEST_F(AgentConsolidatorNullTest, add) {
    AgentConsolidatorHandle *handle;
    SubscriptionRequest request;
    Telemetry::Path *path;

    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");

    // Add it to the consolidator
    handle = cons->addRequest(61, &request);
    EXPECT_TRUE(handle != NULL);
    EXPECT_EQ(2, cons->getSystemRequestCount());
    EXPECT_EQ(1, cons->getAddCount());
    EXPECT_EQ(0, cons->getErrors());

    // Simple check get call is good
    SubscriptionRequest *test_ptr;
    test_ptr = cons->getRequest(handle, false);
    EXPECT_TRUE(test_ptr != NULL);

    // Remove it now
    cons->removeRequest(handle);
    EXPECT_EQ(0, cons->getSystemRequestCount());
    EXPECT_EQ(1, cons->getRemCount());
    EXPECT_EQ(0, cons->getErrors());
}
