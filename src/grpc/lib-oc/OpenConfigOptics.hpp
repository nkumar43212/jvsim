//
//  OpenConfigOptics.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigOptics_hpp
#define OpenConfigOptics_hpp

#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "optics.pb.h"

class OpenConfigOptics : public OpenConfig {
public:
    OpenConfigOptics ()
    {
        OpenConfig::registerSelf("jnpr_optics_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigOptics_hpp */
