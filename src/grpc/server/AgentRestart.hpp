//
//  AgentRestart.hpp
//  Telemetry Agent
//
//  Created: 6/9/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentRestart_hpp
#define AgentRestart_hpp

#include "AgentServerLog.hpp"
#include "AgentSystemFactory.hpp"

bool on_startup (AgentServerLog *logger, AgentSystem *sys_handle);

#endif /* AgentRestart_hpp */
