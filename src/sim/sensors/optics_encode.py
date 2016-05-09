import random
import optics_pb2

def jv_encode_optics(jnpr, index, lc_slot, system_params):
    optics_stats = jnpr.Extensions[optics_pb2.jnpr_optics_ext]
    optics = optics_stats.Optics_diag.add()
    optics.if_name = "xe-" + str(lc_slot) + "/0/" + str(index);
    optics.optics_diag_stats.module_temp   = 10;
