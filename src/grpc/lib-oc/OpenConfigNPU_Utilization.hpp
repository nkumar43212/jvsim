//
//  OpenConfigNPU_Utilization.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigNPU_Utilization_hpp
#define OpenConfigNPU_Utilization_hpp

#include <stdio.h>
#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "npu_util.pb.h"

class OpenConfigNPU_Utilization : public OpenConfig {
public:
    OpenConfigNPU_Utilization ()
    {
        OpenConfig::registerSelf("npu_util", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigNPU_Utilization_hpp */
