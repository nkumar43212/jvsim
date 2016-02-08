//
//  OpenConfigFoo.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigFoo_hpp
#define OpenConfigFoo_hpp

#include <stdio.h>
#include "OpenConfig.hpp"
#include "jvision_top.pb.h"
#include "Foo.pb.h"

class OpenConfigFoo : public OpenConfig {
public:
    OpenConfigFoo()
    {
        OpenConfig::registerSelf("FooStr", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, agent::OpenConfigData *datap);
};

#endif /* OpenConfigFoo_hpp */
