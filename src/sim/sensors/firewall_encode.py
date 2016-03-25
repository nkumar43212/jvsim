import firewall_pb2
import time
import calendar

def jv_encode_firewall(jnpr, index, lc_slot, system_params):
    firewall = jnpr.Extensions[firewall_pb2.jnpr_firewall_ext]
    fw_stats = firewall.firewall_stats.add()
    fw_stats.filter_name = "filter-" + str(index)
    fw_stats.timestamp   = calendar.timegm(time.gmtime())
    mem = fw_stats.memory_usage.add();
    mem.name = "mem1";
    mem.allocated = 1000;
    max_cnts = 10
    c = 0
    for c in range(0, max_cnts):
        cnt = fw_stats.counter_stats.add();
        cnt.name = "cnt-" + str(c);
        cnt.packets = 1000;
        cnt.bytes   = 6000;
   
