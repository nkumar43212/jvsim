//
//  AgentSystemProc.hpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemProc_hpp
#define AgentSystemProc_hpp

#include "AgentSystem.hpp"
#include <grpc++/grpc++.h>

#if defined(__OC_Telemetry_Config__)
    #include "openconfig_mgd.pb.h"
    #include "openconfig_mgd.grpc.pb.h"
#else
    #include "mgd_service.pb.h"
    #include "mgd_service.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
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

class AgentSystemProc : public AgentSystem {
public:
    AgentSystemProc (AgentServerLog *logger) : AgentSystem(logger) {}

    // The stub for the RPC
#if defined(__OC_Telemetry_Config__)
    std::unique_ptr<OpenconfigRpcApi::Stub> stub_;
    grpc::Status _sendOCMessagetoMgd(std::string &config,
                                     SystemId id,
                                     openconfig::SetConfigCommands cmdcode);
#else
    std::unique_ptr<ManagementRpcApi::Stub> stub_;
    grpc::Status _sendJunosMessagetoMgd(std::string &config,
                                        SystemId id,
                                        management::ConfigCommands cmdcode);
#endif

    void systemAdd(SystemId id, const Telemetry::Path *request_path);
    void systemRemove(SystemId id, const Telemetry::Path *request_path);
    Telemetry::Path * systemGet(SystemId sys_id);
};

#endif /* AgentSystemProc_hpp */