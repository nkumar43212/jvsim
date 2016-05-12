//
//  OpenConfigPort.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigPort.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_PORT("/oc-path/interfaces/interface");
std::string BASE_OC_PATH_PORT_ATTR("interface-counters-state");

void
OpenConfigPort::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{
    int i, size;
    GPort *portp = handle->MutableExtension(jnpr_interface_ext);
    Telemetry::KeyValue *kv;

    size = portp->interface_stats_size();
    for (i = 0; i < size; i++) {
        InterfaceInfos *ifd_infop = portp->mutable_interface_stats(i);

        // Prefix
        oc_set_prefix_no_attr(datap, BASE_OC_PATH_PORT, ifd_infop->if_name());

        // Operational State values
        kv = datap->add_kv();
        kv->set_key("init_time");
        kv->set_int_value(ifd_infop->init_time());

        kv = datap->add_kv();
        kv->set_key("snmp_index");
        kv->set_int_value(ifd_infop->snmp_if_index());

        if (ifd_infop->parent_ae_name().c_str()) {
            kv = datap->add_kv();
            kv->set_key("parent_ae_name");
            kv->set_str_value(ifd_infop->parent_ae_name());
        }

        // Counters
        kv = datap->add_kv();
        kv->set_key("counters/out_octets");
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_octets());

        kv = datap->add_kv();
        kv->set_key("counters/out_packets");
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_pkts());

        kv = datap->add_kv();
        kv->set_key("counters/out-unicast-pkts");
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_uc_pkts());

        kv = datap->add_kv();
        kv->set_key("counters/out-multicast-pkts");
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_mc_pkts());

        kv = datap->add_kv();
        kv->set_key("counters/out-broadcast-pkts");
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_bc_pkts());

        kv = datap->add_kv();
        kv->set_key("counters/out-discards");
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_error());

        kv = datap->add_kv();
        kv->set_key("counters/in-octets");
        kv->set_int_value(ifd_infop->mutable_ingress_stats()->if_octets());

        kv = datap->add_kv();
        kv->set_key("counters/in-packets");
        kv->set_int_value(ifd_infop->mutable_ingress_stats()->if_pkts());

        kv = datap->add_kv();
        kv->set_key("counters/in-unicast-pkts");
        kv->set_int_value(ifd_infop->mutable_ingress_stats()->if_uc_pkts());

        kv = datap->add_kv();
        kv->set_key("counters/in-multicast-pkts");
        kv->set_int_value(ifd_infop->mutable_ingress_stats()->if_mc_pkts());
        
        kv = datap->add_kv();
        kv->set_key("counters/in-broadcast-pkts");
        kv->set_int_value(ifd_infop->mutable_ingress_stats()->if_bc_pkts());
        
        kv = datap->add_kv();
        kv->set_key("counters/in-errors");
        kv->set_int_value(ifd_infop->mutable_ingress_errors()->if_in_errors());
    }
}
