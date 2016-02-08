//
//  AgentClient.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//
#include <iostream>
#include <fstream>
#include "AgentClient.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>

// List of active clients
using std::map;
map<const std::string, AgentClient *> active_clients;

AgentClient *
AgentClient::create (std::shared_ptr<Channel> channel, std::string& name, uint32_t id)
{
    // Create a client
    AgentClient *client = new AgentClient(channel, name, id);
    active_clients[name] = client;
    return client;
}

AgentClient::~AgentClient ()
{
    map<const std::string, AgentClient *>::iterator itr;
    
    itr = active_clients.find(_name);
    if (itr != active_clients.end()) {
        active_clients.erase(itr);
    }
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
AgentClient::print ()
{
    map<const std::string, AgentClient *>::iterator itr;
    
    for (itr = active_clients.begin(); itr != active_clients.end(); itr++) {
        std::cout << "Subscription = " << itr->second->getName() << " Active = " << itr->second->getActive() << " ID = "<< itr->second->getId() << "\n";
        std::cout << "   ServerID  = " << itr->second->getServerId() << "(valid = " << itr->second->getServerIdValid() << ")\n";
    }
}

void
AgentClient::subscribeTelemetry (std::vector<std::string> path_list,
                                 uint32_t limit_records,
                                 uint32_t limit_seconds)
{
    // Create a request
    Path *path;
    SubscriptionRequest request;
    
    // Set up the paths
    for (std::vector<std::string>::iterator itr = path_list.begin(); itr != path_list.end(); itr++) {
        path = request.add_path_list();
        path->set_path(*itr);
    }

    // Set the limits
    if (limit_records) {
        request.set_limit_records(limit_records);
    }
    if (limit_seconds) {
        request.set_limit_time_seconds(limit_seconds);
    }
    
    // Create a reader
    ClientContext context;
    std::multimap<grpc::string_ref, grpc::string_ref> server_metadata;
    std::multimap<grpc::string_ref, grpc::string_ref>::iterator metadata_itr;
    std::unique_ptr<ClientReader<OpenConfigData>> reader(stub_->telemetrySubscribe(&context, request));
    
    // Wait for the initial meta data to come back
    reader->WaitForInitialMetadata();
    server_metadata = context.GetServerInitialMetadata();
    metadata_itr = server_metadata.find("subscription-id");
    if (metadata_itr != server_metadata.end()) {
        _subscription_id_valid = true;
        std::string tmp = metadata_itr->second.data();
        _subscription_id       = atoi(tmp.c_str());
    }
    
    // Print out all the elements received in the metadata
    if (getDebug()) {
        for (metadata_itr = server_metadata.begin(); metadata_itr != server_metadata.end(); metadata_itr++) {
            std::cout << "key = " << metadata_itr->first.data() << ": Value = " << metadata_itr->second.data() << "\n";
        }
    }
   
    // Log file handle
    std::ofstream outputFile(_logfile);

    // Start reading the stream
    OpenConfigData kv;
    while (reader->Read(&kv) && _active) {
        // Print it out
        std::string formatted;
        google::protobuf::TextFormat::PrintToString(kv, &formatted);
        outputFile << "Key Value Message Size = " << kv.ByteSize() << "\n";
        outputFile << formatted;
        outputFile.flush();
    }
    
    // Cleanup
    if (getDebug()) {
        std::cout << _subscription_id << ": Ending subscription session. Active = " << _active << "\n";
    }
    reader->Finish();
}

void
AgentClient::unSubscribeTelemetry ()
{
    // Log the request
    if (getDebug()) {
        std::cout << _subscription_id << ": Unsubscribe\n";
    }
    
    // Break the read loop
    _active = false;
    
    // Remove the list
    map<const std::string, AgentClient *>::iterator itr;
    itr = active_clients.find(_name);
    if (itr == active_clients.end()) {
        std::cout << "Failed to find subscription : " << _name << "\n";
        return;
    }
    active_clients.erase(itr);
    
    // Do we have a valid ID from the server ?
    if (getServerIdValid() == 0) {
        std::cout << "Failed to find Server Subscription ID\n";
        return;
    }
    
    // Send over the unsubscribe
    ClientContext context;
    UnSubscribeRequest request;
    Reply reply;
    request.set_id(getServerId());
    stub_->telemetryUnSubscribe(&context, request, &reply);
    
    // What did the server tell us ?
    if (getDebug()) {
        std::cout << "Server Response : " << reply.code() << "\n";
    }
}

void
AgentClient::listSubscriptions (uint32_t verbosity)
{
    // Send over the list request
    // Create a reader
    ClientContext  context;
    GetRequest     request;
    OpenConfigData data;
    request.set_verbosity(verbosity);
    
    stub_->telemetrySubscriptionsGet(&context, request, &data);
    
    // What did the server tell us ?
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(data, &formatted);
    std::cout << "Server Response : \n" << formatted << "\n";
}

void
AgentClient::getOperational (uint32_t verbosity)
{
    // Create a reader
    ClientContext  context;
    GetRequest     request;
    OpenConfigData data;

    if (!getServerId()) {
        std::cout << "No Server ID allocated\n";
        return;
    }
    request.set_id(getServerId());
    request.set_verbosity(verbosity);
    stub_->telemetryOperationalStateGet(&context, request, &data);
    
    // Print out what came back
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(data, &formatted);
    std::cout << formatted;
}


