//
//  OpenConfigCpuMemoryUtilization.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigCpuMemoryUtilization.hpp"

void
OpenConfigCpuMemoryUtilization::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    CpuMemoryUtilization *message = handle->MutableExtension(cpu_memory_util_ext);
    Telemetry::KeyValue *kv;
    
    for (int i = 0; i < message->utilization_size(); i++) {
        const CpuMemoryUtilizationSummary& summary = message->utilization(i);
        
        kv = datap->add_kv();
        kv->set_key("oc-path/cpu-memory-utilization/" + summary.name() + "/size");
        kv->set_int_value(summary.size());
        
        
        kv = datap->add_kv();
        kv->set_key("oc-path/cpu-memory-utilization/" + summary.name() + "/bytes_allocated");
        kv->set_int_value(summary.bytes_allocated());
        
        
        kv = datap->add_kv();
        kv->set_key("oc-path/cpu-memory-utilization/" + summary.name() + "/utilization");
        kv->set_int_value(summary.utilization());
        
        
        for (int j = 0; j < summary.application_utilization_size(); j++) {
            const CpuMemoryUtilizationPerApplication &app = summary.application_utilization(j);
            
            kv = datap->add_kv();
            kv->set_key("oc-path/cpu-memory-utilization/" + summary.name() + "/applications/" + app.name() + "/bytes_allocated");
            kv->set_int_value(app.bytes_allocated());
            
            kv = datap->add_kv();
            kv->set_key("oc-path/cpu-memory-utilization/" + summary.name() + "/applications/" + app.name() + "/allocations");
            kv->set_int_value(app.allocations());
            
            kv = datap->add_kv();
            kv->set_key("oc-path/cpu-memory-utilization/" + summary.name() + "/applications/" + app.name() + "/frees");
            kv->set_int_value(app.frees());
            
            kv = datap->add_kv();
            kv->set_key("oc-path/cpu-memory-utilization/" + summary.name() + "/applications/" + app.name() + "/allocations_failed");
            kv->set_int_value(app.allocations_failed());
        }
    }
}
