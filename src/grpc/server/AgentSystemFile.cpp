//
//  AgentSystemFile.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemFile.hpp"

void
AgentSystemFile::systemAdd (const agent::Path *request_path)
{
    // The common interface
    AgentSystem::systemAdd(request_path);
    
    // Generate a sensor name
    std::string sensor_name;
    generateName(request_path, sensor_name);
    
    // Generate the message
    std::string message("");
    generateAddMessage(request_path, sensor_name, message);
    
    // Write it out
    _outputFile << message << "\n";
    _outputFile.flush();
}

void
AgentSystemFile::systemRemove (const agent::Path *request_path)
{
    // The common interface
    AgentSystem::systemRemove(request_path);
    
    // Generate a sensor name
    std::string sensor_name;
    generateName(request_path, sensor_name);
    
    // Write it out
    std::string message("");
    generateRemoveMessage(request_path, sensor_name, message);
    
    // Write it out
    _outputFile << message << "\n";
    _outputFile.flush();
}