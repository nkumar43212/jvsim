//
//  AgentSystemNull.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemNull.hpp"

void
AgentSystemNull::systemAdd (SystemId id, const agent::Path *request_path)
{
    AgentSystem::systemAdd(id, request_path);
}

void
AgentSystemNull::systemRemove (SystemId id, const agent::Path *request_path)
{
    AgentSystem::systemRemove(id, request_path);
}

agent::Path *
AgentSystemNull::systemGet (SystemId sys_id)
{
    return NULL;
}

