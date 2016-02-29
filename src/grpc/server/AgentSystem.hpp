//
//  AgentSystem.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/21/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystem_hpp
#define AgentSystem_hpp

#include <stdio.h>
#include "AgentServerProtos.h"
#include "AgentServerLog.hpp"

class AgentSystem {
    AgentServerLog *_logger;
    
    // Statistics to track requests into the system
    uint64_t _add_count;
    uint64_t _remove_count;
    uint64_t _error_count;
    
public:
    AgentSystem (AgentServerLog *logger) : _logger(logger) {}
    virtual void systemAdd(const agent::Path *request_path) = 0;
    virtual void systemRemove(const agent::Path *request_path) = 0;
};

#endif /* AgentSystem_hpp */
