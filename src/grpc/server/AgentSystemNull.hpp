//
//  AgentSystemNull.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemNull_hpp
#define AgentSystemNull_hpp

#include "AgentSystem.hpp"
#include <stdio.h>

class AgentSystemNull : public AgentSystem {
public:
    AgentSystemNull (AgentServerLog *logger) : AgentSystem(logger)
    {
    }
    void systemAdd(SystemId id, const agent::Path *request_path);
    void systemRemove(SystemId id, const agent::Path *request_path);
    agent::Path * systemGet(SystemId sys_id);
};



#endif /* AgentSystemNull_hpp */
