import os
import sys
import telemetry_top_pb2
import jv_protos
import google.protobuf.text_format
import time
import calendar
from jv_protos import jv_encode

#Generate a message
def generate_message(frame, sensor_type, max_count, lc_slot, system_id, sequence_number, system_params):
    telemetry_stream              = telemetry_top_pb2.TelemetryStream();
    telemetry_stream.system_id    = system_id
    telemetry_stream.component_id = lc_slot
    telemetry_stream.timestamp    = calendar.timegm(time.gmtime())
    telemetry_stream.sequence_number = sequence_number

    #Extend the juniper enterprise
    enterprise = telemetry_stream.enterprise
    jnpr       = enterprise.Extensions[telemetry_top_pb2.juniperNetworks]

    #Call the appropriate encoder
    x = 0
    for x in range(0, max_count):
        jv_encode(sensor_type, jnpr, x, lc_slot, system_params);

    #Serialize
    byte_stream = telemetry_stream.SerializeToString()
    print_message(byte_stream)

    i = 0
    while True:
        if i >= len(byte_stream):
            break
        frame.append(byte_stream[i])
        i = i + 1

def print_message(byte_stream):
    print("-------------");
    tstream = telemetry_top_pb2.TelemetryStream();
    tstream.ParseFromString(byte_stream)
    print(google.protobuf.text_format.MessageToString(tstream))
    print("Message Size = %u\n" % (tstream.ByteSize()));
    print("-------------");
    sys.stdout.flush()


