import cpu_memory_utilization_pb2

def jv_encode_cpu_mem(jnpr, index, lc_slot, system_params):
    cpu = jnpr.Extensions[cpu_memory_utilization_pb2.cpu_memory_util_ext]

    cpu_stats = cpu.utilization.add();
    cpu_stats.name = "kernel";
    cpu_stats.size = 10000;
    cpu_stats.bytes_allocated = 1000;
    cpu_stats.utilization = 50;

    #Add Partitions
    app = cpu_stats.application_utilization.add();
    app.name = "nh";
    app.bytes_allocated = 10;
    app.allocations_failed = 100;
    app.frees = 4;

    app = cpu_stats.application_utilization.add();
    app.name = "dfw";
    app.bytes_allocated = 10;
    app.allocations_failed = 100;
    app.frees = 4;

    app = cpu_stats.application_utilization.add();
    app.name = "ifl";
    app.bytes_allocated = 10;
    app.allocations_failed = 100;
    app.frees = 4;

    cpu_stats = cpu.utilization.add();
    cpu_stats.name = "packets";
    cpu_stats.size = 10000;
    cpu_stats.bytes_allocated = 1000;
    cpu_stats.utilization = 50;

    #Add Partitions
    app = cpu_stats.application_utilization.add();
    app.name = "nh";
    app.bytes_allocated = 10;
    app.allocations_failed = 100;
    app.frees = 4;

    app = cpu_stats.application_utilization.add();
    app.name = "dfw";
    app.bytes_allocated = 10;
    app.allocations_failed = 100;
    app.frees = 4;

    app = cpu_stats.application_utilization.add();
    app.name = "ifl";
    app.bytes_allocated = 10;
    app.allocations_failed = 100;
    app.frees = 4;

