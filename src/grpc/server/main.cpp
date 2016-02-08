//
//  main.cpp
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "AgentServer.h"
#include "OpenConfig.hpp"
#include "AgentSubscription.hpp"
#include "lib_oc.h"

void RunServer (AgentServerLog *logger)
{
    std::string server_address("0.0.0.0:50051");
    AgentServer service(logger);
    ServerBuilder builder;
    
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}


int main (int argc, const char * argv[])
{
    // Initialize interface with Mosquitto Library
    mosqpp::lib_init();

    // Create a logger
    AgentServerLog *logger;
    if (argv[1]) {
        logger = new AgentServerLog(argv[1]);
    } else {
        logger = new AgentServerLog;
    }
    logger->enable();
    
    // Initialize all the oc translators
    lib_oc_init();
    OpenConfig::display(logger);
    
    // Start the server
    RunServer(logger);
}
