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
#include "mgd_service.pb.h"
#include "mgd_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using openconfig::OpenconfigRpcApi;

using openconfig::EditEphemeralConfigRequest;
using openconfig::EditEphemeralConfigRequest_ConfigOperationList;
using openconfig::EditEphemeralConfigResponse;
using openconfig::EditEphemeralConfigResponse_ResponseList;
using openconfig::EphConfigRequestList;
using openconfig::ExecuteOpCommandRequest;
using openconfig::ExecuteOpCommandResponse;
using openconfig::GetDataEncodingsRequest;
using openconfig::GetDataEncodingsResponse;
using openconfig::GetEphemeralConfigRequest;
using openconfig::GetEphemeralConfigResponse;
using openconfig::GetEphemeralConfigResponse_ResponseList;
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

class AgentSystemProc : public AgentSystem {
public:
    AgentSystemProc (AgentServerLog *logger) : AgentSystem(logger) {}

    // The stub for the RPC
    std::unique_ptr<OpenconfigRpcApi::Stub> stub_;

    // Internal API
    grpc::Status _sendMessagetoMgd(std::string &config,
                                   SystemId id,
                                   openconfig::SetConfigCommands cmdcode);

    void systemAdd(SystemId id, const Telemetry::Path *request_path);
    void systemRemove(SystemId id, const Telemetry::Path *request_path);
    Telemetry::Path * systemGet(SystemId sys_id);
};

#endif /* AgentSystemProc_hpp */