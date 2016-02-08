//
//  OpenConfigPort.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigPort.hpp"
#include <iostream>

void
OpenConfigPort::iterate (JuniperNetworksSensors *handle, agent::OpenConfigData *datap)
{
    int i, size;
    GPort *portp = handle->MutableExtension(jnpr_interface_ext);
    agent::KeyValue *kv;

    size = portp->interface_stats_size();
    for (i = 0; i < size; i++) {
        InterfaceInfos *ifd_infop = portp->mutable_interface_stats(i);
        std::string master_key = "oc-path/interfaces/" + ifd_infop->if_name() + "/";
        std::string field_key;

        kv = datap->add_kv();
        kv->set_key("__prefix__");
        kv->set_str_value(master_key);

        kv = datap->add_kv();
        field_key = master_key + "init_time";
        //kv->set_key("init_time");
        kv->set_key(field_key);
        kv->set_int_value(ifd_infop->init_time());

        kv = datap->add_kv();
        //kv->set_key("snmp_index");
        field_key = master_key + "snmp_index";
        kv->set_key(field_key);
        kv->set_int_value(ifd_infop->snmp_if_index());

        kv = datap->add_kv();
        //kv->set_key("parent_ae_name");
        field_key = master_key + "parent_ae_name";
        kv->set_key(field_key);
        kv->set_str_value(ifd_infop->parent_ae_name());

        kv = datap->add_kv();
        //kv->set_key("output_packets");
        field_key = master_key + "output_packets";
        kv->set_key(field_key);
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_pkts());

        kv = datap->add_kv();
        //kv->set_key("output_bytes");
        field_key = master_key + "output_bytes";
        kv->set_key(field_key);
        kv->set_int_value(ifd_infop->mutable_egress_stats()->if_octets());

        kv = datap->add_kv();
        //kv->set_key("input_packets");
        field_key = master_key + "input_packets";
        kv->set_key(field_key);
        kv->set_int_value(ifd_infop->mutable_ingress_stats()->if_pkts());
    }
}
