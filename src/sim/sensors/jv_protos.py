import sys
from test_encode       import jv_encode_test;
from firewall_encode   import jv_encode_firewall;
from port_encode       import jv_encode_port;
from logical_port_encode import jv_encode_logical_port;
from lsp_stats_encode  import jv_encode_lsp_stats;
from packet_stats_encode     import jv_encode_packet_stats;
from npu_utilization_encode  import jv_encode_npu_utilization;
from iptunnels_encode  import jv_encode_iptunnels;
from npu_mem_encode    import jv_encode_npu_mem;
from cpu_mem_encode    import jv_encode_cpu_mem;

def jv_encode(name, jnpr, index, lc_slot, system_params):
    dispatch[name](jnpr, index, lc_slot, system_params);

dispatch = {
    'test':                jv_encode_test,
    'firewall':            jv_encode_firewall,
    'port':                jv_encode_port,
    'logical_port':        jv_encode_logical_port,
    'lsp_stats':           jv_encode_lsp_stats,
    'packet_stats':        jv_encode_packet_stats,
    'npu_utilization':     jv_encode_npu_utilization,
    'iptunnels':           jv_encode_iptunnels,
    'npu_mem':             jv_encode_npu_mem,
    'cpu_mem':             jv_encode_cpu_mem,
}
