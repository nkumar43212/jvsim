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
    ~AgentSystemNull() {}

    bool systemAdd(SystemId id, const telemetry::Path *request_path);
    bool systemRemove(SystemId id, const telemetry::Path *request_path);
    telemetry::Path * systemGet(SystemId sys_id);
    bool systemClearAll(void);
};

#endif /* AgentSystemNull_hpp */
