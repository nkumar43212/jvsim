//
//  OCTelemetryJsonGenerator.hpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/25/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OCTelemetryJsonGenerator_hpp
#define OCTelemetryJsonGenerator_hpp

#include "AgentServerProtos.h"
#include "AgentServerIdManager.hpp"

class OCTelemetryJsonGenerator {

public:
    static std::string generate_json_oc_config (bool mqtt,
                                        id_idx_t internal_subscription_id,
                                        const Telemetry::Path *path);

};

#endif /* OCTelemetryJsonGenerator_hpp */