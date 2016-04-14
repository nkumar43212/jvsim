//
//  AgentSystemFile.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemFile.hpp"

void
AgentSystemFile::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
    // The common interface
    AgentSystem::systemAdd(id, request_path);
    
    // Generate a sensor name
    std::string sensor_name;
    generateName(request_path, sensor_name);
    
    // Generate the message
    std::string message("");
    generateAddMessage(request_path, sensor_name, message);
    
    // Write it out
    _outputFile << id.getId() << "\n";
    _outputFile << message << "\n";
    _outputFile.flush();
}

void
AgentSystemFile::systemRemove (SystemId id, const Telemetry::Path *request_path)
{
    // The common interface
    AgentSystem::systemRemove(id, request_path);
    
    // Generate a sensor name
    std::string sensor_name;
    generateName(request_path, sensor_name);
    
    // Write it out
    std::string message("");
    generateRemoveMessage(request_path, sensor_name, message);
    
    // Write it out
    _outputFile << id.getId() << "\n";
    _outputFile << message << "\n";
    _outputFile.flush();
}

Telemetry::Path *
AgentSystemFile::systemGet (SystemId sys_id)
{
    return NULL;
}
