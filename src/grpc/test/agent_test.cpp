//
//  agent_test.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/31/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "agent_test.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>

TEST_F(AgentClientTest, subscribe_and_force_terminate) {
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
    path->set_sample_frequency(10);
    SubscriptionAdditionalConfig *add_config = request.mutable_additional_config();
    add_config->set_limit_records(5);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    EXPECT_TRUE(reader != NULL);

    // Wait for the initial meta data to come back
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("init-response");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    // Use Textformat Printer APIs to convert to right format
    // std::cout << "Data received = " << tmp << std::endl;
    google::protobuf::TextFormat::Parser parser;
    SubscriptionReply reply;
    SubscriptionResponse *response;
    parser.ParseFromString(tmp, &reply);
    response = reply.mutable_response();
    subscription_id = response->subscription_id();
    EXPECT_TRUE(subscription_id != 0);

    // Read records
    OpenConfigData kv;
    while (reader->Read(&kv)) {
        EXPECT_STREQ("2.2.2.1", kv.system_id().c_str());
        EXPECT_NE(0, kv.timestamp());
        EXPECT_NE(0, kv.sequence_number());
        EXPECT_STREQ("firewall", kv.path().c_str());
    }

    // Cleanup (This is terminate)
    reader->Finish();

    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(subscription_id);
    client->stub_->cancelTelemetrySubscription(&context_cancel, cancel_request, &cancel_reply);
    EXPECT_TRUE(cancel_reply.code() == Telemetry::NO_SUBSCRIPTION_ENTRY);
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Not Found = " + std::to_string(subscription_id);
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}

TEST_F(AgentClientTest, subscribe_and_graceful_terminate) {
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
    path->set_sample_frequency(10);
    SubscriptionAdditionalConfig *add_config = request.mutable_additional_config();
    add_config->set_limit_records(5);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    EXPECT_TRUE(reader != NULL);

    // Wait for the initial meta data to come back
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("init-response");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    // Use Textformat Printer APIs to convert to right format
    // std::cout << "Data received = " << tmp << std::endl;
    google::protobuf::TextFormat::Parser parser;
    SubscriptionReply reply;
    SubscriptionResponse *response;
    parser.ParseFromString(tmp, &reply);
    response = reply.mutable_response();
    subscription_id = response->subscription_id();
    EXPECT_TRUE(subscription_id != 0);

    // Dont wait to read anything for now

    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(subscription_id);
    client->stub_->cancelTelemetrySubscription(&context_cancel, cancel_request, &cancel_reply);
    EXPECT_TRUE(cancel_reply.code() == Telemetry::SUCCESS);
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
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
    path->set_sample_frequency(10);
    SubscriptionAdditionalConfig *add_config = request.mutable_additional_config();
    add_config->set_limit_records(1);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    EXPECT_TRUE(reader != NULL);

    // Wait for the initial meta data to come back
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("init-response");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    // Use Textformat Printer APIs to convert to right format
    // std::cout << "Data received = " << tmp << std::endl;
    google::protobuf::TextFormat::Parser parser;
    SubscriptionReply reply;
    SubscriptionResponse *response;
    parser.ParseFromString(tmp, &reply);
    response = reply.mutable_response();
    uint32_t subscription_id;
    subscription_id = response->subscription_id();
    EXPECT_TRUE(subscription_id != 0);

    // Dont wait to read anything for now
    
    // Send over the list request
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(subscription_id);
    client->stub_->getTelemetrySubscriptions(&get_context, get_request, &get_reply);
    for (int i = 0; i < get_reply.subscription_list_size(); i++) {
        SubscriptionReply *sub_reply = get_reply.mutable_subscription_list(i);
        EXPECT_TRUE(sub_reply->response().subscription_id() == subscription_id);
        int path_list_size = sub_reply->path_list_size();
        for (int lz = 0; lz < path_list_size; lz++) {
            Telemetry::Path path = sub_reply->path_list(lz);
            EXPECT_STREQ("firewall", path.path().c_str());
        }
    }

    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(subscription_id);
    client->stub_->cancelTelemetrySubscription(&context_cancel, cancel_request, &cancel_reply);
    EXPECT_TRUE(cancel_reply.code() == Telemetry::SUCCESS);
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}

#if 0 /* TODO ABBAS will add later */
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
    path->set_sample_frequency(10);
    SubscriptionAdditionalConfig *add_config = request.mutable_additional_config();
    add_config->set_limit_records(1);
    
    // Create a reader
    ClientContext context;
    uint32_t subscription_id;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("init-response");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    // Use Textformat Printer APIs to convert to right format
    // std::cout << "Data received = " << tmp << std::endl;
    google::protobuf::TextFormat::Parser parser;
    SubscriptionReply reply;
    SubscriptionResponse *response;
    parser.ParseFromString(tmp, &reply);
    response = reply.mutable_response();
    subscription_id = response->subscription_id();
    EXPECT_TRUE(subscription_id != 0);
    OpenConfigData kv;
    while (reader->Read(&kv)) {
    }
    reader->Finish();
    
    // Create a reader
    ClientContext  get_context;
    GetOperationalStateRequest oper_request;
    GetOperationalStateReply oper_reply;
    
    oper_request.set_subscription_id(subscription_id);
    oper_request.set_verbosity(Telemetry::VerbosityLevel::DETAIL);
    client->stub_->getTelemetryOperationalState(&get_context, oper_request, &oper_reply);

    // Verify that all the interesting fields came back
    bool found = false;
    // TODO ABBAS FIXME
    found = true;
    for (int i = 0; i < data.kv_size(); i++) {
        Telemetry::KeyValue kv = data.kv(i);
        if (kv.key() == std::string("total_message_count")) {
            found = true;
        }
    }
    EXPECT_TRUE(found);

    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(subscription_id);
    client->stub_->cancelTelemetrySubscription(&context_cancel, cancel_request, &cancel_reply);
    EXPECT_TRUE(cancel_reply.code() == Telemetry::SUCCESS);
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}
#endif

#define MAX_SUBS 10
#define MAX_RECS 100
TEST_F(AgentClientTest, multiple_subscribe) {
    int n = MAX_SUBS;                      
    Telemetry::OpenConfigData *data[MAX_SUBS];
    TestArgs              *args[MAX_SUBS];
    pthread_t              tid[MAX_SUBS];
    AgentClient           *mgmt_client;

    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    mgmt_client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                      mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(mgmt_client != NULL);
    EXPECT_TRUE(mgmt_client->stub_ != NULL);

    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        data[i] = new Telemetry::OpenConfigData[MAX_RECS];
        args[i] = new TestArgs(i, data[i], MAX_RECS, CLIENT_LOGDIR);
        args[i]->limit_record = 5;
        pthread_create(&tid[i], NULL, AgentClientTest::create_subscriptions, (void *)(args[i]));
    }

    // Wait for all of them to finish
    for (int i = 0; i < n; i++) {
        pthread_join(tid[i], NULL);
    }

    // Go through the data received and validate
    for (int i = 0; i < n; i++) {
        TestArgs *test_args = args[i];
        for (int j = 0; j < test_args->data_size; j++) {
            OpenConfigData *kv = &test_args->data[j];
        }
    }
    
    // Reader finish must have automatically terminated all clients. Verify the same.
    // Check whether all subscriptions exist ?
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context, get_request, &get_reply);
    EXPECT_TRUE(get_reply.subscription_list_size() == 0);
}

TEST_F(AgentClientTest, verify_multiple_subscribe) {
    int n = MAX_SUBS;
    Telemetry::OpenConfigData *data[MAX_SUBS];
    TestArgs              *args[MAX_SUBS];
    pthread_t              tid[MAX_SUBS];
    AgentClient           *mgmt_client;
    
    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    mgmt_client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                      mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(mgmt_client != NULL);
    EXPECT_TRUE(mgmt_client->stub_ != NULL);
    
    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        data[i] = new Telemetry::OpenConfigData[MAX_RECS];
        args[i] = new TestArgs(i, data[i], MAX_RECS, CLIENT_LOGDIR);
        args[i]->limit_record = 50000;
        pthread_create(&tid[i], NULL, AgentClientTest::create_subscriptions, (void *)(args[i]));
    }

    // Sleep a random time till all clients are created
    sleep(5);

    // Check whether all subscriptions exist ?
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context, get_request, &get_reply);
    EXPECT_TRUE(get_reply.subscription_list_size() == n);
    for (int sub = 0; sub < n; sub++) {
        bool found = false;
        // Iterate throught the list again ... that's OK for now
        for (int i = 0; i < get_reply.subscription_list_size(); i++) {
            SubscriptionReply *sub_reply = get_reply.mutable_subscription_list(i);
            if (sub_reply->response().subscription_id() == args[sub]->subscription_id) {
                found = true;
                int path_list_size = sub_reply->path_list_size();
                for (int lz = 0; lz < path_list_size; lz++) {
                    Telemetry::Path path = sub_reply->path_list(lz);
                    EXPECT_STREQ("firewall", path.path().c_str());
                }
                break;
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

    // Reader finish must have automatically terminated all clients. Verify the same.
    // Check whether all subscriptions exist ?
    ClientContext  get_context_2;
    GetSubscriptionsRequest get_request_2;
    GetSubscriptionsReply get_reply_2;
    get_request_2.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context_2, get_request_2, &get_reply_2);
    EXPECT_TRUE(get_reply_2.subscription_list_size() == 0);
}

TEST_F(AgentClientTest, encoding) {
    AgentClient *client;
    
    // Create the test client
    std::string mgmt_client_name("client-list");
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    EXPECT_TRUE(client->stub_ != NULL);
    
    ClientContext context;
    DataEncodingRequest enc_request;
    DataEncodingReply enc_reply;
    
    client->stub_->getDataEncodings(&context, enc_request, &enc_reply);
    EXPECT_TRUE(1 == enc_reply.encoding_list_size());
    EXPECT_TRUE(Telemetry::EncodingType::PROTO3 == enc_reply.encoding_list(0));
}

void *
AgentClientTest::create_subscriptions (void *args)
{
    AgentClient *client;
    TestArgs *test_args = (TestArgs *) args;

    // Create the test client
    std::string client_name("client-" + std::to_string(test_args->index));
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 client_name, 0, test_args->client_logdir);
    EXPECT_TRUE(client != NULL);
    EXPECT_TRUE(client->stub_ != NULL);
    test_args->client = client;

    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(5);
    SubscriptionAdditionalConfig *add_config = request.mutable_additional_config();
    add_config->set_limit_records(test_args->limit_record);

    // Create a reader
    ClientContext context;
    uint32_t subscription_id;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader> reader(client->stub_->telemetrySubscribe(&context, request));
    EXPECT_TRUE(reader != NULL);
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("init-response");
    EXPECT_TRUE(metadata_itr != server_metadata.end());
    std::string tmp = metadata_itr->second.data();
    // Use Textformat Printer APIs to convert to right format
    // std::cout << "Data received = " << tmp << std::endl;
    google::protobuf::TextFormat::Parser parser;
    SubscriptionReply reply;
    SubscriptionResponse *response;
    parser.ParseFromString(tmp, &reply);
    response = reply.mutable_response();
    subscription_id = response->subscription_id();
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

    // Check special case
    if (test_args->return_before_graceful_terminate) {
        return NULL;
    }

    // Cleanup
    reader->Finish();

    return NULL;
}

void *
AgentClientTest::delete_subscriptions (void *args)
{
    TestArgs *test_args = (TestArgs *) args;

    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(test_args->subscription_id);
    test_args->client->stub_->cancelTelemetrySubscription(&context_cancel, cancel_request, &cancel_reply);
    EXPECT_TRUE(cancel_reply.code() == Telemetry::SUCCESS);
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);

    return NULL;
}