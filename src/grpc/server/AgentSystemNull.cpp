//
//  AgentSystemNull.cpp
//  Telemetry Agent
//
//  Created: 2/27/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemNull.hpp"

void
AgentSystemNull::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
    AgentSystem::systemAdd(id, request_path);
}

void
AgentSystemNull::systemRemove (SystemId id, const Telemetry::Path *request_path)
{
    AgentSystem::systemRemove(id, request_path);
}

Telemetry::Path *
AgentSystemNull::systemGet (SystemId sys_id)
{
    return NULL;
}
