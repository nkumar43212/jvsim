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

// Unique Identifier to track an outstanding system request
class SystemId {
    size_t _id;
    
public:
    SystemId(size_t value) : _id(value) {}
    
    size_t getId()
    {
        return _id;
    }
    
    void description ()
    {
        std::cout << "Id = " << _id << "\n";
    }
};

// Interface presented by a system where all telemetry resources are provisioned
class AgentSystem {
    AgentServerLog *_logger;
    
    // Statistics to track requests into the system
    uint64_t _add_count;
    uint64_t _remove_count;
    uint64_t _error_count;
    
public:
    AgentSystem (AgentServerLog *logger) : _logger(logger) {}
    virtual void systemAdd(SystemId sys_id, const agent::Path *request_path) = 0;
    virtual void systemRemove(SystemId sys_id, const agent::Path *request_path) = 0;
    virtual agent::Path * systemGet(SystemId sys_id) = 0;
};

#endif /* AgentSystem_hpp */
