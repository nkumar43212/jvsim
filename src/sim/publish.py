import argparse
import time
import paho.mqtt.client as paho
import os
import sys
from time import gmtime, strftime
from jvision_encode import generate_message


#Set up the execution parameters
parser = argparse.ArgumentParser()
parser.add_argument("linecard")
parser.add_argument("resource")
parser.add_argument("max_count")
parser.add_argument("export_interval");
parser.add_argument("logfile");
parser.add_argument("system_id");
args = parser.parse_args()

linecard_str    = "lc" + args.linecard;
linecard_slot   = int(args.linecard);
resource_str    = args.resource;
resource_cnt    = int(args.max_count);
export_interval = args.export_interval;
logfile         = args.logfile
system_id       = args.system_id;


#Send all output to file
sys.stdout = open(logfile, 'w')

print("linecard = ", linecard_str);
print("resource = ", resource_str);
print("count    = ", resource_cnt);
print("interval = ", export_interval);
print("sys      = ", system_id);
sys.stdout.flush()

# Details for the Broker
broker = "127.0.0.1"
port   = 1883

#MQTT callbacks
def on_connect(client, userdata, rc):
    if rc == 0:
    #rc 0 successful connect
        print("mqtt-connect");
        sys.stdout.flush()
    else:
        raise Exception


def on_publish(mosq, userdata, mid):
    print("mqtt-publish");
    sys.stdout.flush()

#create an mqtt client
mypid       = os.getpid()
client_uniq = "publisher-" + linecard_str + "-" + resource_str + "-" + str(mypid)
#mqttc       = mosquitto.Mosquitto(client_uniq)
mqttc       = paho.Client()

#attach MQTT callbacks
mqttc.on_connect = on_connect
#mqttc.on_publish = on_publish

#connect to broker
mqttc.connect(broker, port, 60)

#Track packet and byte counts
last_packets = 0
last_bytes   = 0
packets = 0
bytes   = 0

#Start the clock
start = time.time()
reset_value = 1
timer = reset_value
elapsed = 0
sequence_number = 1

#remain connected to broker
while mqttc.loop() == 0:
        topic = resource_str

        #Generate a message
        frame = bytearray()
        sequence_number += 1
        generate_message(frame, resource_str, resource_cnt, linecard_slot, system_id, sequence_number)
       

        #Send it over to the bus
        mqttc.publish(topic, frame)

        #Update stats
        packets += 1
        bytes   += len(frame)

        #Compute rate
        timer -= 1
        rate = 0
        if timer == 0:
            elapsed = time.time() - start
            start   = time.time()
            rate    = (packets - last_packets) / elapsed
            rate_b  = (bytes - last_bytes) / elapsed

            last_packets = packets
            last_bytes   = bytes
            timer   = reset_value

            #print("", strftime("%Y-%m-%d %H:%M:%S", gmtime()));
            #print("PPS = , BPS = , delta =  ", rate, rate_b, elapsed);
            sys.stdout.flush()


        #Sleep for the desired interval
        time.sleep(float(export_interval))
        pass

