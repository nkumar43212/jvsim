import test_pb2

def jv_encode_test(jnpr, index, lc_slot):
    test = jnpr.Extensions[test_pb2.jnpr_test_ext]
    test_stats = test.test_stats.add()
    test_stats.id = index;
    test_stats.name = "test-" + str(index)
