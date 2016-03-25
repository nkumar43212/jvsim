import os
import sys
import packet_stats_pb2

def jv_encode_packet_stats(jnpr, index, lc_slot, system_params):
    packet_stats = jnpr.Extensions[packet_stats_pb2.jnpr_packet_statistics_ext]

    # Global per linecard stats
    record       = packet_stats.packet_stats.add()
    record.name  = "ttl_expires";
    record.counter.packet_count = 100;

    record       = packet_stats.packet_stats.add()
    record.name  = "ttl_expires_pps";
    record.counter.packet_count = 1000;

    # Per PFE statistics
    x = 0
    for x in range(0, system_params['pfe_count']):
        pfe_record   = packet_stats.packet_stats_pfe.add();
        pfe_record.pfe_identifier = "pfe" + str(x);

        record  = pfe_record.packet_stats.add();
        record.name  = "v4_discards";
        record.counter.packet_count = 500;

        record       = pfe_record.packet_stats.add()
        record.name  = "v6_discards";
        record.counter.packet_count = 600;
