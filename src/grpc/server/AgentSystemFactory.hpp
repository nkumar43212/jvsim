//
//  AgentSystemFactory.hpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/25/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemFactory_hpp
#define AgentSystemFactory_hpp

#include <stdio.h>
#include "AgentSystem.hpp"

class AgentSystemFactory {
  public:
    static AgentSystem *createFile(AgentServerLog *logger, std::string file_name);
    static AgentSystem *createNull(AgentServerLog *logger);
    static AgentSystem *createSystemProcess(AgentServerLog *logger);
};

#endif /* AgentSystemFactory_hpp */
