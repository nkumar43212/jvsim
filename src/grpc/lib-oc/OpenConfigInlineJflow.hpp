//
//  OpenConfigInlineJflow.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigInlineJflow_hpp
#define OpenConfigInlineJflow_hpp

#include "OpenConfig.hpp"
#include "telemetry_top.pb.h"
#include "inline_jflow.pb.h"

class OpenConfigInlineJflow : public OpenConfig {
public:
    OpenConfigInlineJflow ()
    {
        OpenConfig::registerSelf("inline_jflow_stats_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigInlineJflow_hpp */
