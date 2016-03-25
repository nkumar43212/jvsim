//
//  AgentSystemProc.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <grpc++/grpc++.h>
#include <junos_mgd.pb.h>
#include <junos_mgd.grpc.pb.h>
#include "AgentSystemProc.hpp"

void
AgentSystemProc::systemAdd (SystemId id, const agent::Path *request_path)
{
    // Create a client
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("localhost:50050", grpc::InsecureCredentials());
    stub_ = OpenConfigRpcApi::NewStub(channel);
   
    // Generate a name for the sensor
    std::string sensor_name;
    generateName(request_path, sensor_name);
    
    // Form the request
    EditConfigRequest   request;
    
    // Request Header
    RequestHdr          *request_header;
    request_header = request.mutable_requestheader();
    request_header->set_requestid(id.getId());
    
    // The create command
    ConfigCommand         *cmd;
    OpenConfigDataElement *config_element;
    
    cmd = request.add_configcommand();
    cmd->set_command(junos_mgd::UpdateConfig);
    config_element = cmd->mutable_opencfgdata();
    config_element->set_path("path");
    config_element->set_values(sensor_name);

    // Send over the request
    EditConfigResponse  response;
    grpc::ClientContext context;
    stub_->EditConfig(&context, request, &response);
    
    // Check the request whether everything went OK or not.
}

void
AgentSystemProc::systemRemove (SystemId id, const agent::Path *request_path)
{
}

agent::Path *
AgentSystemProc::systemGet (SystemId id)
{
    // Create a client
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("localhost:50050", grpc::InsecureCredentials());
    stub_ = OpenConfigRpcApi::NewStub(channel);
    
    // Form the request
    GetConfigRequest   request;
    
    // Request Header
    RequestHdr          *request_header;
    request_header = request.mutable_requestheader();
    request_header->set_requestid(id.getId());
    
    // Send over the request
    GetConfigResponse  response;
    grpc::ClientContext context;
    stub_->GetConfig(&context, request, &response);
    
    // Did the reply have the same ID ?
    ReplyHdr reply = response.replyheader();
    if (reply.requestid() != id.getId()) {
        return NULL;
    }
    
    // Is there a path
    if (response.data(0).path() != "path") {
        return NULL;
    }
    
    // Copy Over the paths
    std::string sensor_name(response.data(0).values());
    agent::Path *path = new agent::Path();
    parseName(sensor_name, path);
    
    return path;
}

