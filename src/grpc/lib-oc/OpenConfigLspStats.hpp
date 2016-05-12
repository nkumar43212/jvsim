//
//  OpenConfigLspStats.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef OpenConfigLspStats_hpp
#define OpenConfigLspStats_hpp

#include "OpenConfig.hpp"
#include "telemetry_top.pb.h"
#include "lsp_stats.pb.h"

class OpenConfigLspStats : public OpenConfig {
public:
    OpenConfigLspStats ()
    {
        OpenConfig::registerSelf("jnpr_lsp_statistics_ext", this);
    }
    
    void   iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap);
};

#endif /* OpenConfigLspStats_hpp */
