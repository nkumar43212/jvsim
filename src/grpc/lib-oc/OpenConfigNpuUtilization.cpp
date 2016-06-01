//
//  OpenConfigNpuUtilization.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigNpuUtilization.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_NPU_UTIL("/oc-path/components/component/subcomponents/subcomponent");
std::string OC_ATTRIBUTE_NPU_UTIL("utilization");

void
OpenConfigNpuUtilization::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    NetworkProcessorUtilization *message = handle->MutableExtension(jnpr_npu_utilization_ext);
    int i;
    Telemetry::KeyValue *kv;

    for (i = 0; i < message->npu_util_stats_size(); i++) {
        const Utilization &util = message->npu_util_stats(i);

        oc_set_prefix(datap, BASE_OC_PATH_NPU_UTIL, util.identifier(), OC_ATTRIBUTE_NPU_UTIL);

        kv = datap->add_kv();
        kv->set_key("metric");
        kv->set_int_value(util.utilization());

        for (int j = 0; j < util.memory_size(); j++) {
            const MemoryLoad &mem = util.memory(j);

            kv = datap->add_kv();
            kv->set_key("memory/" + mem.name() + "/average_util");
            kv->set_int_value(mem.average_util());

            kv = datap->add_kv();
            kv->set_key("memory/" + mem.name() + "/highest_util");
            kv->set_int_value(mem.highest_util());

            kv = datap->add_kv();
            kv->set_key("memory/" + mem.name() + "/lowest_util");
            kv->set_int_value(mem.highest_util());
        }

        for (int j = 0; j < util.packets_size(); j++) {
            const PacketLoad &packet = util.packets(j);

            if (!packet.rate()) {
                continue;
            }

            kv = datap->add_kv();
            kv->set_key("packet/" + packet.identifier() + "/rate");
            kv->set_int_value(packet.rate());
        }
    }
}
