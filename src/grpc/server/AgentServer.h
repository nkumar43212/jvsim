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

#include "AgentServerProtos.h"
#include "AgentMessageBus.hpp"
#include "AgentServerLog.hpp"
#include "AgentServerIdManager.hpp"
#include "AgentConsolidator.hpp"

class AgentServer final : public Agent::Service {
    // Logging service
    AgentServerLog *_logger;
    
    // Subscription ID Manager
    AgentServerIdManager _id_manager;
    
    // Consolidator that consolidates requests into JUNOS
    AgentConsolidator _consolidator;
    
    // The Interface
    Status telemetrySubscribe(ServerContext *context, const SubscriptionRequest *args, ServerWriter<OpenConfigData>* writer) override;
    Status telemetryUnSubscribe(ServerContext *context, const UnSubscribeRequest *args, Reply *replyp) override;
    Status telemetrySubscriptionsGet(ServerContext *context, const GetRequest *args, OpenConfigData *subscriptions) override;
    Status telemetryOperationalStateGet(ServerContext *context, const GetRequest *args, OpenConfigData *operational_state) override;
    
public:
    AgentServer (AgentServerLog *logger, AgentSystem *sys_handle) : _logger(logger), _consolidator(sys_handle, logger) {}
};

#endif /* AgentServer_h */
