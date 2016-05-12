//
//  OpenConfigLspStats.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigLspStats.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_LSP("/oc-path/mpls/lsps/constrained-path");
std::string BASE_OC_PATH_LSP_ATTR("statistics");

void
OpenConfigLspStats::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    LspStats *message = handle->MutableExtension(jnpr_lsp_statistics_ext);
    Telemetry::KeyValue *kv;
    

    for (int i = 0; i < message->lsp_stats_records_size(); i++) {
        const LspStatsRecord& record = message->lsp_stats_records(i);

        // Add Prefix
        oc_set_prefix(datap, BASE_OC_PATH_LSP, record.name(), BASE_OC_PATH_LSP_ATTR);

        kv = datap->add_kv();
        kv->set_key(record.counter_name() + "/packets");
        kv->set_int_value(record.packets());
        
        kv = datap->add_kv();
        kv->set_key(record.counter_name() + "/bytes");
        kv->set_int_value(record.bytes());
    }
}
