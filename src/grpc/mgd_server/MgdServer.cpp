//
//  MgdServer.cpp
//  Junos MGD
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
typedef std::map<std::size_t, SetRequest_ConfigOperationList *> ConfigDB;
ConfigDB config_db;

Status
MgdServer::Set (ServerContext* context, const SetRequest* set_request,
                SetResponse* set_response)
{
    // Log the request
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*set_request, &formatted);
    _logger->log(formatted);

    // Store it locally, as we will get asked about it
    uint64_t request_id = set_request->request_id();

    SetRequest_ConfigOperationList *cmd = new SetRequest_ConfigOperationList;
    cmd->CopyFrom(set_request->config_operation(0));
    
    if ((cmd->operation() == openconfig::SetConfigCommands::UPDATE_CONFIG) ||
         (cmd->operation() == openconfig::SetConfigCommands::REPLACE_CONFIG)) {
        // add
        config_db[request_id] = cmd;
    } else {
        // delete
        config_db.erase(request_id);
    }

    set_response->set_request_id(request_id);
    SetResponse_ResponseList *response = set_response->add_response();
    response->set_operation_id(cmd->operation_id());
    response->set_response_code(openconfig::OpenConfigRpcResponseTypes::OK);
    response->set_message("Success");

    return Status::OK;
}


Status
MgdServer::Get (ServerContext* context, const GetRequest* get_request,
                GetResponse* get_response)
{
    // Log the request
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*get_request, &formatted);
    _logger->log(formatted);

    if (get_request->get_request(0).path() != "openconfig-telemetry") {
        _logger->log("Error in requested path not openconfig-telemetry");
    }

    // Prepare the reply
    get_response->set_request_id(get_request->request_id());
    GetResponse_ResponseList *response = get_response->add_response();
    response->set_operation_id("0");
    response->set_path(get_request->get_request(0).path());

    // Request ID
    size_t request_id = get_request->request_id();

    // Look it up in the db
    ConfigDB::iterator it;
    it = config_db.find(request_id);
    if (it == config_db.end()) {
        _logger->log("No data available for request id : " +
                     std::to_string(request_id));
        response->set_value("");
        response->set_response_code(openconfig::OpenConfigRpcResponseTypes::NOK);
        response->set_message("Fail");
    }
    SetRequest_ConfigOperationList *cmd = config_db[request_id];

    // Fill in the details
    response->set_value(cmd->value());
    response->set_response_code(openconfig::OpenConfigRpcResponseTypes::OK);
    response->set_message("Success");

    return Status::OK;
}