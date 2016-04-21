//
//  OpenConfigPort.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigPort_hpp
#define OpenConfigPort_hpp

#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "port.pb.h"

class OpenConfigPort : public OpenConfig {
public:
    OpenConfigPort ()
    {
        OpenConfig::registerSelf("jnpr_interface_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigPort_hpp */
