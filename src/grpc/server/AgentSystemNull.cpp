//
//  AgentSystemNull.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemNull.hpp"

void
AgentSystemNull::systemAdd (const agent::Path *request_path)
{
    AgentSystem::systemAdd(request_path);
}

void
AgentSystemNull::systemRemove (const agent::Path *request_path)
{
    AgentSystem::systemRemove(request_path);
}