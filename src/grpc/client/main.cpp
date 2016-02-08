//
//  main.cpp
//  agent-jv-client
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "AgentClient.hpp"
#include "AgentClientParser.hpp"

extern uint32_t global_id;


int main(int argc, const char * argv[])
{
    // A well known Management Client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                        mgmt_client_name, global_id++);

    // Run a simple command line loop to test all
    AgentParser parser;
    parser.parseLoop();
    
    return 0;
}


