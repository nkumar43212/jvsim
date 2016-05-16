//
//  main.cpp
//  Junos MGD
//
//  Created by NITIN KUMAR on 3/22/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include <string>
#include <grpc++/grpc++.h>
#include "MgdServer.hpp"

void
RunServer (AgentServerLog *logger)
{
    std::string server_address("0.0.0.0:50050");
    MgdServer service_mgd(logger);
    LoginServer service_login(logger);
    ServerBuilder builder;

    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service_mgd);
    builder.RegisterService(&service_login);

    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, const char * argv[])
{
    std::string logfile;

    // Get the Looging dir
    if (argc > 1) {
        logfile = argv[1];
    } else {
        // Validate if logger file was specified
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            logfile = (std::string)env_rp + "/logs/mgd_server.log";
        } else {
            std::cerr << "Please setup ROOTPATH environment variable or "
                         "run as \"mgd_server_binary logfile\"" << std::endl;
            exit(0);
        }
    }

    // Create a logger
    AgentServerLog *logger;
    logger = new AgentServerLog(logfile);
    logger->enable();

    // Start the server
    RunServer(logger);
}