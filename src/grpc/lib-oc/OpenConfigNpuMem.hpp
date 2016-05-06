//
//  OpenConfigNpuMem.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigNpuMem_hpp
#define OpenConfigNpuMem_hpp

#include "OpenConfig.hpp"
#include "telemetry_top.pb.h"
#include "npu_mem.pb.h"

class OpenConfigNpuMem : public OpenConfig {
public:
    OpenConfigNpuMem ()
    {
        OpenConfig::registerSelf("npu_memory_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigNpuMem_hpp */
