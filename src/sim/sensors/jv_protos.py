import sys
from test_encode       import jv_encode_test;
from firewall_encode   import jv_encode_firewall;
from port_encode       import jv_encode_port;
from lsp_stats_encode  import jv_encode_lsp_stats;
from interfaces_encode import jv_encode_interfaces;

def jv_encode(name, jnpr, index):
    dispatch[name](jnpr, index);

dispatch = {
    'test':       jv_encode_test,
    'port':       jv_encode_port,
    'firewall':   jv_encode_firewall,
    'lsp_stats':  jv_encode_lsp_stats,
    'interfaces': jv_encode_interfaces
}
