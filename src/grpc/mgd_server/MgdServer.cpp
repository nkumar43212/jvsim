//
//  MgdServer.cpp
//  Junos MGD
//
//  Created by NITIN KUMAR on 3/22/16.
//  CoAuthor: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "MgdServer.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <map>

// Configuration Database
// The db of all requests into the system

#if defined(__OC_Telemetry_Config__)
// 1. Normal db
typedef std::map<std::size_t, SetRequest_ConfigOperationList *> ConfigDB;
ConfigDB config_db;

#else
// 2. Ephemeral db
typedef std::map<std::size_t, EditEphemeralConfigRequest_ConfigOperationList *>
        EphConfigDB;
EphConfigDB eph_config_db;
#endif

#if defined(__OC_Telemetry_Config__)
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

#else

Status
MgdServer::EditEphemeralConfig (ServerContext* context,
                           const EditEphemeralConfigRequest* edit_eph_request,
                           EditEphemeralConfigResponse* edit_eph_response)
{
    // Log the request
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*edit_eph_request, &formatted);
    _logger->log(formatted);

    // Store it locally, as we will get asked about it
    uint64_t request_id = edit_eph_request->request_id();

    EditEphemeralConfigRequest_ConfigOperationList *cmd =
                            new EditEphemeralConfigRequest_ConfigOperationList;
    cmd->CopyFrom(edit_eph_request->eph_config_operations(0));

    if ((cmd->operation() == management::ConfigCommands::UPDATE_CONFIG) ||
        (cmd->operation() == management::ConfigCommands::REPLACE_CONFIG)) {
        // add
        eph_config_db[request_id] = cmd;
    } else {
        // delete
        eph_config_db.erase(request_id);
    }

    edit_eph_response->set_request_id(request_id);
    EditEphemeralConfigResponse_ResponseList *response =
                                            edit_eph_response->add_response();
    response->set_operation_id(cmd->operation_id());
    response->set_response_code(management::JunosRpcResponseTypes::OK);
    response->set_message("Success");

    return Status::OK;
}

Status
MgdServer::GetEphemeralConfig(ServerContext* context,
                          const GetEphemeralConfigRequest* get_eph_request,
                          GetEphemeralConfigResponse* get_eph_response)
{
    // Log the request
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*get_eph_request, &formatted);
    _logger->log(formatted);

    if (get_eph_request->eph_config_requests(0).path() != "/") {
        _logger->log("Error in requested path not /");
    }

    // Prepare the reply
    get_eph_response->set_request_id(get_eph_request->request_id());
    GetEphemeralConfigResponse_ResponseList *response =
                                            get_eph_response->add_response();
    response->set_operation_id("0");
    response->set_path(get_eph_request->eph_config_requests(0).path());

    // Request ID
    size_t request_id = get_eph_request->request_id();

    // Look it up in the db
    EphConfigDB::iterator it;
    it = eph_config_db.find(request_id);
    if (it == eph_config_db.end()) {
        _logger->log("No data available for request id : " +
                     std::to_string(request_id));
        response->set_value("");
        response->set_response_code(management::JunosRpcResponseTypes::NOK);
        response->set_message("Fail");
    }
    EditEphemeralConfigRequest_ConfigOperationList *cmd =
                                                    eph_config_db[request_id];

    // Fill in the details
    response->set_value(cmd->value());
    response->set_response_code(management::JunosRpcResponseTypes::OK);
    response->set_message("Success");

    return Status::OK;
}

#endif


Status
LoginServer::LoginCheck (ServerContext* context,
                         const LoginRequest* request, LoginReply* response)
{
    // Log the event
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*request, &formatted);
    _logger->log(formatted);

    if (request->user_name() == "regress" && request->password() == "MaRtInI") {
        response->set_result(true);
        return Status::OK;
    } else {
        response->set_result(false);
        return Status::CANCELLED;
    }
}

Status
RegisterServer::RegisterService (ServerContext* context,
                                 const RegisterRequest* request,
                                 RegisterReply* response)
{
    // Log the event
    std::string formatted;
    google::protobuf::TextFormat::PrintToString(*request, &formatted);
    _logger->log(formatted);

    // Register id not specified
    if (request->register_id().length() == 0) {
        response->set_result(false);
        response->set_error("Missing register_id");
        return Status::OK;
    }

    // Target not specified
    if (request->target().length() == 0) {
        response->set_result(false);
        response->set_error("Missing target");
        return Status::OK;
    }

    // Input type not specified
    switch (request->input_type_case()) {
        case registration::RegisterRequest::kFileInput:
            if (request->file_input().length() == 0) {
                response->set_result(false);
                response->set_error("File input cannot be empty");
                return Status::OK;
            }
            break;
        case registration::RegisterRequest::kJsonInput:
            if (request->json_input().length() == 0) {
                response->set_result(false);
                response->set_error("Json input cannot be empty");
                return Status::OK;
            }
            break;
        default:
            _logger->log("Cannot come here");
            break;
    }

    // return sucess
    response->set_result(true);
    return Status::OK;
}
