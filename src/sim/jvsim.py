import argparse
import time
import os
import sys
import json

#Set up the execution parameters
parser = argparse.ArgumentParser()
parser.add_argument("model");
parser.add_argument("logdir");
parser.add_argument("srcdir");
args = parser.parse_args()

#Read the system model
with open(args.model) as json_file:
   sys_data = json.load(json_file)

#Get basic system parameters
max_lcs   = int(sys_data["system"]["infrastructure"]["linecard"])
system_id = sys_data["system"]["infrastructure"]["id"]
max_pfes  = int(sys_data["system"]["infrastructure"]["pfe_count"])

#Set environment
os.system("echo $PYTHONPATH");

#Accumulate all the resources in the system model
sensors = sys_data["system"]["sensors"]

sensor_list = []
for attribute, value in sensors.items():
   sensor_list.append(attribute)

print(sensor_list)

python_cmd = "python3"
publish_cmd = args.srcdir + "/publish.py "

for lc in range(0, max_lcs):
    for res in sensor_list:
        sensors   = sys_data["system"]["sensors"];
        resource  = sensors[res]["resource"];
        max_count = sensors[res]["count"];
        interval  = sensors[res]["interval"];
        logfile   = args.logdir + "/" + resource + "." + str(lc);
        cmd       = python_cmd + " " + publish_cmd + str(lc) + " " + resource + " " + max_count + " " + interval + " " + logfile + " " +  system_id + " " + str(max_pfes) + "&"
        print(cmd)
        os.system(cmd);
