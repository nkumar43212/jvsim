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
#include "jvision_top.pb.h"

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


#endif /* AgentServerProtos_h */
