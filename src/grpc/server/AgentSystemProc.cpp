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
#include "JunosTelemetryJsonGenerator.hpp"
#include "JunosTelemetryJson.hpp"
#include "JsonUtils.hpp"
#include "GlobalConfig.hpp"

// Set connection timeout to MGD server to X sec for a RPC API call
#define CLIENT_CONN_TIMEOUT         5


#if defined(__OC_Telemetry_Config__)
grpc::Status
AgentSystemProc::_sendOCMessagetoMgd (std::string &config,
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

    // Check the status
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

#else

grpc::Status
AgentSystemProc::_authenticateChannel (std::shared_ptr<grpc::Channel> &channel)
{
    _logger->log("Requesting login authentication from device");

    auth_stub_ = Login::NewStub(channel);

    LoginRequest login_request;
    LoginReply login_reply;
    grpc::ClientContext login_context;

    // Fill in login details
    login_request.set_user_name(global_config.device_user_name);
    login_request.set_password(global_config.device_password);
    login_request.set_client_id("Telemetry-service-client");

    // Set authentication timeout
    std::chrono::system_clock::time_point deadline =
                                    std::chrono::system_clock::now() +
                                    std::chrono::seconds(CLIENT_CONN_TIMEOUT);
    login_context.set_deadline(deadline);

    // Issue LoginCheck API
    grpc::Status status = auth_stub_->LoginCheck(&login_context,
                                                 login_request,
                                                 &login_reply);
    if (status.ok() && login_reply.result()) {
        _logger->log("Login Authentication Successful");
    } else {
        ++_authentication_failure;
        _logger->log("Login Authentication Failed");
        auth_stub_ = NULL;
        return status.CANCELLED;
    }
    // Clear the auth_stub_
    auth_stub_ = NULL;
    return status;
}

grpc::Status
AgentSystemProc::_sendJunosMessagetoMgd (std::string &config,
                                         SystemId id,
                                         management::ConfigCommands cmdcode)
{
    // Set mgd address
    std::string mgd_address;
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        mgd_address = global_config.device_mgd_ip + ":" +
                      std::to_string(global_config.device_mgd_port);
    } else {
        mgd_address = "unix:" + global_config.device_mgd_unix_socket;
    }
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
                                                   mgd_address,
                                                   grpc::InsecureCredentials());

    // Off-box mode requires login authentication
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        // Authenticate the channel
        grpc::Status status = _authenticateChannel(channel);
        if (status.ok() == false) {
            return status;
        }
    }

    // Now issue the config command
    stub_ = ManagementRpcApi::NewStub(channel);

    // Form the mgd request
    EditEphemeralConfigRequest edit_eph_request;

    // Request Header
    edit_eph_request.set_request_id(id.getId());
    edit_eph_request.set_encoding(
                     management::JunosDataEncodingTypes::ENCODING_JSON);
    // edit_eph_request.set_eph_instance_name();

    // Create the command
    EditEphemeralConfigRequest_ConfigOperationList *cmd =
                            edit_eph_request.add_eph_config_operations();
    cmd->set_operation_id("0");
    cmd->set_operation(cmdcode);
    cmd->set_path("/");
    cmd->set_value(config);

    // Send over the request
    EditEphemeralConfigResponse edit_eph_response;
    grpc::ClientContext context;
    std::chrono::system_clock::time_point deadline =
                        std::chrono::system_clock::now() +
                        std::chrono::seconds(CLIENT_CONN_TIMEOUT);
    context.set_deadline(deadline);
    grpc::Status status = stub_->EditEphemeralConfig(&context,
                                                     edit_eph_request,
                                                     &edit_eph_response);
    // Check time exceeded (device is not available)
    if (status.ok() == false) {
        // Increment count
        ++_error_system_count;

        _logger->log("MGD command failed (cmd = " +
                     std::to_string(cmdcode) + "). Error code: " +
                     std::to_string(status.error_code()) + ". " +
                     "Error message: " + status.error_message());

        stub_ = NULL;
        return status;
    }

    // Make sure we get atleast one element in response
    if (edit_eph_response.response_size() == 0) {
        // Increment count
        ++_error_system_count;

        _logger->log("MGD command failed (cmd = " +
                     std::to_string(cmdcode) + "). No response received.");

        // Indicate failed status
        status = grpc::Status(grpc::StatusCode::UNKNOWN, "");

        stub_ = NULL;
        return status;
    }

    // Get the response - only one request is send
    EditEphemeralConfigResponse_ResponseList response =
                                                edit_eph_response.response(0);

    // Check the status
    if (response.response_code() != management::JunosRpcResponseTypes::OK) {
            // Increment count
            ++_error_system_count;

            _logger->log("MGD command failed (cmd = " +
                         std::to_string(cmdcode) + "). Error code: " +
                         std::to_string(response.response_code()) + ". " +
                         "Error message: " + response.message());

            // Indicate failed status
            status = grpc::Status(grpc::StatusCode::UNKNOWN, "");

        stub_ = NULL;
        return status;
    }

    // Clear the stub_
    stub_ = NULL;
    return status;
}

#endif

void
AgentSystemProc::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
#if defined(__OC_Telemetry_Config__)
    // OpenConfig Config
    bool mqtt = true;
    std::string config;
    config = OCTelemetryJsonGenerator::generate_json_oc_config(mqtt,
                                                    (id_idx_t)id.getId(),
                                                    request_path);
    Status status = _sendOCMessagetoMgd(config, id,
                            openconfig::SetConfigCommands::UPDATE_CONFIG);
    if (status.ok()) {
        // Increment the counter
        ++_add_system_count;

        _logger->log("MGD command UPDATE_CONFIG passed. Request_id: " +
                     std::to_string(id.getId()) +
                     " Path: " + AgentUtils::getMessageString(*request_path));
    }
#else
    // Junos Config
    std::string config;
    bool add = true;
    bool mqtt = true;
    config = JunosTelemetryJsonGenerator::generate_json_junos_config(add,
                                    mqtt,
                                    (id_idx_t)id.getId(),
                                    request_path);
    Status status = _sendJunosMessagetoMgd(config, id,
                        management::ConfigCommands::UPDATE_CONFIG);
    if (status.ok()) {
        // Increment the counter
        ++_add_system_count;

        _logger->log("MGD command UPDATE_CONFIG passed. Request_id: " +
                     std::to_string(id.getId()) +
                     " Path: " + AgentUtils::getMessageString(*request_path));
    }
#endif
}

void
AgentSystemProc::systemRemove (SystemId id, const Telemetry::Path *request_path)
{
#if defined(__OC_Telemetry_Config__)
    // OpenConfig Config
    bool mqtt = true;
    std::string config;
    config = OCTelemetryJsonGenerator::generate_json_oc_config(mqtt,
                                    (id_idx_t)id.getId(),
                                    request_path);
    Status status = _sendOCMessagetoMgd(config, id,
                           openconfig::SetConfigCommands::DELETE_CONFIG);
    if (status.ok()) {
        // Increment the counter
        ++_remove_system_count;

        _logger->log("MGD command DELETE_CONFIG passed. Request_id: " +
                     std::to_string(id.getId()) +
                     " Path: " + AgentUtils::getMessageString(*request_path));
    }
#else
    // Junos Config
    std::string config;
    bool add = false;
    bool mqtt = true;
    config = JunosTelemetryJsonGenerator::generate_json_junos_config(add,
                                    mqtt,
                                    (id_idx_t)id.getId(),
                                    request_path);
    Status status = _sendJunosMessagetoMgd(config, id,
                        management::ConfigCommands::DELETE_CONFIG);
    if (status.ok()) {
        // Increment the counter
        ++_add_system_count;

        _logger->log("MGD command DELETE_CONFIG passed. Request_id: " +
                     std::to_string(id.getId()) +
                     " Path: " + AgentUtils::getMessageString(*request_path));
    }
#endif
}

Telemetry::Path *
AgentSystemProc::systemGet (SystemId id)
{
    // Set mgd address
    std::string mgd_address;
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        mgd_address = global_config.device_mgd_ip + ":" +
        std::to_string(global_config.device_mgd_port);
    } else {
        mgd_address = "unix:" + global_config.device_mgd_unix_socket;
    }
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
                                                mgd_address,
                                                grpc::InsecureCredentials());
    stub_ = ManagementRpcApi::NewStub(channel);

#if defined(__OC_Telemetry_Config__)
    // OpenConfig Config
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
#else
    // Off-box mode requires login authentication
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        // Authenticate the channel
        grpc::Status status = _authenticateChannel(channel);
        if (status.ok() == false) {
            return NULL;
        }
    }

    // Junos Config
    // Form the request
    GetEphemeralConfigRequest   get_eph_request;
    get_eph_request.set_request_id(id.getId());
    get_eph_request.set_encoding(
                    management::JunosDataEncodingTypes::ENCODING_JSON);
    // get_eph_request.set_eph_instance_name();
    get_eph_request.set_merge_view(true);
    EphConfigRequestList *request = get_eph_request.add_eph_config_requests();
    request->set_operation_id("0");
    request->set_path("/");

    // Send over the request
    GetEphemeralConfigResponse  get_eph_response;
    grpc::ClientContext context;
    stub_->GetEphemeralConfig(&context, get_eph_request, &get_eph_response);

    // Did the reply have the same ID ?
    if (get_eph_response.request_id() != id.getId()) {
        return NULL;
    }

    // Is there a path
    if (get_eph_response.response(0).path() != "/") {
        return NULL;
    }

    std::string response_json_str = get_eph_response.response(0).value();
    _logger->log("Get response : " + response_json_str);

    // Remove the special tags
    AgentUtils::SearchNReplaceString(response_json_str,
                                     "<configuration-json>", "");
    AgentUtils::SearchNReplaceString(response_json_str,
                                     "</configuration-json>", "");

    // Convert to path
    // TODO ABBAS - FIX THIS HOLISTICALLY LATER
    Telemetry::Path *path = new Telemetry::Path();
    Json::Value json_obj;
    JsonUtils::parse_string_to_json_obj(response_json_str,
                                        json_obj);
    std::string path_str = json_obj["configuration"]["services"]["analytics"]
                                   ["sensor"][0]["resource"].asCString();
    path->set_path(path_str);
    std::string sample_frequency_str = json_obj["configuration"]["services"]
                                               ["analytics"]["sensor"][0]
                                               ["reporting-rate"].asCString();
    path->set_sample_frequency(std::stoi(sample_frequency_str));
    return path;
#endif
}