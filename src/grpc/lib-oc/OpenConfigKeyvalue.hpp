//
//  OpenConfigKeyvalue.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigKeyvalue_hpp
#define OpenConfigKeyvalue_hpp

#include "OpenConfig.hpp"
#include "telemetry_top.pb.h"
#include "keyvalue.pb.h"

class OpenConfigKeyvalue : public OpenConfig {
public:
    OpenConfigKeyvalue ()
    {
        OpenConfig::registerSelf("kvresponseExt", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigKeyvalue_hpp */
