import sys
from test_encode       import jv_encode_test;
from firewall_encode   import jv_encode_firewall;
from port_encode       import jv_encode_port;
from lsp_stats_encode  import jv_encode_lsp_stats;
from interfaces_encode import jv_encode_interfaces;
from packet_stats_encode     import jv_encode_packet_stats;
from npu_util_encode   import jv_encode_npu_util;
from iptunnels_encode  import jv_encode_iptunnels;

def jv_encode(name, jnpr, index, lc_slot, system_params):
    dispatch[name](jnpr, index, lc_slot, system_params);

dispatch = {
    'test':         jv_encode_test,
    'port':         jv_encode_port,
    'firewall':     jv_encode_firewall,
    'lsp_stats':    jv_encode_lsp_stats,
    'interfaces':   jv_encode_interfaces,
    'packet_stats': jv_encode_packet_stats,
    'npu_util':     jv_encode_npu_util,
    'iptunnels':    jv_encode_iptunnels,
}
