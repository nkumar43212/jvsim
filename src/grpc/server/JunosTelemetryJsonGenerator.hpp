//
//  JunosTelemetryJsonGenerator.hpp
//  Telemetry Agent
//
//  Created: 4/29/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef JunosTelemetryJsonGenerator_hpp
#define JunosTelemetryJsonGenerator_hpp

#include "AgentServerProtos.h"
#include "AgentServerIdManager.hpp"

class JunosTelemetryJsonGenerator {

public:
    static std::string generate_json_junos_config(bool add,
                                    bool udp,
                                    std::string udp_server_ip,
                                    uint32_t udp_server_port,
                                    id_idx_t internal_subscription_id,
                                    const telemetry::Path *path);

    static std::string generate_json_clear_all_junos_config(void);
};

#endif /* JunosTelemetryJsonGenerator_hpp */
