//
//  OpenConfigPacketStats.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigPacketStats.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_PACKET_STATS("/oc-path/components/component/subcomponents/subcomponent");
std::string OC_ATTRIBUTE_PACKET_STATS("packet_statistics");

void
OpenConfigPacketStats::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    PacketStatistics *message = handle->MutableExtension(jnpr_packet_statistics_ext);
    int i;
    Telemetry::KeyValue *kv;

    // Add Prefix
    std::string name_str = "FPC" + std::to_string(datap->component_id()) + ":CPU0";
    oc_set_prefix(datap, BASE_OC_PATH_PACKET_STATS, name_str, OC_ATTRIBUTE_PACKET_STATS);

    for (i = 0; i < message->packet_stats_size(); i++) {
        const PacketStatsClass &classp = message->packet_stats(i);
        
        kv = datap->add_kv();
        kv->set_key(classp.name());
        kv->set_int_value(classp.counter().packet_count());
    }

    for (i = 0; i < message->packet_stats_pfe_size(); i++) {
        const PacketStatsPacketForwardingEngine &pfe = message->packet_stats_pfe(i);

        // Add Prefix
        oc_set_prefix(datap, BASE_OC_PATH_PACKET_STATS, pfe.pfe_identifier(), OC_ATTRIBUTE_PACKET_STATS);
        for (int j = 0; j < pfe.packet_stats_size(); j++) {
            const PacketStatsClass &classp = pfe.packet_stats(j);
            
            kv = datap->add_kv();
            kv->set_key(classp.name());
            kv->set_int_value(classp.counter().packet_count());
        }
    }
}
