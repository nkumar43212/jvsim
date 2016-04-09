//
//  OpenConfigFirewall.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigFirewall_hpp
#define OpenConfigFirewall_hpp

#include <stdio.h>
#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "firewall.pb.h"

class OpenConfigFirewall : public OpenConfig {
public:
    OpenConfigFirewall ()
    {
        OpenConfig::registerSelf("firewall", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigFirewall_hpp */
