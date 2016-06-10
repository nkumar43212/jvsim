//
//  AgentRestart.cpp
//  Telemetry Agent
//
//  Created: 6/9/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentRestart.hpp"

#include <grpc++/grpc++.h>
#include "json/json.h"
#include "GlobalConfig.hpp"
#include "authentication_service.pb.h"
#include "authentication_service.grpc.pb.h"
#include "registration_service.pb.h"
#include "registration_service.grpc.pb.h"


bool
on_startup (AgentServerLog *logger, AgentSystem *sys_handle)
{
    bool result;
    std::string log_str;

    // 1. Clear the Ephemeral DB --- no retry logic
    logger->log("Issue systemCleanAll");
    result = sys_handle->systemClearAll();
    log_str = "systemClearAll: ";
    log_str.append((result?"Success":"Fail"));
    logger->log(log_str);
    if (!result) {
        exit(0);
    }

    // 2. Load the protoc-thrift compiled json file and register with JSD


    return true;
}
