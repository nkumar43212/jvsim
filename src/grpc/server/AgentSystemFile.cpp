//
//  AgentSystemFile.cpp
//  Telemetry Agent
//
//  Created: 2/27/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystemFile.hpp"
#include "OCTelemetryJsonGenerator.hpp"
#include "JunosTelemetryJsonGenerator.hpp"
#include "GlobalConfig.hpp"

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
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
                         (add,
                          global_config.udp_server_module,
                          global_config.udp_server_ip,
                          global_config.udp_server_port,
                          (id_idx_t)id.getId(),
                          request_path);
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
    std::string config = JunosTelemetryJsonGenerator::generate_json_junos_config
                         (add,
                          global_config.udp_server_module,
                          global_config.udp_server_ip,
                          global_config.udp_server_port,
                          (id_idx_t)id.getId(),
                          request_path);
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

bool
AgentSystemFile::systemClearAll (void)
{
#if defined(__OC_Telemetry_Config__)
    std::string config = "Unsupported for now";
#else
    // Generate the Jsonized Junos configuration
    std::string config =
        JunosTelemetryJsonGenerator::generate_json_clear_all_junos_config();
#endif

    // Write it out
    _outputFile << "ClearAll config: " << std::endl;
    _outputFile << config << std::endl;
    _outputFile.flush();

    return true;
}
