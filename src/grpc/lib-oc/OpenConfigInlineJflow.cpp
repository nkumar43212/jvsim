//
//  OpenConfigInlineJflow.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigInlineJflow.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_JFLOW("/oc-path/components/component/subcomponents/subcomponent");
std::string OC_ATTRIBUTE_JFLOW("jflow");

void
OpenConfigInlineJflow::iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap)
{
    InlineJflow *message = handle->MutableExtension(inline_jflow_stats_ext);
    telemetry::KeyValue *kv;
    
    // Add Prefix
    std::string name_str = "FPC" + std::to_string(datap->component_id()) + ":CPU0";
    oc_set_prefix(datap, BASE_OC_PATH_JFLOW, name_str, OC_ATTRIBUTE_JFLOW);

    // Copy over
    kv = datap->add_kv();
    kv->set_key("ipv4_flows_export_format");
    kv->set_str_value(message->ipv4_flows_export_format());

    if (message->ipv4_route_record_cnt()) {
        kv = datap->add_kv();
        kv->set_key("ipv4_route_record_cnt");
        kv->set_uint_value(message->ipv4_route_record_cnt());
    }

    if (message->ipv6_route_record_cnt()) {
        kv = datap->add_kv();
        kv->set_key("ipv6_route_record_cnt");
        kv->set_uint_value(message->ipv6_route_record_cnt());
    }

    if (message->mpls_route_record_cnt()) {
        kv = datap->add_kv();
        kv->set_key("mpls_route_record_cnt");
        kv->set_uint_value(message->mpls_route_record_cnt());
    }

    if (message->memory_alloc_fail_cnt()) {
        kv = datap->add_kv();
        kv->set_key("memory_alloc_fail_cnt");
        kv->set_uint_value(message->memory_alloc_fail_cnt());
    }

    if (message->last_clear_timestamp()) {
        kv = datap->add_kv();
        kv->set_key("last_clear_timestamp");
        kv->set_uint_value(message->last_clear_timestamp());
    }

    for (int i = 0; i < message->npu_stats_size(); i++) {
        const InlineJflowNpuStats &pfe = message->npu_stats(i);

        // Add Prefix
        u_int8_t lc_slot  = datap->component_id();
        u_int8_t pfe_slot = pfe.npu_identifier() & 0xf;
        std::string pfe_str = "FPC" + std::to_string(lc_slot) + ":" + "NPU" + std::to_string(pfe_slot);
        oc_set_prefix(datap, BASE_OC_PATH_JFLOW, pfe_str, OC_ATTRIBUTE_JFLOW);

	if (pfe.ipv4_flow_packets()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flow_packets");
		kv->set_uint_value(pfe.ipv4_flow_packets());
	}

	if (pfe.ipv4_flow_bytes()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flow_bytes");
		kv->set_uint_value(pfe.ipv4_flow_bytes());
	}

	if (pfe.ipv4_total_flows()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_total_flows");
		kv->set_uint_value(pfe.ipv4_total_flows());
	}

	if (pfe.ipv4_active_flows()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_active_flows");
		kv->set_uint_value(pfe.ipv4_active_flows());
	}

	if (pfe.ipv4_flows_exported()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flows_exported");
		kv->set_uint_value(pfe.ipv4_flows_exported());
	}

	if (pfe.ipv4_packets_exported()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_packets_exported");
		kv->set_uint_value(pfe.ipv4_packets_exported());
	}

	if (pfe.ipv4_flows_inactive_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flows_inactive_timed_out");
		kv->set_uint_value(pfe.ipv4_flows_inactive_timed_out());
	}

	if (pfe.ipv4_flows_active_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flows_active_timed_out");
		kv->set_uint_value(pfe.ipv4_flows_active_timed_out());
	}

	if (pfe.ipv4_flow_create_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flow_create_failure");
		kv->set_uint_value(pfe.ipv4_flow_create_failure());
	}

	if (pfe.ipv4_route_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_route_lookup_failure");
		kv->set_uint_value(pfe.ipv4_route_lookup_failure());
	}

	if (pfe.ipv4_autonomous_system_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_autonomous_system_lookup_failure");
		kv->set_uint_value(pfe.ipv4_autonomous_system_lookup_failure());
	}

	if (pfe.ipv4_flow_packet_export_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flow_packet_export_failure");
		kv->set_uint_value(pfe.ipv4_flow_packet_export_failure());
	}

	if (pfe.ipv4_flow_table_utilization()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flow_table_utilization");
		kv->set_double_value(pfe.ipv4_flow_table_utilization());
	}

	if (pfe.ipv4_flow_insert_count()) {
		kv = datap->add_kv();
		kv->set_key("ipv4_flow_insert_count");
		kv->set_uint_value(pfe.ipv4_flow_insert_count());
	}

	if (pfe.ipv6_flow_packets()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flow_packets");
		kv->set_uint_value(pfe.ipv6_flow_packets());
	}

	if (pfe.ipv6_flow_bytes()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flow_bytes");
		kv->set_uint_value(pfe.ipv6_flow_bytes());
	}

	if (pfe.ipv6_total_flows()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_total_flows");
		kv->set_uint_value(pfe.ipv6_total_flows());
	}

	if (pfe.ipv6_active_flows()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_active_flows");
		kv->set_uint_value(pfe.ipv6_active_flows());
	}

	if (pfe.ipv6_flows_exported()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flows_exported");
		kv->set_uint_value(pfe.ipv6_flows_exported());
	}

	if (pfe.ipv6_packets_exported()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_packets_exported");
		kv->set_uint_value(pfe.ipv6_packets_exported());
	}

	if (pfe.ipv6_flows_inactive_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flows_inactive_timed_out");
		kv->set_uint_value(pfe.ipv6_flows_inactive_timed_out());
	}

	if (pfe.ipv6_flows_active_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flows_active_timed_out");
		kv->set_uint_value(pfe.ipv6_flows_active_timed_out());
	}

	if (pfe.ipv6_flow_create_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flow_create_failure");
		kv->set_uint_value(pfe.ipv6_flow_create_failure());
	}

	if (pfe.ipv6_route_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_route_lookup_failure");
		kv->set_uint_value(pfe.ipv6_route_lookup_failure());
	}

	if (pfe.ipv6_autonomous_system_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_autonomous_system_lookup_failure");
		kv->set_uint_value(pfe.ipv6_autonomous_system_lookup_failure());
	}

	if (pfe.ipv6_flow_packet_export_failure()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flow_packet_export_failure");
		kv->set_uint_value(pfe.ipv6_flow_packet_export_failure());
	}

	if (pfe.ipv6_flow_table_utilization()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flow_table_utilization");
		kv->set_double_value(pfe.ipv6_flow_table_utilization());
	}

	if (pfe.ipv6_flow_insert_count()) {
		kv = datap->add_kv();
		kv->set_key("ipv6_flow_insert_count");
		kv->set_uint_value(pfe.ipv6_flow_insert_count());
	}

	if (pfe.vpls_flow_packets()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flow_packets");
		kv->set_uint_value(pfe.vpls_flow_packets());
	}

	if (pfe.vpls_flow_bytes()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flow_bytes");
		kv->set_uint_value(pfe.vpls_flow_bytes());
	}

	if (pfe.vpls_total_flows()) {
		kv = datap->add_kv();
		kv->set_key("vpls_total_flows");
		kv->set_uint_value(pfe.vpls_total_flows());
	}

	if (pfe.vpls_active_flows()) {
		kv = datap->add_kv();
		kv->set_key("vpls_active_flows");
		kv->set_uint_value(pfe.vpls_active_flows());
	}

	if (pfe.vpls_flows_exported()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flows_exported");
		kv->set_uint_value(pfe.vpls_flows_exported());
	}

	if (pfe.vpls_packets_exported()) {
		kv = datap->add_kv();
		kv->set_key("vpls_packets_exported");
		kv->set_uint_value(pfe.vpls_packets_exported());
	}

	if (pfe.vpls_flows_inactive_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flows_inactive_timed_out");
		kv->set_uint_value(pfe.vpls_flows_inactive_timed_out());
	}

	if (pfe.vpls_flows_active_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flows_active_timed_out");
		kv->set_uint_value(pfe.vpls_flows_active_timed_out());
	}

	if (pfe.vpls_flow_create_failure()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flow_create_failure");
		kv->set_uint_value(pfe.vpls_flow_create_failure());
	}

	if (pfe.vpls_route_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("vpls_route_lookup_failure");
		kv->set_uint_value(pfe.vpls_route_lookup_failure());
	}

	if (pfe.vpls_autonomous_system_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("vpls_autonomous_system_lookup_failure");
		kv->set_uint_value(pfe.vpls_autonomous_system_lookup_failure());
	}

	if (pfe.vpls_flow_packet_export_failure()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flow_packet_export_failure");
		kv->set_uint_value(pfe.vpls_flow_packet_export_failure());
	}

	if (pfe.vpls_flow_table_utilization()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flow_table_utilization");
		kv->set_double_value(pfe.vpls_flow_table_utilization());
	}

	if (pfe.vpls_flow_insert_count()) {
		kv = datap->add_kv();
		kv->set_key("vpls_flow_insert_count");
		kv->set_uint_value(pfe.vpls_flow_insert_count());
	}

	if (pfe.mpls_flow_packets()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flow_packets");
		kv->set_uint_value(pfe.mpls_flow_packets());
	}

	if (pfe.mpls_flow_bytes()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flow_bytes");
		kv->set_uint_value(pfe.mpls_flow_bytes());
	}

	if (pfe.mpls_total_flows()) {
		kv = datap->add_kv();
		kv->set_key("mpls_total_flows");
		kv->set_uint_value(pfe.mpls_total_flows());
	}

	if (pfe.mpls_active_flows()) {
		kv = datap->add_kv();
		kv->set_key("mpls_active_flows");
		kv->set_uint_value(pfe.mpls_active_flows());
	}

	if (pfe.mpls_flows_exported()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flows_exported");
		kv->set_uint_value(pfe.mpls_flows_exported());
	}

	if (pfe.mpls_packets_exported()) {
		kv = datap->add_kv();
		kv->set_key("mpls_packets_exported");
		kv->set_uint_value(pfe.mpls_packets_exported());
	}

	if (pfe.mpls_flows_inactive_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flows_inactive_timed_out");
		kv->set_uint_value(pfe.mpls_flows_inactive_timed_out());
	}

	if (pfe.mpls_flows_active_timed_out()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flows_active_timed_out");
		kv->set_uint_value(pfe.mpls_flows_active_timed_out());
	}

	if (pfe.mpls_flow_create_failure()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flow_create_failure");
		kv->set_uint_value(pfe.mpls_flow_create_failure());
	}

	if (pfe.mpls_route_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("mpls_route_lookup_failure");
		kv->set_uint_value(pfe.mpls_route_lookup_failure());
	}

	if (pfe.mpls_autonomous_system_lookup_failure()) {
		kv = datap->add_kv();
		kv->set_key("mpls_autonomous_system_lookup_failure");
		kv->set_uint_value(pfe.mpls_autonomous_system_lookup_failure());
	}

	if (pfe.mpls_flow_packet_export_failure()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flow_packet_export_failure");
		kv->set_uint_value(pfe.mpls_flow_packet_export_failure());
	}

	if (pfe.mpls_flow_table_utilization()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flow_table_utilization");
		kv->set_double_value(pfe.mpls_flow_table_utilization());
	}

	if (pfe.mpls_flow_insert_count()) {
		kv = datap->add_kv();
		kv->set_key("mpls_flow_insert_count");
		kv->set_uint_value(pfe.mpls_flow_insert_count());
	}

    }
       
}
