# jVision Simulator (jvsim)

jvsim is a simulation of all the modules that participate in an jVision ecosystem. These are
* **Junos System** The following components of the system are simulated
  * Different flavors of sensors (interfaces, firewall, lsp stats)
  * Number of linecards
  * The structure of the system is specified by a configuration file (**config/system.json**)
* **gRPC Server**
  * This is the process that implements the telemetry interface
  * The RPC interface is specified by the Google RPC proto definition
* **gRPC Client**
  * A test client is included with this project. 
  * An interactive CLI is provided which enables invoking various jVision RPCs


# Docker installation
Since jvsim has dependencies on several open source components (like gRPC, protocol buffers), it is easier to run the simulator as a docker container, where all these packages are installed as part of the container creation. The following simple steps will enable this
* Install docker on your server [Skip if already installed] "https://docs.docker.com/engine/installation/linux/ubuntulinux/"
* Copy jvsim docker file  [https://github.com/nkumar43212/jvsim/config/jvsim_docker]
* docker build  -t jvsim_cont -f jvsim_docker .
* docker run --name jvsim_cont -t -i jvsim_cont  /bin/bash
* Inside the container 
  * cd /home/jvsim
  * setenv ROOTPATH=/home/jvsim
  * make
  * bin/jvsim  -s -t  [This runs the server (-s) and the test harness (-t)]

# jVsim Runtime Options
    bin/jvsim -u
        usage: jvtest [-u] [-k] [-s] [-c] [-t]
          -k  : cleanup and exit
          -s  : run server
          -c  : run interactive client
          -t  : run test harness

 

  




