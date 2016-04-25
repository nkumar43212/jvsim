//
//  AgentSystemFile.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemFile.hpp"
#include "OCTelemetryJsonGenerator.hpp"

void
AgentSystemFile::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
    // The common interface
    AgentSystem::systemAdd(id, request_path);

    // Generate the Jsonized OC configuration
    std::string config = OCTelemetryJsonGenerator::generate_json_oc_config(true,
                                    (id_idx_t)id.getId(), request_path);

    // Write it out
    _outputFile << "Push config (" << id.getId() << "):" << std::endl;
    _outputFile << config << std::endl;
    _outputFile.flush();
}

void
AgentSystemFile::systemRemove (SystemId id, const Telemetry::Path *request_path)
{
    // The common interface
    AgentSystem::systemRemove(id, request_path);

    // Generate the Jsonized OC configuration
    std::string config = OCTelemetryJsonGenerator::generate_json_oc_config(true,
                                    (id_idx_t)id.getId(), request_path);

    // Write it out
    _outputFile << "Remove config (" << id.getId() << "):" << std::endl;
    _outputFile << config << std::endl;
    _outputFile.flush();
}

Telemetry::Path *
AgentSystemFile::systemGet (SystemId sys_id)
{
    return NULL;
}