//
//  OpenConfigInterface.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigInterface.hpp"

void
OpenConfigInterface::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    int i, size;
    GLogicalPort *portp = handle->MutableExtension(jnprLogicalInterfaceExt);
    Telemetry::KeyValue *kv;
    
    size = portp->interface_info_size();
    for (i = 0; i < size; i++) {
        LogicalInterfaceInfo *ifl_infop = portp->mutable_interface_info(i);
        std::string master_key = "oc-path/interfaces/" + ifl_infop->if_name() + "/";
        
        kv = datap->add_kv();
        kv->set_key("__prefix__");
        kv->set_str_value(master_key);
        
        kv = datap->add_kv();
        kv->set_key("init_time");
        kv->set_int_value(ifl_infop->init_time());
        
        kv = datap->add_kv();
        kv->set_key("snmp_index");
        kv->set_int_value(ifl_infop->snmp_if_index());
        
        kv = datap->add_kv();
        kv->set_key("parent_ae_name");
        kv->set_str_value(ifl_infop->parent_ae_name());
        
        kv = datap->add_kv();
        kv->set_key("operational_state");
        kv->set_str_value(ifl_infop->mutable_op_state()->operational_status());
        
        kv = datap->add_kv();
        kv->set_key("output_packets");
        kv->set_int_value(ifl_infop->mutable_egress_stats()->if_pkts());
        
        kv = datap->add_kv();
        kv->set_key("output_bytes");
        kv->set_int_value(ifl_infop->mutable_egress_stats()->if_octets());
        
        kv = datap->add_kv();
        kv->set_key("input_packets");
        kv->set_int_value(ifl_infop->mutable_ingress_stats()->if_pkts());
        
        kv = datap->add_kv();
        kv->set_key("input_bytes");
        kv->set_int_value(ifl_infop->mutable_ingress_stats()->if_octets());
    }
}

