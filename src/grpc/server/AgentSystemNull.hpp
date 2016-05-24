//
//  AgentSystemNull.hpp
//  Telemetry Agent
//
//  Created: 2/27/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemNull_hpp
#define AgentSystemNull_hpp

#include "AgentSystem.hpp"

class AgentSystemNull : public AgentSystem {
public:
    AgentSystemNull (AgentServerLog *logger) : AgentSystem(logger) {}

    void systemAdd(SystemId id, const Telemetry::Path *request_path);
    void systemRemove(SystemId id, const Telemetry::Path *request_path);
    Telemetry::Path * systemGet(SystemId sys_id);
};

#endif /* AgentSystemNull_hpp */