//
//  AgentClient.cpp
//  agent-jv-client
//
//  Created by NITIN KUMAR on 12/29/15.
//  CoAuthors: ABBAS SAKARWALA
//
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include "AgentClient.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include "AgentClientLag.hpp"
#include "AgentTelegrafLog.hpp"

// Server address
std::string server_ip("localhost");
std::string server_port("50051");

// List of active clients
using std::map;
map<const std::string, AgentClient *> active_clients;
std::mutex _client_mutex;

// Location of logs
std::string AGENTCLIENT_LOG_DIR;

AgentClient *
AgentClient::create (std::shared_ptr<Channel> channel,
                     std::string& name,
                     uint32_t id,
                     const std::string &logfile_dir)
{
    // Grab the lock
    std::lock_guard<std::mutex> guard(_client_mutex);

    // Create a client
    AgentClient *client = new AgentClient(channel, name, id, logfile_dir);
    active_clients[name] = client;
    return client;
}

AgentClient::~AgentClient (void)
{
    map<const std::string, AgentClient *>::iterator itr;

    // Grab the lock
    std::lock_guard<std::mutex> guard(_client_mutex);

    itr = active_clients.find(_name);
    if (itr != active_clients.end()) {
        active_clients.erase(itr);
    }
}

void
AgentClient::rehome (void)
{
    stub_ = OpenConfigTelemetry::NewStub(grpc::CreateChannel(AGENT_SERVER_IP_PORT,
                                         grpc::InsecureChannelCredentials()));
}

AgentClient *
AgentClient::find (std::string name)
{
    map<const std::string, AgentClient *>::iterator itr;

    itr = active_clients.find(name);
    if (itr != active_clients.end()) {
        return itr->second;
    }

    return NULL;
}

void
AgentClient::print (void)
{
    map<const std::string, AgentClient *>::iterator itr;

    for (itr = active_clients.begin(); itr != active_clients.end(); itr++) {
        std::cout << std::endl << "Subscription = " << itr->second->getName()
                  << ", ID = "<< itr->second->getId() << " (Active = "
                  << itr->second->getActive() << ")" << std::endl;
        std::cout << "   Server/Subscription ID  = "
                  << itr->second->getServerId()
                  << " (Valid = " << itr->second->getServerIdValid() << ")"
                  << std::endl;
    }
}

void
AgentClient::subscribeTelemetry (std::vector<std::string> path_list,
                                 uint32_t sample_frequency,
                                 bool     telegraf,
                                 uint32_t limit_records,
                                 uint32_t limit_seconds)
{
    // Create a request
    Path *path;
    SubscriptionRequest request;
    SubscriptionAdditionalConfig *p_add_config =
                                        request.mutable_additional_config();
    SubscriptionReply reply;
    SubscriptionResponse *response;

    // Set up the paths
    for (std::vector<std::string>::iterator itr = path_list.begin();
         itr != path_list.end(); itr++) {
        path = request.add_path_list();
        path->set_path(*itr);
        path->set_sample_frequency(sample_frequency);
    }

    // Set the limits
    if (limit_records) {
        p_add_config->set_limit_records(limit_records);
    }
    if (limit_seconds) {
        p_add_config->set_limit_time_seconds(limit_seconds);
    }

    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<ClientReader<OpenConfigData>>
                        reader(stub_->telemetrySubscribe(&context, request));

    // Wait for the initial meta data to come back
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("init-response");
    if (metadata_itr != server_metadata.end()) {
        _subscription_id_valid = true;
        std::string tmp = metadata_itr->second.data();
        // Use Textformat Printer APIs to convert to right format
        // std::cout << "Data received = " << tmp << std::endl;
        google::protobuf::TextFormat::Parser parser;
        parser.ParseFromString(tmp, &reply);
        response = reply.mutable_response();
        _subscription_id = response->subscription_id();
    } else {
        std::cout << "Error in Metadata iterator." << std::endl;
        return;
    }

    // Std on the terminal
    std::cout << std::endl << "Received Subcription Id = "
              << _subscription_id << std::endl;
    if (reply.path_list_size()) {
        for (int i = 0; i < reply.path_list_size(); i++) {
            // Convert request to string
            std::string message_str;
            // Serialize the data in text format
            google::protobuf::TextFormat::Printer printer;
            // Use single line mode
            printer.SetSingleLineMode(true);
            printer.PrintToString(reply.path_list(i), &message_str);
            std::cout << "Path[" << i << "]: " << message_str << std::endl;
        }
    } else {
        std::cout << "*** No Paths accepted ***" << std::endl;
    }

    // Log file handle
    AgentServerLog *logger = new AgentServerLog(_logfile);
    logger->enable();

    // Telegraf file handle
    AgentTelegrafLog *telegraf_logger = new AgentTelegrafLog(_logfile);

    // Start reading the stream
    OpenConfigData kv;
    while (reader->Read(&kv) && _active) {
        // Log the message size
        logger->log("Message Size = " + std::to_string(kv.ByteSize()));

        // Print it out
        std::string formatted;
        google::protobuf::TextFormat::PrintToString(kv, &formatted);
        logger->log(formatted);
        logger->log("-----");

        // If this is a port interface subscription update the LAG
        AgentLag::updateStats(&kv);

        // Are we to send a telegram ?
        if (telegraf) {
            telegraf_logger->updateStats(&kv);   
        }
    }

    // Cleanup
    std::cout << "Ending subscription session for Id = "
              << _subscription_id << std::endl;

    delete logger;
    delete telegraf_logger;
    reader->Finish();
}

void
AgentClient::cancelSubscribeTelemetry (void)
{
    std::cout << std::endl << "Unsubscribe Subcription Id = "
              << _subscription_id << std::endl;

    // Do we have a valid ID from the server ?
    if (getServerIdValid() == 0) {
        std::cout << "Failed to find Server/Subscription Id" << std::endl;
        return;
    }

    // Send over the unsubscribe
    ClientContext context;
    CancelSubscriptionRequest request;
    CancelSubscriptionReply reply;
    request.set_subscription_id(getServerId());
    stub_->cancelTelemetrySubscription(&context, request, &reply);

    // What did the server tell us ?
    std::cout << "Server Response code : " << reply.code() << std::endl;
    std::cout << "Server Response string : " << reply.code_str() << std::endl;

    if (reply.code() == telemetry::SUCCESS) {
        // Break the read loop
        _active = false;
    }
}

void
AgentClient::cancelSubscribeTelemetryId (uint32_t subscription_id)
{
    std::cout << std::endl << "Unsubscribe Subcription Id = "
                           << subscription_id << std::endl;

    // Send over the unsubscribe
    ClientContext context;
    CancelSubscriptionRequest request;
    CancelSubscriptionReply reply;
    request.set_subscription_id(subscription_id);
    stub_->cancelTelemetrySubscription(&context, request, &reply);

    // What did the server tell us ?
    std::cout << "Server Response code : " << reply.code() << std::endl;
    std::cout << "Server Response string : " << reply.code_str() << std::endl;
}

void
AgentClient::listSubscriptions (uint32_t subscription_id)
{
    // Send over the list request
    ClientContext  get_context;
    GetSubscriptionsRequest get_request;
    GetSubscriptionsReply get_reply;
    get_request.set_subscription_id(subscription_id);
    stub_->getTelemetrySubscriptions(&get_context, get_request, &get_reply);
    // Iterate throught the list
    for (int i = 0; i < get_reply.subscription_list_size(); i++) {
        SubscriptionReply *sub_reply = get_reply.mutable_subscription_list(i);
        const SubscriptionResponse &sub_response = sub_reply->response();
        std::cout << std::endl << "[" << i << "] ---> Subscription Id = "
                  << sub_response.subscription_id() << std::endl;
        int path_list_size = sub_reply->path_list_size();
        for (int lz = 0; lz < path_list_size; lz++) {
            telemetry::Path path = sub_reply->path_list(lz);
            std::cout << "Path[" << lz << "]: " << path.path() << std::endl;
            std::cout << "Filter[" << lz << "]: " << path.filter() << std::endl;
            std::cout << "Sup Unchanged[" << lz << "]: "
                      << path.suppress_unchanged() << std::endl;
            std::cout << "Max Silent Interval[" << lz << "]: "
                      << path.max_silent_interval() << std::endl;
            std::cout << "Sample Freq[" << lz << "]: "
                      << path.sample_frequency() << std::endl;
        }
    }
}

void
AgentClient::getOperational (uint32_t subscription_id,
                             telemetry::VerbosityLevel verbosity)
{
    ClientContext  context;
    GetOperationalStateRequest  operational_request;
    GetOperationalStateReply operational_reply;
    telemetry::KeyValue *kv;
    std::string subscription_id_str("subscription_id");
    std::string agent_stats_str("agent-stats");
    std::string begin_str("begin");

    operational_request.set_subscription_id(subscription_id);
    operational_request.set_verbosity(verbosity);
    stub_->getTelemetryOperationalState(&context,
                                    operational_request, &operational_reply);

    for (int lz = 0; lz < operational_reply.kv_size(); lz++) {
        kv = operational_reply.mutable_kv(lz);
        // Print efficiently
        if (kv->key() == subscription_id_str) {
            std::cout << std::endl;
        }
        if (kv->key() == agent_stats_str) {
            if (kv->str_value() == begin_str) {
                std::cout << std::endl;
            }
        }

        // Print key
        std::cout << kv->key() << ": ";

        // Print value
        switch (kv->value_case()) {
            case KeyValue::kDoubleValue:
                std::cout << kv->double_value() << std::endl;
                break;
            case KeyValue::kIntValue:
                std::cout << kv->int_value() << std::endl;
                break;
            case KeyValue::kUintValue:
                std::cout << kv->uint_value() << std::endl;
                break;
            case KeyValue::kSintValue:
                std::cout << kv->sint_value() << std::endl;
                break;
            case KeyValue::kBoolValue:
                std::cout << kv->bool_value() << std::endl;
                break;
            case KeyValue::kStrValue:
                std::cout << kv->str_value() << std::endl;
                break;
            case KeyValue::kBytesValue:
                std::cout << kv->bytes_value() << std::endl;
                break;
            default:
                std::cout << "Value Error" << std::endl;
                break; 
        }
    }
}
