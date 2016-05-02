//
//  JunosTelemetryJsonGenerator.hpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef JunosTelemetryJsonGenerator_hpp
#define JunosTelemetryJsonGenerator_hpp

#include "AgentServerProtos.h"
#include "AgentServerIdManager.hpp"

class JunosTelemetryJsonGenerator {

public:
    static std::string generate_json_junos_config (bool add,
                                    bool mqtt,
                                    id_idx_t internal_subscription_id,
                                    const Telemetry::Path *path);
};

#endif /* JunosTelemetryJsonGenerator_hpp */