//
//  Sensor_keys.h
//  udp_data_simulator
//
//  Created by Abbas Sakarwala on 7/26/16.
//
//

#ifndef Sensor_keys_h
#define Sensor_keys_h

// Keys of /cpu/memory
std::string cpu_memory_keys[] = {
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/size",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/utilization",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/nh/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/nh/allocations",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/nh/frees",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/nh/allocations_failed",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/dfw/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/dfw/allocations",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/dfw/frees",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/dfw/allocations_failed",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/ifl/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/ifl/allocations",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/ifl/frees",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationkernel/applications/ifl/allocations_failed",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/size",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/utilization",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/nh/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/nh/allocations",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/nh/frees",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/nh/allocations_failed",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/dfw/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/dfw/allocations",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/dfw/frees",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/dfw/allocations_failed",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/ifl/bytes_allocated",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/ifl/allocations",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/ifl/frees",
    "/oc-path/components/component/subcomponents/subcomponent[name='FPC5:CPU0']/memory_utilizationpackets/applications/ifl/allocations_failed"
};

// Keys of firewall
std::string firewall_keys[] = {
    "/oc-path/firewalls/[name='filter-0']/timestamp",
    "/oc-path/firewalls/[name='filter-0']/memory_usage/mem1",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-0",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-0",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-1",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-1",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-2",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-2",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-3",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-3",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-4",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-4",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-5",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-5",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-6",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-6",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-7",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-7",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-8",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-8",
    "/oc-path/firewalls/[name='filter-0']/counters/packets/cnt-9",
    "/oc-path/firewalls/[name='filter-0']/counters/bytes/cnt-9"
};

// Keys of lsp
std::string lsp_keys[] = {
    "/oc-path/mpls/lsps/constrained-path[name='foo-0']/statisticscounter1/packets",
    "/oc-path/mpls/lsps/constrained-path[name='foo-0']/statisticscounter1/bytes"
};

// Keys of logical_port
std::string logical_port_keys[] = {
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/init_time",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/snmp_index",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/parent_ae_name",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/operational_state",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/counters/out_octets",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/counters/out_packets",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/counters/in-octets",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/counters/in-packets",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/counters/in-unicast-pkts",
    "/oc-path/interfaces/interface[name='xe-0']/subinterfaces/subinterface[index='10']/counters/in-multicast-pkts"
};

#endif /* Sensor_keys_h */
