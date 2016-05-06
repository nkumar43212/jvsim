//
//  OpenConfigCpuMemoryUtilization.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigCpuMemoryUtilization_hpp
#define OpenConfigCpuMemoryUtilization_hpp

#include "OpenConfig.hpp"
#include "telemetry_top.pb.h"
#include "cpu_memory_utilization.pb.h"

class OpenConfigCpuMemoryUtilization : public OpenConfig {
public:
    OpenConfigCpuMemoryUtilization ()
    {
        OpenConfig::registerSelf("cpu_memory_util_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigCpuMemoryUtilization_hpp */
