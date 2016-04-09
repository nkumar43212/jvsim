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
#include "AgentServerTransport.hpp"
#include "AgentServerLog.hpp"
#include "AgentServerIdManager.hpp"
#include "AgentSubscription.hpp"
#include "AgentConsolidator.hpp"

class AgentServer final : public Telemetry::OpenConfigTelemetry::Service {
    // Logging service
    AgentServerLog *_logger;
    
    // Subscription ID Manager
    AgentServerIdManager _id_manager;
    
    // Consolidator that consolidates requests into JUNOS
    AgentConsolidator _consolidator;

    // Internal functions
    Status _sendMetaDataInfo(ServerContext* context, ServerWriter<OpenConfigData>* writer, SubscriptionReply& reply);
    void _cleanupSubscription(AgentSubscription *sub);

    // The Interface
    Status telemetrySubscribe(ServerContext* context, const SubscriptionRequest* request, ServerWriter<OpenConfigData>* writer) override;
    Status cancelTelemetrySubscription(ServerContext* context, const CancelSubscriptionRequest* cancel_request,CancelSubscriptionReply* cancel_reply) override;
    Status getTelemetrySubscriptions(ServerContext* context, const GetSubscriptionsRequest* get_request, GetSubscriptionsReply* get_reply) override;
    Status getTelemetryOperationalState(ServerContext* context, const GetOperationalStateRequest* operational_request, GetOperationalStateReply* operational_reply) override;
    Status getDataEncodings(ServerContext* context, const DataEncodingRequest* data_enc_request, DataEncodingReply* data_enc_reply) override;

    // TODO ABBAS delete them later
#if 0
    Status telemetrySubscribe(ServerContext *context, const SubscriptionRequest *args, ServerWriter<OpenConfigData>* writer) override;
    Status telemetryUnSubscribe(ServerContext *context, const UnSubscribeRequest *args, Reply *replyp) override;
    Status telemetrySubscriptionsGet(ServerContext *context, const GetRequest *args, OpenConfigData *subscriptions) override;
    Status telemetryOperationalStateGet(ServerContext *context, const GetRequest *args, OpenConfigData *operational_state) override;
#endif

public:
    AgentServer (AgentServerLog *logger, AgentSystem *sys_handle) : _logger(logger), _consolidator(sys_handle, logger) {}
};

#endif /* AgentServer_h */
