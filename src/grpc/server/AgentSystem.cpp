//
//  AgentSystem.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/21/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystem.hpp"
#include "AgentUtils.hpp"

void
AgentSystem::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
    // Operation
    std::string log_str = "SystemAdd: ";

    // The gist of the request
    // Convert request to string
    std::string request_path_str = AgentUtils::getMessageString(*request_path);

    // Write it
    _logger->log(log_str + request_path_str);

    // One more
    _add_system_count++;
}

void
AgentSystem::systemRemove (SystemId id, const Telemetry::Path *request_path)
{
    // Operation
    std::string log_str = "SystemRemove: ";

    // The gist of the request
    // Convert request to string
    std::string request_path_str = AgentUtils::getMessageString(*request_path);

    // Write it
    _logger->log(log_str + request_path_str);

    // One more
    _remove_system_count++;
}