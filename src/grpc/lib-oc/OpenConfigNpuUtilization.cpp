//
//  OpenConfigNpuUtilization.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigNpuUtilization.hpp"

void
OpenConfigNpuUtilization::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    NetworkProcessorUtilization *message = handle->MutableExtension(jnpr_npu_utilization_ext);
    int i;
    Telemetry::KeyValue *kv;

    std::string master_key = "oc-path/network-processor-utilization/";
    kv = datap->add_kv();
    kv->set_key("__prefix__");
    kv->set_str_value(master_key);

    for (i = 0; i < message->npu_util_stats_size(); i++) {
        const Utilization &util = message->npu_util_stats(i);

        kv = datap->add_kv();
        kv->set_key("__prefix__");
        kv->set_str_value(util.identifier());

        kv = datap->add_kv();
        kv->set_key("utilization");
        kv->set_int_value(util.utilization());

        for (int j = 0; j < util.memory_size(); j++) {
            const MemoryLoad &mem = util.memory(j);

            kv = datap->add_kv();
            kv->set_key("__prefix__");
            kv->set_str_value("memory/" + mem.name() + "/");

            kv = datap->add_kv();
            kv->set_key("/average_util");
            kv->set_int_value(mem.average_util());

            kv = datap->add_kv();
            kv->set_key("/highest_util");
            kv->set_int_value(mem.highest_util());

            kv = datap->add_kv();
            kv->set_key("/lowest_util");
            kv->set_int_value(mem.highest_util());
        }

        for (int j = 0; j < util.packets_size(); j++) {
            const PacketLoad &packet = util.packets(j);

            kv = datap->add_kv();
            kv->set_key("__prefix__");
            kv->set_str_value("packet/" + packet.identifier() + "/");

            kv = datap->add_kv();
            kv->set_key("/rate");
            kv->set_int_value(packet.rate());
        }
    }
}