//
//  OpenConfigPacketStats.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigPacketStats_hpp
#define OpenConfigPacketStats_hpp

#include "OpenConfig.hpp"
#include "telemetry_top.pb.h"
#include "packet_stats.pb.h"

class OpenConfigPacketStats : public OpenConfig {
public:
    OpenConfigPacketStats ()
    {
        OpenConfig::registerSelf("jnpr_packet_statistics_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigPacketStats_hpp */
