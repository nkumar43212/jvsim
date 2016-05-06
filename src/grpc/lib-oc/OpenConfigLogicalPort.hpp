//
//  OpenConfigLogicalPort.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigLogicalPort_hpp
#define OpenConfigLogicalPort_hpp

#include "OpenConfig.hpp"
#include "telemetry_top.pb.h"
#include "logical_port.pb.h"

class OpenConfigLogicalPort : public OpenConfig {
public:
    OpenConfigLogicalPort ()
    {
        OpenConfig::registerSelf("jnprLogicalInterfaceExt", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigLogicalPort_hpp */
