//
//  AgentSystemProc.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <grpc++/grpc++.h>
#include "AgentSystemProc.hpp"
#include "AgentUtils.hpp"
#include "OCTelemetryJsonGenerator.hpp"
#include "OCTelemetryJson.hpp"
#include "JsonUtils.hpp"
#include "GlobalConfig.hpp"

grpc::Status
AgentSystemProc::_sendMessagetoMgd (std::string &config,
                                    SystemId id,
                                    openconfig::SetConfigCommands cmdcode)
{
    // Create a client
    std::string mgd_address(global_config.device_mgd_ip + ":" +
                            std::to_string(global_config.device_mgd_port));
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
                                                 mgd_address,
                                                 grpc::InsecureCredentials());
    stub_ = OpenconfigRpcApi::NewStub(channel);

    // Form the mgd request
    SetRequest   set_request;

    // Request Header
    set_request.set_request_id(id.getId());
    set_request.set_transaction(true);
    set_request.set_encoding(
                openconfig::OpenConfigDataEncodingTypes::ENCODING_JSON);

    // Create the command
    SetRequest_ConfigOperationList *cmd = set_request.add_config_operation();
    cmd->set_operation_id("0");
    cmd->set_operation(cmdcode);
    cmd->set_path("openconfig-telemetry");
    cmd->set_value(config);

    // Send over the request
    SetResponse set_response;
    grpc::ClientContext context;
    grpc::Status status = stub_->Set(&context, set_request, &set_response);

    // Get the response - only one request is send
    SetResponse_ResponseList response = set_response.response(0);

    if ((status.ok() == false) ||
        (response.response_code() !=
                             openconfig::OpenConfigRpcResponseTypes::OK)) {
        // Increment count
        ++_error_system_count;

        _logger->log("MGD command failed (cmd = " +
                     std::to_string(cmdcode) + "). Error code: " +
                     std::to_string(status.error_code()) +
                     " " + std::to_string(response.response_code()) +
                     "Error message: " + status.error_message() +
                     response.message());

        // Indicate failed status
        status = grpc::Status(grpc::StatusCode::UNKNOWN, "");
    }

    return status;
}

void
AgentSystemProc::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
    std::string config = OCTelemetryJsonGenerator::generate_json_oc_config(
                                    true,
                                    (id_idx_t)id.getId(),
                                    request_path);
    Status status = _sendMessagetoMgd(config, id,
                           openconfig::SetConfigCommands::UPDATE_CONFIG);
    if (status.ok()) {
        // Increment the counter
        ++_add_system_count;

        _logger->log("MGD command UPDATE_CONFIG passed. Request_id: " +
                     std::to_string(id.getId()) +
                     " Path: " + AgentUtils::getMessageString(*request_path));
    }
}

void
AgentSystemProc::systemRemove (SystemId id, const Telemetry::Path *request_path)
{
    std::string config = OCTelemetryJsonGenerator::generate_json_oc_config(
                                    true,
                                    (id_idx_t)id.getId(),
                                    request_path);
    Status status = _sendMessagetoMgd(config, id,
                           openconfig::SetConfigCommands::DELETE_CONFIG);
    if (status.ok()) {
        // Increment the counter
        ++_remove_system_count;

        _logger->log("MGD command DELETE_CONFIG passed. Request_id: " +
                     std::to_string(id.getId()) +
                     " Path: " + AgentUtils::getMessageString(*request_path));
    }
}

Telemetry::Path *
AgentSystemProc::systemGet (SystemId id)
{
    // Create a client
    std::string mgd_address(global_config.device_mgd_ip + ":" +
                            std::to_string(global_config.device_mgd_port));
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
                                                mgd_address,
                                                grpc::InsecureCredentials());
    stub_ = OpenconfigRpcApi::NewStub(channel);

    // Form the request
    GetRequest   get_request;
    get_request.set_request_id(id.getId());
    get_request.set_encoding(
                openconfig::OpenConfigDataEncodingTypes::ENCODING_JSON);
    GetRequestList *request = get_request.add_get_request();
    request->set_operation_id("0");
    request->set_operation(openconfig::GetDataCommands::GET_CONFIG);
    request->set_path("openconfig-telemetry");
    
    // Send over the request
    GetResponse  get_response;
    grpc::ClientContext context;
    stub_->Get(&context, get_request, &get_response);

    // Did the reply have the same ID ?
    if (get_response.request_id() != id.getId()) {
        return NULL;
    }

    // Is there a path
    if (get_response.response(0).path() != "openconfig-telemetry") {
        return NULL;
    }

    std::string response_json_str = get_response.response(0).value();
    _logger->log("Get response : " + response_json_str);

    // Convert to path
    // TODO ABBAS - FIX THIS HOLISTICALLY LATER
    Telemetry::Path *path = new Telemetry::Path();
    Json::Value json_obj;
    JsonUtils::parse_string_to_json_obj(response_json_str,
                                        json_obj);
    std::string path_str = json_obj["sensor-groups"]["sensor-paths"][0]
                                ["sensor-path"]["config"]["path"].asCString();
    path->set_path(path_str);
    std::string sample_frequency_str = json_obj["subscriptions"]["persistent"]
            ["subscription"]["sensor-profiles"]["sensor-profile"]["config"]
            ["sample-interval"].asCString();
    path->set_sample_frequency(std::stoi(sample_frequency_str));
    return path;
}