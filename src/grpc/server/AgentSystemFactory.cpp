//
//  AgentSystemFactory.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/25/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemFactory.hpp"
#include "AgentSystemNull.hpp"
#include "AgentSystemFile.hpp"
#include "AgentSystemProc.hpp"


AgentSystem *
AgentSystemFactory::createNull (AgentServerLog *logger)
{
    return new AgentSystemNull(logger);
}

AgentSystem *
AgentSystemFactory::createFile (AgentServerLog *logger, std::string file_name)
{
    return new AgentSystemFile(logger, file_name);
}

AgentSystem *
AgentSystemFactory::createSystemProcess (AgentServerLog *logger)
{
    return new AgentSystemProc(logger);
}
