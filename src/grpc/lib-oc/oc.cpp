//
//  !! This is a generated file. Do not edit !!
// 
//  oc.cpp
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "telemetry_top.pb.h"
#include "lib_oc.h"

#include "OpenConfigNpuUtilization.hpp"
#include "OpenConfigFirewall.hpp"
#include "OpenConfigPacketStats.hpp"
#include "OpenConfigPort.hpp"
#include "OpenConfigLogicalPort.hpp"
#include "OpenConfigNpuMem.hpp"
#include "OpenConfigCpuMemoryUtilization.hpp"
#include "OpenConfigOptics.hpp"
//@header

void
lib_oc_init ()
{
    OpenConfigNpuUtilization	 *npu_utilization = new OpenConfigNpuUtilization;
    OpenConfigFirewall	 *firewall = new OpenConfigFirewall;
    OpenConfigPacketStats	 *packet_stats = new OpenConfigPacketStats;
    OpenConfigPort	 *port = new OpenConfigPort;
    OpenConfigLogicalPort	 *logical_port = new OpenConfigLogicalPort;
    OpenConfigNpuMem	 *npu_mem = new OpenConfigNpuMem;
    OpenConfigCpuMemoryUtilization	 *cpu_memory_utilization = new OpenConfigCpuMemoryUtilization;
    OpenConfigOptics	 *optics = new OpenConfigOptics;
//@next
}

std::string
get_oc_extension_type (JuniperNetworksSensors *handle)
{
    if (handle->HasExtension(jnpr_npu_utilization_ext)) {
        return "jnpr_npu_utilization_ext";
    }
    if (handle->HasExtension(jnpr_firewall_ext)) {
        return "jnpr_firewall_ext";
    }
    if (handle->HasExtension(jnpr_packet_statistics_ext)) {
        return "jnpr_packet_statistics_ext";
    }
    if (handle->HasExtension(jnpr_interface_ext)) {
        return "jnpr_interface_ext";
    }
    if (handle->HasExtension(jnprLogicalInterfaceExt)) {
        return "jnprLogicalInterfaceExt";
    }
    if (handle->HasExtension(npu_memory_ext)) {
        return "npu_memory_ext";
    }
    if (handle->HasExtension(cpu_memory_util_ext)) {
        return "cpu_memory_util_ext";
    }
    if (handle->HasExtension(jnpr_optics_ext)) {
        return "jnpr_optics_ext";
    }
//@ext_type
    return "unknown";
}
