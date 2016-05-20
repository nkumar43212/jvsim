//
//  agent_test_oc.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 5/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include "agent_test_oc.hpp"

#define GRPC_SERVER_IP_PORT     "localhost:50051"

TEST_F(AgentClientOpenConfigTest, oc_paths) {
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

    for (int i = 0; i < test_paths_count; i++) {
        path = request.add_path_list();
        path->set_path(test_paths[i]);
        path->set_sample_frequency(10);
        SubscriptionAdditionalConfig *add_config =
                                        request.mutable_additional_config();
        add_config->set_limit_records(test_paths_count+1);
    }

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

    // Read records
    bool stop_reading = false;
    int record_read = 0;
    OpenConfigData kv;
    while (reader->Read(&kv) && !stop_reading) {
        // Read all the key values in this stream
        std::string prefix("");

        // The Sensor
        std::string sensor_path = "Sensor = " + kv.path();
        logger->log(sensor_path);
        for (int i = 0; i < kv.kv_size(); i++) {
            const KeyValue &kv_data = kv.kv(i);
            
            if (kv_data.key() == "__prefix__") {
                prefix = kv_data.str_value();
                continue;
            }

            // Append the current prefix
            std::string final_key = prefix + kv_data.key();
            logger->log(final_key);
        }
        record_read++;
        
        if (record_read == test_paths_count) {
            stop_reading = true;
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
