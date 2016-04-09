//
//  OpenConfigPacketStats.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigPacketStats_hpp
#define OpenConfigPacketStats_hpp

#include <stdio.h>
#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "packet_stats.pb.h"

class OpenConfigPacketStats : public OpenConfig {
public:
    OpenConfigPacketStats ()
    {
        OpenConfig::registerSelf("packet_stats", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigPacketStats_hpp */
