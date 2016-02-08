//
//  OpenConfigFirewall.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigFirewall.hpp"
#include <iostream>

void
OpenConfigFirewall::iterate (JuniperNetworksSensors *handle, agent::OpenConfigData *datap)
{
    Firewall *message = handle->MutableExtension(jnpr_firewall_ext);
    int i, j;
    agent::KeyValue *kv;
    
    for (i = 0; i < message->firewall_stats_size(); i++) {
        const FirewallStats& filter = message->firewall_stats(i);
        std::string master_key = "oc-path/firewalls/" + filter.filter_name() + "/";
        
        kv = datap->add_kv();
        kv->set_key("__prefix__");
        kv->set_str_value(master_key);
        
        for (j = 0; j < filter.memory_usage_size(); j++) {
            const MemoryUsage& mem = filter.memory_usage(j);
            
            kv = datap->add_kv();
            kv->set_key(master_key + "memory-utilization/" + mem.name());
            kv->set_int_value(mem.allocated());
        }
        
        for (j = 0; j < filter.counter_stats_size(); j++) {
            const CounterStats& cntr = filter.counter_stats(j);
            
            kv = datap->add_kv();
            kv->set_key("counters-packets/" + cntr.name());
            kv->set_int_value(cntr.packets());
            
            kv = datap->add_kv();
            kv->set_key("counters-bytes/" + cntr.name());
            kv->set_int_value(cntr.bytes());
        }
        
        for (j = 0; j < filter.policer_stats_size(); j++) {
             const PolicerStats& pol = filter.policer_stats(j);
            
            kv = datap->add_kv();
            kv->set_key("policer/out-of-spec-packets/" + pol.name());
            kv->set_int_value(pol.out_of_spec_packets());
            
            kv = datap->add_kv();
            kv->set_key("policer/out-of-spec-bytes/" + pol.name());
            kv->set_int_value(pol.out_of_spec_bytes());
        }
    }
    
}
