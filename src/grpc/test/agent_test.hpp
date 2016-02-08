//
//  agent_test.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/31/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_hpp
#define agent_test_hpp
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <stdio.h>

typedef ClientReader<OpenConfigData> AgentClientReader;

// This is a test class
class AgentClientTest: public testing::Test {
public:
    static void *create_subscriptions(void *args);
    static void *delete_subscriptions(void *args);
};

class TestArgs {
public:
    TestArgs (int i, agent::OpenConfigData *d, int N) : index(i), data(d), max_data_size(N), data_size(0) {}
    int    index;
    agent::OpenConfigData *data;
    int max_data_size;
    int data_size;
    uint32_t subscription_id;
    AgentClient *client;
};
#endif /* agent_test_hpp */
