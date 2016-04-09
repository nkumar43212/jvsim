//
//  OpenConfigInterface.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigInterface_hpp
#define OpenConfigInterface_hpp

#include <stdio.h>
#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "logical_port.pb.h"

class OpenConfigInterface : public OpenConfig {
public:
    OpenConfigInterface()
    {
        OpenConfig::registerSelf("interfaces", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigInterface_hpp */
