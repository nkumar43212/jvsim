//
//  AgentServerProtos.h
//  Telemetry Agent
//
//  Created: 2/19/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
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

using telemetry::OpenConfigTelemetry;

using telemetry::CancelSubscriptionReply;
using telemetry::CancelSubscriptionRequest;
using telemetry::Collector;
using telemetry::DataEncodingReply;
using telemetry::DataEncodingRequest;
using telemetry::GetOperationalStateReply;
using telemetry::GetOperationalStateRequest;
using telemetry::GetSubscriptionsReply;
using telemetry::GetSubscriptionsRequest;
using telemetry::KeyValue;
using telemetry::OpenConfigData;
using telemetry::Path;
using telemetry::SubscriptionAdditionalConfig;
using telemetry::SubscriptionInput;
using telemetry::SubscriptionReply;
using telemetry::SubscriptionRequest;
using telemetry::SubscriptionResponse;
using telemetry::ReturnCode;
using telemetry::VerbosityLevel;
using telemetry::EncodingType;

#endif /* AgentServerProtos_h */
