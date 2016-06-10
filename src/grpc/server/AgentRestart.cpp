//
//  AgentRestart.cpp
//  Telemetry Agent
//
//  Created: 6/9/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <cmath>
#include <grpc++/grpc++.h>
#include "json/json.h"
#include "authentication_service.pb.h"
#include "authentication_service.grpc.pb.h"
#include "registration_service.pb.h"
#include "registration_service.grpc.pb.h"
#include "GlobalConfig.hpp"
#include "JsonUtils.hpp"
#include "AgentRestart.hpp"

using authentication::Login;

using authentication::LoginRequest;
using authentication::LoginReply;

using registration::Register;

using registration::RegisterRequest;
using registration::RegisterReply;

// Set connection timeout to JSD server to X sec for a RPC API call
#define CLIENT_CONN_TIMEOUT         5

bool
_read_json_from_file (std::string filename,
                      Json::Value &content_json)
{
    // Open file and read the file
    std::ifstream file(filename);
    std::stringstream tmp_buffer;
    tmp_buffer << file.rdbuf();
    std::string tmp_str = tmp_buffer.str();

    Json::Reader reader;
    bool parsingSuccessful = reader.parse(tmp_str, content_json);
    if (!parsingSuccessful) {
        return false;
    }
    return true;
}

grpc::Status
_authenticateChannel (AgentServerLog *_logger,
                      std::shared_ptr<grpc::Channel> &channel)
{
    std::unique_ptr<Login::Stub> auth_stub_;

    _logger->log("Requesting login authentication from device");
    auth_stub_ = Login::NewStub(channel);

    LoginRequest login_request;
    LoginReply login_reply;
    grpc::ClientContext login_context;

    // Fill in login details
    login_request.set_user_name(global_config.device_user_name);
    login_request.set_password(global_config.device_password);
    login_request.set_client_id(INI_SECTION_TELEMETRY_GRPC);

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
        _logger->log("Login Authentication Failed");
        auth_stub_ = NULL;
        return status.CANCELLED;
    }
    // Clear the auth_stub_
    auth_stub_ = NULL;
    return status;
}

bool
_jsd_register (AgentServerLog *_logger, Json::Value &registration_obj)
{
    // Set mgd address
    std::string jsd_address;
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        // Hardcode the jsd address
        // TODO ABBAS --- revisit if needed
        // use mgd port for now ???
        jsd_address = global_config.device_mgd_ip + ":" +
                      std::to_string(global_config.device_mgd_port);
    } else {
        // Hardcode the jsd address
        // TODO ABBAS --- revisit if needed
        // both in-box and off-box uses this mechanism for now
        jsd_address = global_config.device_mgd_ip + ":" +
                      std::to_string(global_config.device_mgd_port);;
    }
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
                                                jsd_address,
                                                grpc::InsecureCredentials());

    // Off-box mode requires login authentication
    // Check this for on-box --- TODO ABBAS
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        // Authenticate the channel
        grpc::Status status = _authenticateChannel(_logger, channel);
        if (status.ok() == false) {
            return false;
        }
    }

    // Now issue the jsd registration command
    std::unique_ptr<Register::Stub> register_stub_;
    register_stub_ = Register::NewStub(channel);

    // Form the request
    RegisterRequest reg_request;

    // Request Data
    std::string registration_json_str =
                        JsonUtils::write_json_obj_to_string(registration_obj);
    reg_request.set_json_input(registration_json_str);
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        reg_request.set_target(global_config.grpc_server_ip + ":" +
                               std::to_string(global_config.grpc_server_port));
    } else {
        reg_request.set_target("unix:" + global_config.grpc_server_unix_socket);
    }
    reg_request.set_register_id("na-grpcd");

    // Send over the request
    RegisterReply reg_reply;
    grpc::ClientContext context;
    std::chrono::system_clock::time_point deadline =
                        std::chrono::system_clock::now() +
                        std::chrono::seconds(CLIENT_CONN_TIMEOUT);
    context.set_deadline(deadline);
    grpc::Status status = register_stub_->RegisterService(&context,
                                                          reg_request,
                                                          &reg_reply);
    // Check time exceeded (device is not available)
    if (status.ok() == false) {
        _logger->log("Could not register with JSD. Error code: " +
                     std::to_string(status.error_code()) + ". " +
                     "Error message: " + status.error_message());

        register_stub_ = NULL;
        return false;
    }

    // Make sure we get atleast one element in response
    if (!reg_reply.result()) {
        _logger->log("JSD replied with error. Error code: " +
                     reg_reply.error());

        register_stub_ = NULL;
        return false;
    }

    // Clear the stub_
    register_stub_ = NULL;
    return true;
}

bool
on_startup (AgentServerLog *logger, AgentSystem *sys_handle)
{
    bool result;
    std::string log_str;

    // 1. Clear the Ephemeral DB --- no retry logic
    logger->log("Issue systemCleanAll");
    result = sys_handle->systemClearAll();
    log_str = "systemClearAll: ";
    log_str.append(result?"Success":"Fail");
    logger->log(log_str);
    if (!result) {
        return false;
    }

    // 2. Load the protoc-thrift compiled json file and register with JSD
    if (global_config.jsd_init_registration) {
        std::string filename = global_config.jsd_json_file_path + "/" +
                               global_config.jsd_json_file_name;
        Json::Value registration_content_json;

        // read json file
        result = _read_json_from_file(filename, registration_content_json);
        log_str = "Registration json file parsing ";
        log_str.append(result?"Success":"Fail");
        logger->log(log_str);
        if (!result) {
            return false;
        }

        // Register with JSD
        int retry = 1;
        while (retry <= global_config.jsd_num_of_retries) {
            result = _jsd_register(logger, registration_content_json);

            if (result == false) {
                logger->log("Failed to register with JSD - Attempt " +
                            std::to_string(retry));
            } else {
                logger->log("Successful to register with JSD - Attempt " +
                            std::to_string(retry));
                break;
            }
            retry++;
        }
        if (retry > global_config.jsd_num_of_retries) {
            logger->log("Could not register with JSD");
            return false;
        }
    }

    return true;
}
