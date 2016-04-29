//
//  AgentServer.h
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#ifndef AgentServer_h
#define AgentServer_h

#include <mutex>
#include <grpc++/grpc++.h>
#include "AgentServerProtos.h"
#include "AgentServerLog.hpp"
#include "AgentServerIdManager.hpp"
#include "AgentSubscription.hpp"
#include "AgentConsolidator.hpp"
#include "AgentSystem.hpp"


class AgentServer final : public Telemetry::OpenConfigTelemetry::Service {
    // Logging service
    AgentServerLog *_logger;

    // Subscription ID Manager
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> _id_manager;

    // Consolidator that consolidates requests into JUNOS
    AgentConsolidator _consolidator;

    // Global lock so that we serialize delete operations due to client termination or cancelSubscription
    std::mutex    _delete_initiate_mutex;

    // Internal functions
    Status _sendMetaDataInfo(ServerContext* context,
                             ServerWriter<OpenConfigData>* writer,
                             SubscriptionReply& reply);
    void _cleanupSubscription(AgentSubscription *sub);

    // The Interface
    Status telemetrySubscribe(ServerContext* context,
                              const SubscriptionRequest* request,
                              ServerWriter<OpenConfigData>* writer) override;
    Status cancelTelemetrySubscription(ServerContext* context,
                              const CancelSubscriptionRequest* cancel_request,
                              CancelSubscriptionReply* cancel_reply) override;
    Status getTelemetrySubscriptions(ServerContext* context,
                              const GetSubscriptionsRequest* get_request,
                              GetSubscriptionsReply* get_reply) override;
    Status getTelemetryOperationalState(ServerContext* context,
                        const GetOperationalStateRequest* operational_request,
                        GetOperationalStateReply* operational_reply) override;
    Status getDataEncodings(ServerContext* context,
                        const DataEncodingRequest* data_enc_request,
                        DataEncodingReply* data_enc_reply) override;

public:
    AgentServer (AgentServerLog *logger, AgentSystem *sys_handle) :
                 _logger(logger),
                 _consolidator(sys_handle, logger) {}
};

#endif /* AgentServer_h */