//
//  OpenConfigNpuUtilization.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigNpuUtilization_hpp
#define OpenConfigNpuUtilization_hpp

#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "npu_utilization.pb.h"

class OpenConfigNpuUtilization : public OpenConfig {
public:
    OpenConfigNpuUtilization ()
    {
        OpenConfig::registerSelf("jnpr_npu_utilization_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigNpuUtilization_hpp */
