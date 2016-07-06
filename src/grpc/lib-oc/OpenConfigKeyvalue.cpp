//
//  OpenConfigKeyvalue.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigKeyvalue.hpp"
#include "oc.hpp"

void
OpenConfigKeyvalue::iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap)
{
    kvresponse *message = handle->MutableExtension(kvresponseExt);
    int i;
    telemetry::KeyValue *kv;

    for (i = 0; i < message->kvpairs_size(); i++) {
        const keyvalue &kvpair = message->kvpairs(i);

        kv = datap->add_kv();
        kv->set_key(kvpair.key());
        if (kvpair.string_val() != "") {
            kv->set_str_value(kvpair.string_val());
            continue;
        }
        if (kvpair.int32_val()) {
            kv->set_int_value(kvpair.int32_val());
            continue;
        }
        if (kvpair.uint64_val()) {
            kv->set_uint_value(kvpair.uint64_val());
            continue;
        }
    }
}
