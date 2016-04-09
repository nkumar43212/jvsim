//
//  AgentServerProtos.h
//  grpc
//
//  Created by NITIN KUMAR on 2/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentServerProtos_h
#define AgentServerProtos_h

#include "agent.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using Telemetry::OpenConfigTelemetry;

using Telemetry::CancelSubscriptionReply;
using Telemetry::CancelSubscriptionRequest;
using Telemetry::Collector;
using Telemetry::DataEncodingReply;
using Telemetry::DataEncodingRequest;
using Telemetry::GetOperationalStateReply;
using Telemetry::GetOperationalStateRequest;
using Telemetry::GetSubscriptionsReply;
using Telemetry::GetSubscriptionsRequest;
using Telemetry::KeyValue;
using Telemetry::OpenConfigData;
using Telemetry::Path;
using Telemetry::SubscriptionAdditionalConfig;
using Telemetry::SubscriptionInput;
using Telemetry::SubscriptionReply;
using Telemetry::SubscriptionRequest;
using Telemetry::SubscriptionResponse;
using Telemetry::ReturnCode;
using Telemetry::VerbosityLevel;
using Telemetry::EncodingType;

#endif /* AgentServerProtos_h */
