//
//  OpenConfigNpuMem.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigNpuMem.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_NPU("/oc-path/components/component/subcomponents/subcomponent");
std::string OC_ATTRIBUTE_NPU("memory");

void
OpenConfigNpuMem::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    NetworkProcessorMemoryUtilization *message = handle->MutableExtension(npu_memory_ext);
    Telemetry::KeyValue *kv;
    
    for (int i = 0; i < message->memory_stats_size(); i++) {
        oc_set_prefix(datap, BASE_OC_PATH_NPU, message->memory_stats(i).identifier(), OC_ATTRIBUTE_NPU);
        
        for (int j = 0; j < message->memory_stats(i).summary_size(); j++) {
            const NpuMemorySummary &summary = message->memory_stats(i).summary(j);
            
            if (!summary.allocated()) {
                continue;
            }

            kv = datap->add_kv();
            kv->set_key("summary/" + summary.resource_name() + "/size");
            kv->set_uint_value(summary.size());
            
            kv = datap->add_kv();
            kv->set_key("summary/" + summary.resource_name() + "/allocated");
            kv->set_uint_value(summary.allocated());
            
            kv = datap->add_kv();
            kv->set_key("summary/" + summary.resource_name() + "/utilization");
            kv->set_int_value(summary.utilization());
        }
        
        for (int j = 0; j < message->memory_stats(i).partition_size(); j++) {
            const NpuMemoryPartition &partition = message->memory_stats(i).partition(j);
            
            if (!partition.bytes_allocated()) {
                continue;
            }

            kv = datap->add_kv();
            kv->set_key("partition/" + partition.application_name() + "/partition_name");
            kv->set_str_value(partition.name());
            
            kv = datap->add_kv();
            kv->set_key("partition/" + partition.application_name() + "/bytes_allocated");
            kv->set_int_value(partition.bytes_allocated());
            
            kv = datap->add_kv();
            kv->set_key("partition/" + partition.application_name() + "/allocation_count");
            kv->set_int_value(partition.allocation_count());
            
            kv = datap->add_kv();
            kv->set_key("partition/" + partition.application_name() + "/free_count");
            kv->set_int_value(partition.free_count());
        }
    }
}
