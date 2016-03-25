import random
import port_pb2

def jv_encode_port(jnpr, index, lc_slot, system_params):
    port_stats = jnpr.Extensions[port_pb2.jnpr_interface_ext]
    port = port_stats.interface_stats.add()
    port.if_name = "xe-" + str(lc_slot) + "/0/" + str(index);
    port.parent_ae_name = "ae" + str(index)
    port.snmp_if_index = index
    port.init_time = 100
    port.egress_stats.if_pkts   = 10;
#    port.egress_stats.if_pkts   = random.randint(1, 7000000);
    port.egress_stats.if_octets = random.randint(1, 1000000);
    port.egress_stats.if_1sec_pkts   = 0;
    port.egress_stats.if_1sec_octets = 0;
    port.egress_stats.if_uc_pkts = random.randint(1, 7000000);
    port.egress_stats.if_mc_pkts = random.randint(1, 7000000); 
    port.egress_stats.if_bc_pkts = random.randint(1, 7000000); 
    port.ingress_stats.if_pkts   = random.randint(1, 7000000);
    port.ingress_stats.if_octets = random.randint(1, 1000000);
    port.ingress_stats.if_1sec_pkts   = 0;
    port.ingress_stats.if_1sec_octets = 0;
    port.ingress_stats.if_uc_pkts = random.randint(1, 7000000);
    port.ingress_stats.if_mc_pkts = random.randint(1, 7000000); 
    port.ingress_stats.if_bc_pkts = random.randint(1, 7000000); 
