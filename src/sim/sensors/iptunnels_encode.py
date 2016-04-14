import random
import iptunnels_pb2

def jv_encode_iptunnels(jnpr, index, lc_slot, system_params):
    iptunnels_stats = jnpr.Extensions[iptunnels_pb2.jnpr_iptunnels_statistics_ext]
    record    = iptunnels_stats.iptunnel_stats_records.add()
    tunnel_type = " gre";
    if (random.randint(1, 3) == 1):
        tunnel_type = " udp";
    record.name = "src = 100.100.100." + str(index) +  " dst = 200.200.200." + str(index) + tunnel_type;
    record.packets            = random.randint(1, 7000000);
    record.bytes              =  random.randint(1, 7000000);
    record.rpf_failed_packets = random.randint(1, 10);
    record.rpf_failed_bytes   = random.randint(1, 400);
