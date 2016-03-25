import npu_util_pb2

def jv_encode_npu_util(jnpr, index, lc_slot, system_params):
    npu = jnpr.Extensions[npu_util_pb2.jnpr_npu_util_ext]

    # Per PFE statistics
    x = 0
    for x in range(0, system_params['pfe_count']):
        npu_stats = npu.npu_util_stats.add();
        npu_stats.identifier = "pfe" + str(lc_slot) + "/" + str(x);
        npu_stats.utilization = 50 + x;

        #Packet loads for different memories
        packet_stats = npu_stats.packets.add();
        packet_stats.identifier = "quad1";
        packet_stats.rate       = 70 + x;
        packet_stats = npu_stats.packets.add();
        packet_stats.identifier = "quad2";
        packet_stats.rate       = 80 + x;
        packet_stats = npu_stats.packets.add();
        packet_stats.identifier = "quad3";
        packet_stats.rate       = 90 + x;
        packet_stats = npu_stats.packets.add();
        packet_stats.identifier = "quad4";
        packet_stats.rate       = 40 + x;

        #Memory Load
        mem_stats    = npu_stats.memory.add();
        mem_stats.name = "mem0";
        mem_stats.average_util = 40 + x;
        mem_stats.highest_util = 40 + x;
        mem_stats.lowest_util  = 40 + x;

        mem_stats    = npu_stats.memory.add();
        mem_stats.name = "mem1";
        mem_stats.average_util = 50 + x;
        mem_stats.highest_util = 50 + x;
        mem_stats.lowest_util  = 50 + x;

        mem_stats    = npu_stats.memory.add();
        mem_stats.name = "mem2";
        mem_stats.average_util = 60 + x;
        mem_stats.highest_util = 60 + x;
        mem_stats.lowest_util  = 60 + x;

        mem_stats    = npu_stats.memory.add();
        mem_stats.name = "mem3";
        mem_stats.average_util = 70 + x;
        mem_stats.highest_util = 70 + x;
        mem_stats.lowest_util  = 70 + x;
 
