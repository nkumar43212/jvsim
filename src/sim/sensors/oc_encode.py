import keyvalue_pb2

def jv_encode_oc(jnpr, index, lc_slot, system_params):
    oc           = jnpr.Extensions[keyvalue_pb2.kvresponseExt]

    record       = oc.kvpairs.add()
    record.key   = "/oc_path/foo/bar/hello"
    record.string_val = "AAAA";

    record       = oc.kvpairs.add()
    record.key   = "/oc_path/foo/bar/jamesbond"
    record.string_val = "007";

    record       = oc.kvpairs.add()
    record.key   = "/oc_path/foo/bar/drno"
    record.int32_val = -1

    record       = oc.kvpairs.add()
    record.key   = "/oc_path/foo/bar/gold"
    record.uint64_val = 9999
