# jVision Simulator (jvsim)

jvsim is a simulation of all the modules that participate in a jVision ecosystem. These are
* **Network Device** The following components of the device are simulated
  * linecards (Simulated processes mimic the behavior of linecards in a real chassis)
  * sensors (e.g interfaces, firewall, lsp stats)
  * The system is modelled using a configuration file (**config/system.json**)
* **gRPC Server**
  * The process that implements the telemetry interface
  * The interface is specified by a Google RPC service
* **gRPC Client**
  * A test client is included with this project. 
  * An interactive CLI is provided which enables invoking various jVision RPCs


# Docker installation
Since jvsim has dependencies on several open source components (like gRPC, protocol buffers), it is easier to run the simulator as a docker container, where all these packages are installed as part of the container creation. The following simple steps will enable this
* Install docker on your server [Skip if already installed] "https://docs.docker.com/engine/installation/linux/ubuntulinux/"
* Copy jvsim docker file  [https://github.com/nkumar43212/jvsim/blob/master/config/jvsim_docker]
* docker build  -t jvsim_cont -f jvsim_docker .
* docker run --name jvsim_cont -t -i jvsim_cont  /bin/bash
* Inside the container 
  * cd /home/jvsim
  * export ROOTPATH=/home/jvsim
  * make
  * bin/jvsim  -s -t  [This runs the server (-s) and the test harness (-t)]

# jVsim Runtime Options
    bin/jvsim -u
        usage: jvsim [-u] [-k] [-s] [-c] [-m] [-d]
         -k  : cleanup and exit
         -s  : run server
         -m  : run simulation management daemon server
         -c  : run interactive client
         -t  : run test harness
         -d  : run data streamers

# Running gRPC server and client to enable telemetry on Junos device (non-jVsim mode):
* Start Mosquitto broker on Junos device.  Mosquitto broker can be started with below config on Junos device.  
  > set system services extension-service notification allow-clients address 0.0.0.0/0
* Edit config/agent_server.ini  
  [junos-device]  
  device_mgd_ip       = w.x.y.z           ; IP address of JUNOS device  
  device_mgd_port     = 50060             ; Port of MGD Lego API service  

  [mqtt]  
  mqtt_broker_ip      = w.x.y.z           ; IP address of MQTT broker  

  [other-knobs]
  system_mode         = proc              ; Valid options: null, file, proc  
* Start the server  
  > bin/linux/agent_server -c config/agent_server.ini &
* Start the client  
  > bin/linux/agent_client  
  * jvsim> help
  * jvsim> subscribe <subscription-name> <sample-frequency> <path>+  
    e.g.: jvsim> subscribe abbas-test 1 /junos/system/linecard/cpu/memory/ /junos/system/linecard/npu/memory/
* Telemetry data is dumped in client log file. Check in new shell prompt  
  > tail -f abbas-test  

    0:Logging enabled --  

    1:Message Size = 14630  
    2:system_id: "sdn-st-mx480-a:0.0.0.0"  
    component_id: 1  
    path: "/1000"  
    timestamp: 1462906772655  
    kv {  
      key: "oc-path/cpu-memory-utilization/Kernel/size"  
      int_value: 1864553756  
    }  

# Layout
Important directories in the project are 
* **protos/** includes all the .proto files used by the simulation. 
  * Native GPB based encoding used by linecard sensors
  * Encodings used in the gRPC service. 
* **src/grpc/server** is the implementation of the gRPC server
* **src/grpc/client** is an interactive client which can be used to explore the RPC interface
* **src/grpc/mgd_server** is the simulation of JUNOS mgd lego API implementation (GRPC)
* **src/grpc/test** is a test harness where select workflows are automated into gtest based unit tests
* **src/sim** is the simulation of all the data sensors that produce simulated telemetry data
* **logs/** will have the runtime logs produced by different components. Some important files to look out for 
  * **agent_server.log** Produced by the server
  * **jv_test_mosquitto.log** The mosquitto broker is run in a verbose mode. This file has all the logs from the broker process 
  * **port.N, firewall.N, lsp_stats.N** are the logs for each of the sensors. There is a separate log file for each linecard slot (N)

