//
//  MgdServer.cpp
//  grpc
//
//  Created by NITIN KUMAR on 3/22/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "MgdServer.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <map>

// Configuration Database
// The db of all requests into the system
typedef std::map<std::size_t, ConfigCommand *> ConfigDB;
ConfigDB config_db;

Status
MgdServer::EditConfig (ServerContext *context,
                       const EditConfigRequest *args, EditConfigResponse * response)
{
    // Log the request
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*args, &formatted);
    _logger->log(formatted);
    _logger->log("-----");
    _logger->log("");
    
    // Store it locally, as we will get asked about it
    RequestHdr          request_header;
    request_header = args->requestheader();
    
    ConfigCommand *cmd = new junos_mgd::ConfigCommand;
    cmd->CopyFrom(args->configcommand(0));
    config_db[request_header.requestid()] = cmd;
    
    return Status::OK;
}

Status
MgdServer::GetConfig (ServerContext *context,
                      const GetConfigRequest *args, GetConfigResponse * response)
{
    // Log the request
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*args, &formatted);
    _logger->log(formatted);
    _logger->log("-----");
    _logger->log("");
  
    // Prepare the reply
    ReplyHdr *reply = response->mutable_replyheader();
    OpenConfigDataElement *data = response->add_data();
    
    // Request ID
    size_t request_id = args->requestheader().requestid();
    
    // Look it up and return in the response
    if (config_db.count(request_id) == 0) {
        reply->set_message("Not Found");
        return Status::OK;
    }
    ConfigCommand *cmd = config_db[request_id];
    
    // Fill in the details
    reply->set_requestid(request_id);
    reply->set_message("OK");
    data->CopyFrom(cmd->opencfgdata());
    
    return Status::OK;
}