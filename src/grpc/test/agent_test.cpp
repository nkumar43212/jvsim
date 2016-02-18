//
//  agent_test.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/31/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "AgentClient.hpp"
#include "agent_test.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>

std::string CLIENT_LOGDIR("/Users/nitin/jvsim/logs");

TEST_F(AgentClientTest, subscribe) {
    AgentClient *client;
    uint32_t subscription_id;
    
    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    EXPECT_TRUE(client->stub_ != NULL);
    
    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    request.set_limit_records(5);
    
    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    EXPECT_TRUE(reader != NULL);
    
    // Wait for the initial meta data to come back
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("subscription-id");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    subscription_id = atoi(tmp.c_str());
    EXPECT_TRUE(subscription_id != 0);
    
    // Read records
    OpenConfigData kv;
    while (reader->Read(&kv)) {
        EXPECT_STREQ(kv.system_id().c_str(), "2.2.2.1");
        EXPECT_NE(kv.timestamp(), 0);
        EXPECT_NE(kv.sequence_number(), 0);
        //EXPECT_STRNE(kv.sensor_name().c_str(), "");
    }
    
    // Cleanup
    reader->Finish();

    // Unsubscribe
    ClientContext context_1;
    UnSubscribeRequest request_1;
    Reply reply;
    request_1.set_id(subscription_id);
    client->stub_->telemetryUnSubscribe(&context_1, request_1, &reply);
    
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(reply, &formatted);
    EXPECT_TRUE(reply.code_str() == "Subscription Deleted");
}

TEST_F(AgentClientTest, list) {
    AgentClient *client;
    
    // Create the test client
    std::string mgmt_client_name("client-list");
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    EXPECT_TRUE(client->stub_ != NULL);
    
    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    request.set_limit_records(1);
    
    // Create a reader
    ClientContext context;
    uint32_t subscription_id;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("subscription-id");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    subscription_id = atoi(tmp.c_str());
    OpenConfigData kv;
    while (reader->Read(&kv)) {
    }
    reader->Finish();
    
    // Send over the list request
    ClientContext  get_context;
    GetRequest     get_request;
    OpenConfigData data;
    get_request.set_verbosity(1);
    client->stub_->telemetrySubscriptionsGet(&get_context, get_request, &data);
    
    // What did the server tell us ?
    bool found = false;
    for (int i = 0; i < data.kv_size(); i++) {
        agent::KeyValue kv = data.kv(i);
        if (kv.int_value() == subscription_id) {
            found = true;
        }
    }
    EXPECT_TRUE(found);

    // Remove the subscriptions
    ClientContext context_1;
    UnSubscribeRequest request_1;
    Reply reply;
    request_1.set_id(subscription_id);
    client->stub_->telemetryUnSubscribe(&context_1, request_1, &reply);
}

TEST_F(AgentClientTest, get) {
    AgentClient *client;
    
    // Create the test client
    std::string mgmt_client_name("client-list");
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    EXPECT_TRUE(client->stub_ != NULL);
    
    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    request.set_limit_records(1);
    
    // Create a reader
    ClientContext context;
    uint32_t subscription_id;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("subscription-id");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    subscription_id = atoi(tmp.c_str());
    OpenConfigData kv;
    while (reader->Read(&kv)) {
    }
    reader->Finish();
    
    // Create a reader
    ClientContext  get_context;
    GetRequest     get_request;
    OpenConfigData data;
    
    get_request.set_id(subscription_id);
    get_request.set_verbosity(1);
    client->stub_->telemetryOperationalStateGet(&get_context, get_request, &data);
    
    // Verify that all the interesting fields came back
    bool found = false;
    for (int i = 0; i < data.kv_size(); i++) {
        agent::KeyValue kv = data.kv(i);
        if (kv.key() == std::string("total_message_count")) {
            found = true;
        }
    }
    EXPECT_TRUE(found);

    // Remove the subscriptions
    ClientContext context_1;
    UnSubscribeRequest request_1;
    Reply reply;
    request_1.set_id(subscription_id);
    client->stub_->telemetryUnSubscribe(&context_1, request_1, &reply);
}

#define MAX_SUBS 10
#define MAX_RECS 100
TEST_F(AgentClientTest, multiple_subscribe) {
    int n = MAX_SUBS;                      
    agent::OpenConfigData *data[MAX_SUBS]; 
    TestArgs              *args[MAX_SUBS];
    pthread_t              tid[MAX_SUBS];
    
    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        data[i] = new agent::OpenConfigData[MAX_RECS];
        args[i] = new TestArgs(i, data[i], MAX_RECS);
        pthread_create(&tid[i], NULL, AgentClientTest::create_subscriptions, (void *)(args[i]));
    }
    
    // Wait for all of them to finish
    for (int i = 0; i < n; i++) {
        pthread_join(tid[i], NULL);
    }

    // Check whether all subscriptions exist ?
    ClientContext  get_context;
    GetRequest     get_request;
    OpenConfigData list_data;
    get_request.set_verbosity(1);
    args[0]->client->stub_->telemetrySubscriptionsGet(&get_context, get_request, &list_data);
    
    // Check whether all subscriptions are present
    for (int j = 0; j < n; j++) {
        bool found = false;
        for (int i = 0; i < list_data.kv_size(); i++) {
            agent::KeyValue kv = list_data.kv(i);
            if (kv.int_value() == args[j]->subscription_id) {
                found = true;
            }
        }
        EXPECT_TRUE(found);
    }

    // Go through the data received and validate
    for (int i = 0; i < n; i++) {
        TestArgs *test_args = args[i];
        for (int j = 0; j < test_args->data_size; j++) {
            OpenConfigData *kv = &test_args->data[j];
        }
    }

    // Now unsubscribe
    for (int i = 0; i < n; i++) {
        AgentClientTest::delete_subscriptions((void *) args[i]);
    }
}

void *
AgentClientTest::create_subscriptions (void *args)
{
    AgentClient *client;
    uint32_t subscription_id;
    TestArgs *test_args = (TestArgs *) args;
    
    // Create the test client
    std::string mgmt_client_name("client-" + std::to_string(test_args->index));
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    EXPECT_TRUE(client->stub_ != NULL);
    test_args->client = client;
    
    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    request.set_limit_records(5);
    
    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    EXPECT_TRUE(reader != NULL);
    
    // Wait for the initial meta data to come back
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("subscription-id");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    subscription_id = atoi(tmp.c_str());
    EXPECT_TRUE(subscription_id != 0);
    test_args->subscription_id = subscription_id;
   
    // Read records
    OpenConfigData *kv = test_args->data;
    int x = 0;
    while (reader->Read(kv)) {
        kv++;
        x++;
        if (x == test_args->max_data_size) {
            kv = test_args->data;
        }
    }
    test_args->data_size = x;
    
    // Cleanup
    reader->Finish();
    return NULL;
}

void *
AgentClientTest::delete_subscriptions (void *args)
{
    TestArgs *test_args = (TestArgs *) args;
    
    // Unsubscribe
    ClientContext context_1;
    UnSubscribeRequest request_1;
    Reply reply;
    request_1.set_id(test_args->subscription_id);
    test_args->client->stub_->telemetryUnSubscribe(&context_1, request_1, &reply);
    
    return NULL;
}