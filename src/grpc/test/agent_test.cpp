//
//  agent_test.cpp
//
//  Created: 1/31/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
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
    client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    if (!client) {
        return;
    }
    EXPECT_TRUE(client->stub_ != NULL);

    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(10);
    SubscriptionAdditionalConfig *add_config =
                                        request.mutable_additional_config();
    add_config->set_limit_records(5);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader>
                reader(client->stub_->telemetrySubscribe(&context, request));
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
    EXPECT_GT(subscription_id, 0);

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
    client->stub_->cancelTelemetrySubscription(&context_cancel,
                                               cancel_request, &cancel_reply);
    EXPECT_EQ(Telemetry::ReturnCode::NO_SUBSCRIPTION_ENTRY,cancel_reply.code());
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Not Found. ID = " +
                            std::to_string(subscription_id);
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}

TEST_F(AgentClientTest, subscribe_and_graceful_terminate) {
    AgentClient *client;
    uint32_t subscription_id;
    
    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    if (!client) {
        return;
    }
    EXPECT_TRUE(client->stub_ != NULL);

    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(10);
    SubscriptionAdditionalConfig *add_config =
                                        request.mutable_additional_config();
    add_config->set_limit_records(5);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader>
                reader(client->stub_->telemetrySubscribe(&context, request));
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
    EXPECT_GT(subscription_id, 0);

    // Dont wait to read anything for now

    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(subscription_id);
    client->stub_->cancelTelemetrySubscription(&context_cancel,
                                               cancel_request, &cancel_reply);
    EXPECT_EQ(Telemetry::ReturnCode::SUCCESS, cancel_reply.code());
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}

TEST_F(AgentClientTest, list) {
    AgentClient *client;

    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    if (!client) {
        return;
    }
    EXPECT_TRUE(client->stub_ != NULL);

    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(10);
    SubscriptionAdditionalConfig *add_config =
                                        request.mutable_additional_config();
    add_config->set_limit_records(1);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader>
                reader(client->stub_->telemetrySubscribe(&context, request));
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
    EXPECT_GT(subscription_id, 0);

    // Dont wait to read anything for now
    
    // Send over the list request
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(subscription_id);
    client->stub_->getTelemetrySubscriptions(&get_context,
                                             get_request, &get_reply);
    for (int i = 0; i < get_reply.subscription_list_size(); i++) {
        SubscriptionReply *sub_reply = get_reply.mutable_subscription_list(i);
        EXPECT_EQ(subscription_id, sub_reply->response().subscription_id());
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
    client->stub_->cancelTelemetrySubscription(&context_cancel,
                                               cancel_request, &cancel_reply);
    EXPECT_EQ(Telemetry::ReturnCode::SUCCESS, cancel_reply.code());
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}

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
    mgmt_client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                      grpc::InsecureCredentials()),
                                      mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(mgmt_client != NULL);
    if (!mgmt_client) {
        return;
    }
    EXPECT_TRUE(mgmt_client->stub_ != NULL);

    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        data[i] = new Telemetry::OpenConfigData[MAX_RECS];
        args[i] = new TestArgs(i, data[i], MAX_RECS, CLIENT_LOGDIR,
                               GRPC_SERVER_IP_PORT);
        args[i]->limit_record = 5;
        pthread_create(&tid[i], NULL, AgentClientTest::create_subscriptions,
                       (void *)(args[i]));
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
    
    // Reader finish must have automatically terminated all clients.
    // Verify the same.
    // Check whether all subscriptions exist ?
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context,
                                                  get_request, &get_reply);
    EXPECT_EQ(0, get_reply.subscription_list_size());
}

TEST_F(AgentClientTest, verify_multiple_subscribe) {
    int n = MAX_SUBS;
    Telemetry::OpenConfigData *data[MAX_SUBS];
    TestArgs              *args[MAX_SUBS];
    pthread_t              tid[MAX_SUBS];
    AgentClient           *mgmt_client;
    
    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    mgmt_client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                      grpc::InsecureCredentials()),
                                      mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(mgmt_client != NULL);
    if (!mgmt_client) {
        return;
    }
    EXPECT_TRUE(mgmt_client->stub_ != NULL);
    
    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        data[i] = new Telemetry::OpenConfigData[MAX_RECS];
        args[i] = new TestArgs(i, data[i], MAX_RECS, CLIENT_LOGDIR,
                               GRPC_SERVER_IP_PORT);
        args[i]->limit_record = 50000;
        args[i]->return_before_graceful_terminate = true;
        pthread_create(&tid[i], NULL, AgentClientTest::create_subscriptions,
                       (void *)(args[i]));
    }

    // Sleep a random time till all clients are created
    sleep(5);

    // Check whether all subscriptions exist ?
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context,
                                                  get_request, &get_reply);
    EXPECT_EQ(n, get_reply.subscription_list_size());
    for (int sub = 0; sub < n; sub++) {
        bool found = false;
        // Iterate throught the list again ... that's OK for now
        for (int i = 0; i < get_reply.subscription_list_size(); i++) {
            SubscriptionReply *sub_reply =
                                        get_reply.mutable_subscription_list(i);
            if (sub_reply->response().subscription_id() ==
                args[sub]->subscription_id) {
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

    // Reader finish must have automatically terminated all clients.
    // Verify the same.
    // Check whether all subscriptions exist ?
    ClientContext  get_context_2;
    GetSubscriptionsRequest get_request_2;
    GetSubscriptionsReply get_reply_2;
    get_request_2.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context_2,
                                                  get_request_2, &get_reply_2);
    EXPECT_EQ(0, get_reply_2.subscription_list_size());
}

#define OPER_SUB    5
TEST_F(AgentClientTest, get_oper_all) {
    int n = OPER_SUB;
    Telemetry::OpenConfigData *data[MAX_SUBS];
    TestArgs              *args[MAX_SUBS];
    pthread_t              tid[MAX_SUBS];
    AgentClient           *mgmt_client;
    
    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    mgmt_client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                      grpc::InsecureCredentials()),
                                      mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(mgmt_client != NULL);
    if (!mgmt_client) {
        return;
    }
    EXPECT_TRUE(mgmt_client->stub_ != NULL);
    
    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        data[i] = new Telemetry::OpenConfigData[MAX_RECS];
        args[i] = new TestArgs(i, data[i], MAX_RECS, CLIENT_LOGDIR,
                               GRPC_SERVER_IP_PORT);
        args[i]->limit_record = 50000;
        args[i]->return_before_graceful_terminate = true;
        pthread_create(&tid[i], NULL, AgentClientTest::create_subscriptions,
                       (void *)(args[i]));
    }
    
    // Sleep a random time till all clients are created
    sleep(5);
    
    // Check whether all subscriptions exist ?
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context,
                                                  get_request, &get_reply);
    EXPECT_EQ(n, get_reply.subscription_list_size());
    for (int sub = 0; sub < n; sub++) {
        bool found = false;
        // Iterate throught the list again ... that's OK for now
        for (int i = 0; i < get_reply.subscription_list_size(); i++) {
            SubscriptionReply *sub_reply =
                                        get_reply.mutable_subscription_list(i);
            if (sub_reply->response().subscription_id() ==
                args[sub]->subscription_id) {
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

    // Operational state
    ClientContext  operational_context;
    GetOperationalStateRequest  operational_request;
    GetOperationalStateReply operational_reply;
    operational_request.set_subscription_id(0xFFFFFFFF);
    operational_request.set_verbosity(Telemetry::VerbosityLevel::DETAIL);
    mgmt_client->stub_->getTelemetryOperationalState(&operational_context,
                                    operational_request, &operational_reply);

    Telemetry::KeyValue *kv;
    std::string subscription_id_str("subscription_id");
    std::string total_subscriptions_str("total_subscriptions");
    std::string mqtt_total_message_str("mqtt-total_message_count");
    // std::string agent_stats_str("agent-stats");
    // std::string begin_str("begin");
    int64_t subscription_count = 0, total_subscriptions = 0;
    bool found = false;
    for (int lz = 0; lz < operational_reply.kv_size(); lz++) {
        kv = operational_reply.mutable_kv(lz);
        if (kv->key() == subscription_id_str) {
            subscription_count += 1;
        }
        if (kv->key() == total_subscriptions_str) {
            total_subscriptions = kv->int_value();
        }
        if (kv->key() == mqtt_total_message_str) {
            found = true;
        }
    }
    EXPECT_EQ(n, subscription_count);
    EXPECT_EQ(n, total_subscriptions);
    EXPECT_TRUE(found);

    // Now unsubscribe
    for (int i = 0; i < n; i++) {
        AgentClientTest::delete_subscriptions((void *) args[i]);
    }

    // Reader finish must have automatically terminated all clients.
    // Verify the same.
    // Check whether all subscriptions exist ?
    ClientContext  get_context_2;
    GetSubscriptionsRequest get_request_2;
    GetSubscriptionsReply get_reply_2;
    get_request_2.set_subscription_id(0xFFFFFFFF);
    mgmt_client->stub_->getTelemetrySubscriptions(&get_context_2,
                                                  get_request_2, &get_reply_2);
    EXPECT_EQ(0, get_reply_2.subscription_list_size());
}

TEST_F(AgentClientTest, subscribe_and_path_validation_2v_1inv) {
    AgentClient *client;
    uint32_t subscription_id;
    
    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    if (!client) {
        return;
    }
    EXPECT_TRUE(client->stub_ != NULL);

    // Create a request
    SubscriptionRequest request;
    Path *path;
    // Send 3 paths (1 valid, 2 invalid)
    path = request.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(4600);
    path = request.add_path_list();
    path->set_path("port");
    path->set_sample_frequency(4400);
    path = request.add_path_list();
    path->set_path("invalid-resource");
    path->set_sample_frequency(5000);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader>
    reader(client->stub_->telemetrySubscribe(&context, request));
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
    EXPECT_GT(subscription_id, 0);
    EXPECT_EQ(2, reply.path_list_size());
    for (int i = 0; i < reply.path_list_size(); i++) {
        Path path = reply.path_list(i);
        if (i == 0) {
            EXPECT_EQ("firewall", path.path());
            EXPECT_EQ(5000, path.sample_frequency());
        } else if (i == 1) {
            EXPECT_EQ("port", path.path());
            EXPECT_EQ(4000, path.sample_frequency());
        }
    }

    // Dont wait to read anything for now
    
    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(subscription_id);
    client->stub_->cancelTelemetrySubscription(&context_cancel,
                                               cancel_request, &cancel_reply);
    EXPECT_EQ(Telemetry::ReturnCode::SUCCESS, cancel_reply.code());
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}

TEST_F(AgentClientTest, subscribe_and_path_validation_Allinv) {
    AgentClient *client;
    uint32_t subscription_id;

    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    if (!client) {
        return;
    }
    EXPECT_TRUE(client->stub_ != NULL);

    // Create a request
    SubscriptionRequest request;
    Path *path;
    // Send 3 paths (3 invalid)
    path = request.add_path_list();
    path->set_path("firewall-1"); // Note, "firewall-1" and not "firewall"
    path->set_sample_frequency(4600);
    path = request.add_path_list();
    path->set_path("port-1");
    path->set_sample_frequency(4400);
    path = request.add_path_list();
    path->set_path("invalid-resource");
    path->set_sample_frequency(5000);

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader>
    reader(client->stub_->telemetrySubscribe(&context, request));
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
    EXPECT_GT(subscription_id, 0);
    EXPECT_EQ(0, reply.path_list_size());

    // Dont wait to read anything for now

    // Unsubscribe
    ClientContext context_cancel;
    CancelSubscriptionRequest cancel_request;
    CancelSubscriptionReply cancel_reply;
    cancel_request.set_subscription_id(subscription_id);
    client->stub_->cancelTelemetrySubscription(&context_cancel,
                                               cancel_request, &cancel_reply);
    EXPECT_EQ(Telemetry::ReturnCode::SUCCESS, cancel_reply.code());
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);
}

TEST_F(AgentClientTest, encoding) {
    AgentClient *client;

    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    client = AgentClient::create(grpc::CreateChannel(GRPC_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(client != NULL);
    if (!client) {
        return;
    }
    EXPECT_TRUE(client->stub_ != NULL);

    ClientContext context;
    DataEncodingRequest enc_request;
    DataEncodingReply enc_reply;

    client->stub_->getDataEncodings(&context, enc_request, &enc_reply);
    EXPECT_EQ(1, enc_reply.encoding_list_size());
    EXPECT_EQ(Telemetry::EncodingType::PROTO3, enc_reply.encoding_list(0));
}

void *
AgentClientTest::create_subscriptions (void *args)
{
    AgentClient *client;
    TestArgs *test_args = (TestArgs *) args;

    // Create the test client
    std::string client_name("client-" + std::to_string(test_args->index));
    client = AgentClient::create(grpc::CreateChannel(
                                   test_args->grpc_server_ip_port,
                                   grpc::InsecureCredentials()),
                                 client_name, 0, test_args->client_logdir);
    EXPECT_TRUE(client != NULL);
    if (!client) {
        return NULL;
    }
    EXPECT_TRUE(client->stub_ != NULL);
    test_args->client = client;

    // Create a request
    SubscriptionRequest request;
    Path *path;
    path = request.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(5);
    SubscriptionAdditionalConfig *add_config =
                                        request.mutable_additional_config();
    add_config->set_limit_records(test_args->limit_record);

    // Create a reader
    ClientContext context;
    uint32_t subscription_id;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<AgentClientReader>
                reader(client->stub_->telemetrySubscribe(&context, request));
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
    EXPECT_GT(subscription_id, 0);
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
    test_args->client->stub_->cancelTelemetrySubscription(&context_cancel,
                                                cancel_request, &cancel_reply);
    EXPECT_EQ(Telemetry::ReturnCode::SUCCESS, cancel_reply.code());
    const char * code_str = cancel_reply.code_str().c_str();
    std::string expected = "Subscription Successfully Deleted";
    const char * expected_str = expected.c_str();
    EXPECT_STREQ(expected_str, code_str);

    return NULL;
}

/* *********************** SPECIAL STRESS TESTS ************************* */

#define LOCAL_TEST
#define STRESS_TEST_SUB_UNSUB               100
#ifdef LOCAL_TEST
    // Add all the paths we want
    std::string test_paths[] = {
        "cpu_mem",
        "firewall",
        "logical_port",
        "lsp_stats",
        "npu_mem",
        "npu_utilization",
        "optics",
        "packet_stats",
        "port",
        "oc"
    };
    int test_paths_count = sizeof(test_paths)/sizeof(std::string);

#else
    // Add all the paths we want
    std::string test_paths[] = {
        "/junos/system/linecard/interface/",
        "/junos/system/linecard/cpu/memory/",
        "/junos/system/linecard/npu/memory/",
        "/lacp"
    };
    int test_paths_count = sizeof(test_paths)/sizeof(std::string);

#endif /* #ifdef LOCAL_TEST */

TEST_F(AgentClientTest, stress_test_sub_unsub) {
    int count = 0;
    while (count < STRESS_TEST_SUB_UNSUB) {
        AgentClient *client;
        uint32_t subscription_id;
        std::string grpc_server_ip_port(GRPC_SERVER_IP_PORT);

        // Create the test client
        std::string client_name("stress_test");
        client = AgentClient::create(grpc::CreateChannel(grpc_server_ip_port,
                                     grpc::InsecureCredentials()),
                                     client_name, 0, CLIENT_LOGDIR);
        EXPECT_TRUE(client != NULL);
        if (!client) {
            return;
        }
        EXPECT_TRUE(client->stub_ != NULL);

        // Create a request
        SubscriptionRequest request;
        Path *path;
        path = request.add_path_list();
        path->set_path(test_paths[0].c_str());
        path->set_sample_frequency(2000);
        path = request.add_path_list();
        path->set_path(test_paths[1].c_str());
        path->set_sample_frequency(2000);
        path = request.add_path_list();
        path->set_path(test_paths[2].c_str());
        path->set_sample_frequency(2000);
        path = request.add_path_list();
        path->set_path(test_paths[3].c_str());
        path->set_sample_frequency(3000);
        SubscriptionAdditionalConfig *add_config =
                                        request.mutable_additional_config();
        add_config->set_limit_records(0);

        // Create a reader
        ClientContext context;
        std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
        std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
        std::unique_ptr<AgentClientReader>
        reader(client->stub_->telemetrySubscribe(&context, request));
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
        EXPECT_GT(subscription_id, 0);

        // Dont wait to read anything for now
        sleep(1);

        // Unsubscribe
        ClientContext context_cancel;
        CancelSubscriptionRequest cancel_request;
        CancelSubscriptionReply cancel_reply;
        cancel_request.set_subscription_id(subscription_id);
        client->stub_->cancelTelemetrySubscription(&context_cancel,
                                            cancel_request, &cancel_reply);
        EXPECT_EQ(Telemetry::ReturnCode::SUCCESS, cancel_reply.code());
        const char * code_str = cancel_reply.code_str().c_str();
        std::string expected = "Subscription Successfully Deleted";
        const char * expected_str = expected.c_str();
        EXPECT_STREQ(expected_str, code_str);

        // Increment the count
        count++;

        // Display to user the progress
        if ((count % 10) == 0) {
            std::cout << "Ran " << count << " of "
                    << STRESS_TEST_SUB_UNSUB << std::endl;
        }
    }

    // Validate no subscription left
    AgentClient *mgmt_client;
    std::string grpc_server_ip_port(GRPC_SERVER_IP_PORT);

    // Create the test client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    mgmt_client = AgentClient::create(grpc::CreateChannel(grpc_server_ip_port,
                                      grpc::InsecureCredentials()),
                                      mgmt_client_name, 0, CLIENT_LOGDIR);
    EXPECT_TRUE(mgmt_client != NULL);
    if (!mgmt_client) {
        return;
    }
    EXPECT_TRUE(mgmt_client->stub_ != NULL);

    // Operational state
    ClientContext  operational_context;
    GetOperationalStateRequest  operational_request;
    GetOperationalStateReply operational_reply;
    operational_request.set_subscription_id(0xFFFFFFFF);
    operational_request.set_verbosity(Telemetry::VerbosityLevel::DETAIL);
    mgmt_client->stub_->getTelemetryOperationalState(&operational_context,
                                    operational_request, &operational_reply);

    Telemetry::KeyValue *kv;
    std::string total_subscriptions_str("total_subscriptions");
    // std::string agent_stats_str("agent-stats");
    // std::string begin_str("begin");
    int64_t total_subscriptions = 0;
    for (int lz = 0; lz < operational_reply.kv_size(); lz++) {
        kv = operational_reply.mutable_kv(lz);
        if (kv->key() == total_subscriptions_str) {
            total_subscriptions = kv->int_value();
        }
    }
    EXPECT_EQ(0, total_subscriptions);
}
