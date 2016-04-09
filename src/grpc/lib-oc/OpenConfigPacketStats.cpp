//
//  OpenConfigPacketStats.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigPacketStats.hpp"
#include <iostream>

void
OpenConfigPacketStats::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    PacketStatistics *message = handle->MutableExtension(jnpr_packet_statistics_ext);
    int i;
    Telemetry::KeyValue *kv;
    
    std::string master_key = "oc-path/packet-statistics/";
    kv = datap->add_kv();
    kv->set_key("__prefix__");
    kv->set_str_value(master_key);
    
    for (i = 0; i < message->packet_stats_size(); i++) {
        const PacketStatsClass &classp = message->packet_stats(i);
        
        kv = datap->add_kv();
        kv->set_key(classp.name());
        kv->set_int_value(classp.counter().packet_count());
    }
    
    for (i = 0; i < message->packet_stats_pfe_size(); i++) {
        const PacketStatsPacketForwardingEngine &pfe = message->packet_stats_pfe(i);
      
        kv = datap->add_kv();
        kv->set_key("__prefix__");
        kv->set_str_value("pfe/" + pfe.pfe_identifier());
        for (int j = 0; j < pfe.packet_stats_size(); j++) {
            const PacketStatsClass &classp = pfe.packet_stats(j);
            
            kv = datap->add_kv();
            kv->set_key(classp.name());
            kv->set_int_value(classp.counter().packet_count());
        }
    }
}

