//
//  OpenConfigFirewall.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigFirewall.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_FW("/oc-path/firewalls/");

void
OpenConfigFirewall::iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap)
{
    Firewall *message = handle->MutableExtension(jnpr_firewall_ext);
    int i, j;
    telemetry::KeyValue *kv;

    for (i = 0; i < message->firewall_stats_size(); i++) {
        const FirewallStats& filter = message->firewall_stats(i);
        oc_set_prefix_no_attr(datap, BASE_OC_PATH_FW, filter.filter_name());

        kv = datap->add_kv();
        kv->set_key("timestamp");
        kv->set_int_value(filter.timestamp());

        for (j = 0; j < filter.memory_usage_size(); j++) {
            const MemoryUsage& mem = filter.memory_usage(j);
            
            if (!mem.allocated()) {
                continue;
            }

            kv = datap->add_kv();
            kv->set_key("memory_usage/" + mem.name());
            kv->set_int_value(mem.allocated());
        }

        for (j = 0; j < filter.counter_stats_size(); j++) {
            const CounterStats& cntr = filter.counter_stats(j);
            
            if (!cntr.packets()) {
                continue;
            }

            kv = datap->add_kv();
            kv->set_key("counters/packets/" + cntr.name());
            kv->set_int_value(cntr.packets());
            
            kv = datap->add_kv();
            kv->set_key("counters/bytes/" + cntr.name());
            kv->set_int_value(cntr.bytes());
        }

        for (j = 0; j < filter.policer_stats_size(); j++) {
            const PolicerStats& pol = filter.policer_stats(j);
            
            if (!pol.out_of_spec_packets()) {
                continue;
            }

            kv = datap->add_kv();
            kv->set_key("policers/out-of-spec-packets/" + pol.name());
            kv->set_int_value(pol.out_of_spec_packets());
            
            kv = datap->add_kv();
            kv->set_key("policers/out-of-spec-bytes/" + pol.name());
            kv->set_int_value(pol.out_of_spec_bytes());
        }
    }
}
