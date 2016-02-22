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
 

  




