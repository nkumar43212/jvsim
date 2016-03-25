//
//  main.cpp
//  mgd_server
//
//  Created by NITIN KUMAR on 3/22/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "AgentServerLog.hpp"
#include <iostream>
#include <string>
#include <grpc++/grpc++.h>
#include "MgdServer.hpp"

void
RunServer (AgentServerLog *logger)
{
    std::string server_address("0.0.0.0:50050");
    MgdServer service(logger);
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


int main(int argc, const char * argv[])
{
    // Create a logger
    AgentServerLog *logger;
    logger = new AgentServerLog(argv[1]);
    logger->enable();
    
    // Start the server
    RunServer(logger);
}
