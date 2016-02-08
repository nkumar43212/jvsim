//
//  OpenConfigPort.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigPort_hpp
#define OpenConfigPort_hpp

#include <stdio.h>
#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "port.pb.h"

class OpenConfigPort : public OpenConfig {
public:
    OpenConfigPort ()
    {
        OpenConfig::registerSelf("port", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, agent::OpenConfigData *datap);
};

#endif /* OpenConfigPort_hpp */
