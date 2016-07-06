//
//  OpenConfigLogicalPort.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigLogicalPort.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_IFL("/oc-path/interfaces/interface");

void
OpenConfigLogicalPort::iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap)
{
    int i, size;
    GLogicalPort *portp = handle->MutableExtension(jnprLogicalInterfaceExt);
    telemetry::KeyValue *kv;

    size = portp->interface_info_size();
    for (i = 0; i < size; i++) {
        LogicalInterfaceInfo *ifl_infop = portp->mutable_interface_info(i);
        std::string ifl_name = ifl_infop->if_name();

        size_t pos = ifl_name.find(".", 0);
        std::string port_name; 
        std::string unit;
        if (pos != std::string::npos) {
            // Found matching "."
            port_name = ifl_name.substr(0, pos);
            unit      = ifl_name.substr(pos+1);
        } else {
            port_name = ifl_name;
            unit      = "0";
        }

        // Prefix
        std::string sub_unit_str("subinterfaces/subinterface[index='" + unit + "']");
        oc_set_prefix(datap, BASE_OC_PATH_IFL, port_name, sub_unit_str);

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

        // Counters
        if (ifl_infop->mutable_egress_stats()->if_octets()) {
            kv = datap->add_kv();
            kv->set_key("counters/out_octets");
            kv->set_int_value(ifl_infop->mutable_egress_stats()->if_octets());

            kv = datap->add_kv();
            kv->set_key("counters/out_packets");
            kv->set_int_value(ifl_infop->mutable_egress_stats()->if_pkts());
        }

        if (ifl_infop->mutable_ingress_stats()->if_octets()) {
            kv = datap->add_kv();
            kv->set_key("counters/in-octets");
            kv->set_int_value(ifl_infop->mutable_ingress_stats()->if_octets());

            kv = datap->add_kv();
            kv->set_key("counters/in-packets");
            kv->set_int_value(ifl_infop->mutable_ingress_stats()->if_pkts());

            kv = datap->add_kv();
            kv->set_key("counters/in-unicast-pkts");
            kv->set_int_value(ifl_infop->mutable_ingress_stats()->if_ucast_pkts());

            kv = datap->add_kv();
            kv->set_key("counters/in-multicast-pkts");
            kv->set_int_value(ifl_infop->mutable_ingress_stats()->if_mcast_pkts());
        }
    }
}
