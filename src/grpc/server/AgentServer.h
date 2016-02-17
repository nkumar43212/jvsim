//
//  AgentServer.h
//  agent-jv
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#ifndef AgentServer_h
#define AgentServer_h

#include <iostream>
#include <string>
#include <grpc++/grpc++.h>

#include "agent.grpc.pb.h"
#include "jvision_top.pb.h"
#include "AgentMessageBus.hpp"
#include "AgentServerLog.hpp"
#include "AgentServerIdManager.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using agent::Agent;
using agent::Reply;
using agent::SubscriptionRequest;
using agent::UnSubscribeRequest;
using agent::GetRequest;
using agent::OpenConfigData;
using agent::KeyValue;
using agent::ReturnCode;

class AgentServer final : public Agent::Service {
    // Logging service
    AgentServerLog *_logger;
    
    // Subscription ID Manager
    AgentServerIdManager _id_manager;
    
    // The Interface
    Status telemetrySubscribe(ServerContext *context, const SubscriptionRequest *args, ServerWriter<OpenConfigData>* writer) override;
    Status telemetryUnSubscribe(ServerContext *context, const UnSubscribeRequest *args, Reply *replyp) override;
    Status telemetrySubscriptionsGet(ServerContext *context, const GetRequest *args, OpenConfigData *subscriptions) override;
    Status telemetryOperationalStateGet(ServerContext *context, const GetRequest *args, OpenConfigData *operational_state) override;
    
public:
    AgentServer (AgentServerLog *logger) : _logger(logger) {}
};

#endif /* AgentServer_h */
