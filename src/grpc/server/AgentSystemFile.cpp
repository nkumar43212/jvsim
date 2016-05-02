//
//  AgentSystemFile.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemFile.hpp"
#include "OCTelemetryJsonGenerator.hpp"
#include "JunosTelemetryJsonGenerator.hpp"

void
AgentSystemFile::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
    // The common interface
    AgentSystem::systemAdd(id, request_path);

#if defined(__OC_Telemetry_Config__)
    bool mqtt = true;
    // Generate the Jsonized OC configuration
    std::string config = OCTelemetryJsonGenerator::generate_json_oc_config(mqtt,
                                    (id_idx_t)id.getId(), request_path);
#else
    // Generate the Jsonized Junos configuration
    bool add = true;
    bool mqtt = true;
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
                         (add, mqtt, (id_idx_t)id.getId(), request_path);
#endif

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

#if defined(__OC_Telemetry_Config__)
    bool mqtt = true;
    // Generate the Jsonized OC configuration
    std::string config = OCTelemetryJsonGenerator::generate_json_oc_config(mqtt,
                                    (id_idx_t)id.getId(), request_path);
#else
    // Generate the Jsonized Junos configuration
    bool add = false;
    bool mqtt = true;
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
                         (add, mqtt, (id_idx_t)id.getId(), request_path);
#endif

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