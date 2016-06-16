//
//  PidFileUtils.hpp
//  Telemetry Agent
//
//  Created: 6/15/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef PidFileUtils_hpp
#define PidFileUtils_hpp

#include "AgentServerLog.hpp"

int pid_lock(std::string filename, AgentServerLog *logger);

#endif /* PidFileUtils_hpp */
