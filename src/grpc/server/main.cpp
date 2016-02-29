//
//  main.cpp
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "AgentServer.h"
#include "AgentServerCmdOptions.hpp"
#include "AgentSystemFactory.hpp"
#include "OpenConfig.hpp"
#include "AgentSubscription.hpp"
#include "lib_oc.h"

void
RunServer (AgentServerLog *logger, AgentSystem *sys_handle)
{
    std::string server_address("0.0.0.0:50051");
    AgentServer service(logger, sys_handle);
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

AgentSystem *
CreateSystemHandle (AgentServerCmdOptions *cmd_options,
                    AgentServerLog *logger)
{
    AgentSystem *sys_handle;
    
    // Now look at the system mode provided
    if (cmd_options->isSystemModeFile()) {
        sys_handle = AgentSystemFactory::createFile(logger, cmd_options->getSystemFileName());
    } else if (cmd_options->isSystemModeProc()) {
        sys_handle = AgentSystemFactory::createSystemProcess(logger);
    } else {
        // Default Mode
        sys_handle = AgentSystemFactory::createNull(logger);
    }

    return sys_handle;
}

int
main (int argc, char * argv[])
{
    // Get all command line options
    AgentServerCmdOptions opts;
    if (!opts.parseArgs(argc, argv)) {
        return -1;
    }
    
    // Initialize interface with Mosquitto Library
    mosqpp::lib_init();

    // Create a logger
    AgentServerLog *logger;
    logger = new AgentServerLog(opts.getLogFile());
    logger->enable();
    
    // Initialize all the oc translators
    lib_oc_init();
    OpenConfig::display(logger);
    
    // Create a handle for the system
    AgentSystem *sys_handle = CreateSystemHandle(&opts, logger);
    
    // Start the server
    RunServer(logger, sys_handle);
}
