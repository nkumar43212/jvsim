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
#include "mgd_service.pb.h"
#include "mgd_service.grpc.pb.h"
#include "AgentServerLog.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
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

class MgdServer final : public OpenconfigRpcApi::Service {
    // Logging service
    AgentServerLog *_logger;

    // The Mgd Interface
    Status Set(ServerContext* context, const SetRequest* set_request,
               SetResponse* set_response) override;

    Status Get(ServerContext* context, const GetRequest* get_request,
               GetResponse* get_response) override;

public:
    MgdServer (AgentServerLog *logger) : _logger(logger) {}
};

#endif /* MgdServer_hpp */