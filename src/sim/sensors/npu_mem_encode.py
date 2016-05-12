import npu_mem_pb2

def jv_encode_npu_mem(jnpr, index, lc_slot, system_params):
    npu = jnpr.Extensions[npu_mem_pb2.npu_memory_ext]

    # Per PFE statistics
    x = 0
    for x in range(0, system_params['pfe_count']):
        npu_stats = npu.memory_stats.add();
        npu_stats.identifier = "FPC" + str(lc_slot) + ":NPU" + str(x);

        #Summary stats
        summary_stats = npu_stats.summary.add();
        summary_stats.resource_name = "idmem";
        summary_stats.size       = 70 + x;
        summary_stats.allocated  = 100 + x;
        summary_stats.utilization=x;


        summary_stats = npu_stats.summary.add();
        summary_stats.resource_name = "odmem";
        summary_stats.size       = 50 + x;
        summary_stats.allocated  = 500 + x;
        summary_stats.utilization=4*x;

        summary_stats = npu_stats.summary.add();
        summary_stats.resource_name = "dmem";
        summary_stats.size       = 70 + x;
        summary_stats.allocated  = 100 + x;
        summary_stats.utilization=x;

        summary_stats = npu_stats.summary.add();
        summary_stats.resource_name = "ldmem";
        summary_stats.size       = 70 + x;
        summary_stats.allocated  = 100 + x;
        summary_stats.utilization=x;

        #Partition Stats
        part_stats = npu_stats.partition.add();
        part_stats.name = "NH";
        part_stats.application_name = "IFL";
        part_stats.bytes_allocated  = 100 + x;

        part_stats = npu_stats.partition.add();
        part_stats.name = "DFW";
        part_stats.application_name = "FILTER-1";
        part_stats.bytes_allocated  = 100 + x;

        part_stats = npu_stats.partition.add();
        part_stats.name = "NH";
        part_stats.application_name = "RT";
        part_stats.bytes_allocated  = 100 + x;
