//
//  MgdServer.hpp
//  grpc
//
//  Created by NITIN KUMAR on 3/22/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef MgdServer_hpp
#define MgdServer_hpp

#include <stdio.h>
#include <grpc++/grpc++.h>
#include "junos_mgd.pb.h"
#include "junos_mgd.grpc.pb.h"
#include "AgentServerLog.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using junos_mgd::OpenConfigRpcApi;
using junos_mgd::EditConfigRequest;
using junos_mgd::RequestHdr;
using junos_mgd::ReplyHdr;
using junos_mgd::ConfigCommand;
using junos_mgd::OpenConfigDataElement;
using junos_mgd::EditConfigCommands;
using junos_mgd::EditConfigResponse;
using junos_mgd::GetConfigRequest;
using junos_mgd::GetConfigResponse;

class MgdServer final : public OpenConfigRpcApi::Service {
    // Logging service
    AgentServerLog *_logger;
    
    // The Mgd Interface
    Status EditConfig(ServerContext *context, const EditConfigRequest *args, EditConfigResponse * response) override;
    Status GetConfig(ServerContext *context, const GetConfigRequest *args, GetConfigResponse * response) override;
    
public:
    MgdServer (AgentServerLog *logger) : _logger(logger) {}
};

#endif /* MgdServer_hpp */
