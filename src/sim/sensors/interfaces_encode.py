import logical_port_pb2

def jv_encode_interfaces(jnpr, index, lc_slot):
    message = jnpr.Extensions[logical_port_pb2.jnprLogicalInterfaceExt]
    if_stats = message.interface_info.add()
    if_stats.if_name = "xe-" + str(index)
    if_stats.init_time = 1
    
