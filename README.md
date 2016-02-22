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
  * setenv ROOTPATH=/home/jvsim
  * make
  * bin/jvsim  -s -t  [This runs the server (-s) and the test harness (-t)]

# jVsim Runtime Options
    bin/jvsim -u
        usage: jvsim [-u] [-k] [-s] [-c] [-t]
          -k  : cleanup and exit
          -s  : run server
          -c  : run interactive client
          -t  : run test harness
          
# Layout
Important directories in the project are 
* **protos/** includes all the .proto files used by the simulation. These include the native gpb encoding of the sensor data as well as the RPC interface exposed by the server
* **src/grpc/server** is the implementation of the gRPC server
* **src/grpc/client** is the interactive client which can be used to explore the RPC interface
* **src/grpc/test** is the test harness
* **src/sim** is the simulation of all the data sensors that produce the simulated telemetry data
* logs/ will have the runtime logs produced by different components. Some important files to look out for 
  * **agent_server.log** Produced by the server
  * **jv_test_mosquitto.log** The mosquitto broker is run in a verbose mode. This file has all the logs from the broker process 
  * **port.N, firewall.N**, lsp_stats.N are the logs for each of the sensors. There is a separate log file for each linecard slot (N)

 

  




