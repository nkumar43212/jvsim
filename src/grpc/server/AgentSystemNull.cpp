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

bool
AgentSystemNull::systemAdd (SystemId id, const telemetry::Path *request_path)
{
    return AgentSystem::systemAdd(id, request_path);
}

bool
AgentSystemNull::systemRemove (SystemId id, const telemetry::Path *request_path)
{
    return AgentSystem::systemRemove(id, request_path);
}

telemetry::Path *
AgentSystemNull::systemGet (SystemId sys_id)
{
    return NULL;
}

bool
AgentSystemNull::systemClearAll (void)
{
    return AgentSystem::systemClearAll();
}
