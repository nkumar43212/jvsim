//
//  OpenConfigCpuMemoryUtilization.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigCpuMemoryUtilization.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_CPU("/oc-path/components/component/subcomponents/subcomponent");
std::string OC_ATTRIBUTE_CPU("memory_utilization");

void
OpenConfigCpuMemoryUtilization::iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap)
{
    CpuMemoryUtilization *message = handle->MutableExtension(cpu_memory_util_ext);
    telemetry::KeyValue *kv;
    
    // Add Prefix
    std::string name_str = "FPC" + std::to_string(datap->component_id()) + ":CPU0";
    oc_set_prefix(datap, BASE_OC_PATH_CPU, name_str, OC_ATTRIBUTE_CPU);

    for (int i = 0; i < message->utilization_size(); i++) {
        const CpuMemoryUtilizationSummary& summary = message->utilization(i);
       
        if (!summary.bytes_allocated()) {
            continue;
        }
 
        kv = datap->add_kv();
        kv->set_key(summary.name() + "/size");
        kv->set_uint_value(summary.size());
        
        kv = datap->add_kv();
        kv->set_key(summary.name() + "/bytes_allocated");
        kv->set_uint_value(summary.bytes_allocated());
        
        kv = datap->add_kv();
        kv->set_key(summary.name() + "/utilization");
        kv->set_int_value(summary.utilization());
        
        for (int j = 0; j < summary.application_utilization_size(); j++) {
            const CpuMemoryUtilizationPerApplication &app = summary.application_utilization(j);
            
            if (!app.bytes_allocated()) {
                continue;
            }

            kv = datap->add_kv();
            kv->set_key(summary.name() + "/applications/" + app.name() + "/bytes_allocated");
            kv->set_uint_value(app.bytes_allocated());
            
            kv = datap->add_kv();
            kv->set_key(summary.name() + "/applications/" + app.name() + "/allocations");
            kv->set_uint_value(app.allocations());

            kv = datap->add_kv();
            kv->set_key(summary.name() + "/applications/" + app.name() + "/frees");
            kv->set_uint_value(app.frees());
            
            kv = datap->add_kv();
            kv->set_key(summary.name() + "/applications/" + app.name() + "/allocations_failed");
            kv->set_uint_value(app.allocations_failed());
        }
    }
}
