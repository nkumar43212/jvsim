//
//  AgentSystemProc.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemProc_hpp
#define AgentSystemProc_hpp

#include <stdio.h>
#include "AgentSystem.hpp"

class AgentSystemProc : public AgentSystem {
public:
    AgentSystemProc (AgentServerLog *logger) : AgentSystem(logger)
    {
    }
    
    void systemAdd(const agent::Path *request_path);
    void systemRemove(const agent::Path *request_path);
};

#endif /* AgentSystemProc_hpp */
