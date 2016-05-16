//
//  MgdServer.hpp
//  Junos MGD
//
//  Created by NITIN KUMAR on 3/22/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef MgdServer_hpp
#define MgdServer_hpp

#include <grpc++/grpc++.h>
#include "AgentServerLog.hpp"

#if defined(__OC_Telemetry_Config__)
    #include "openconfig_mgd.pb.h"
    #include "openconfig_mgd.grpc.pb.h"
#else
    #include "mgd_service.pb.h"
    #include "mgd_service.grpc.pb.h"
#endif
#include "authentication_service.pb.h"
#include "authentication_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

#if defined(__OC_Telemetry_Config__)

using openconfig::OpenconfigRpcApi;

using openconfig::GetDataEncodingsRequest;
using openconfig::GetDataEncodingsResponse;
using openconfig::GetModelsRequest;
using openconfig::GetModelsResponse;
using openconfig::GetRequest;
using openconfig::GetRequestList;
using openconfig::GetResponse;
using openconfig::GetResponse_ResponseList;
using openconfig::Model;
using openconfig::SetDataEncodingRequest;
using openconfig::SetDataEncodingResponse;
using openconfig::SetRequest;
using openconfig::SetRequest_ConfigOperationList;
using openconfig::SetResponse;
using openconfig::SetResponse_ResponseList;

#else

using management::ManagementRpcApi;

using management::EditEphemeralConfigRequest;
using management::EditEphemeralConfigRequest_ConfigOperationList;
using management::EditEphemeralConfigResponse;
using management::EditEphemeralConfigResponse_ResponseList;
using management::EphConfigRequestList;
using management::ExecuteOpCommandRequest;
using management::ExecuteOpCommandResponse;
using management::GetEphemeralConfigRequest;
using management::GetEphemeralConfigResponse;
using management::GetEphemeralConfigResponse_ResponseList;

#endif

using authentication::Login;
using authentication::LoginRequest;
using authentication::LoginReply;

#if defined(__OC_Telemetry_Config__)
class MgdServer final : public OpenconfigRpcApi::Service {
#else
class MgdServer final : public ManagementRpcApi::Service {
#endif

    // Logging service
    AgentServerLog *_logger;

    // The Mgd Interface
#if defined(__OC_Telemetry_Config__)

    Status Set(ServerContext* context, const SetRequest* set_request,
               SetResponse* set_response) override;

    Status Get(ServerContext* context, const GetRequest* get_request,
               GetResponse* get_response) override;

#else

    Status EditEphemeralConfig(ServerContext* context,
               const EditEphemeralConfigRequest* edit_eph_request,
               EditEphemeralConfigResponse* edit_eph_response) override;

    Status GetEphemeralConfig(ServerContext* context,
               const GetEphemeralConfigRequest* get_eph_request,
               GetEphemeralConfigResponse* get_eph_response) override;
#endif

public:
    MgdServer (AgentServerLog *logger) : _logger(logger) {}
};

class LoginServer final : public Login::Service {
    // Logging service
    AgentServerLog *_logger;

    // Login Service
    Status LoginCheck(ServerContext* context,
                      const LoginRequest* request,
                      LoginReply* response) override;

    public:
    LoginServer (AgentServerLog *logger) : _logger(logger) {}
};

#endif /* MgdServer_hpp */