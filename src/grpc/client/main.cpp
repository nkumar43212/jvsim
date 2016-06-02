//
//  main.cpp
//  agent-jv-client
//
//  Created by NITIN KUMAR on 12/29/15.
//  CoAuthors: ABBAS SAKARWALA
//
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <sys/stat.h>
#include <iostream>
#include <cstdlib>
#include "AgentClient.hpp"
#include "AgentClientParser.hpp"

// :-/ globals
extern uint32_t global_id;
AgentParser *parser;
std::string server_ip("localhost");
std::string server_port("50051");

int main(int argc, const char * argv[])
{
    std::string logfile_dir;

    // Get the Looging dir
    if (argc > 1) {
        logfile_dir = argv[1];
        // Add a '/' in the end of directory name given
        if (logfile_dir.back() != '/') {
            logfile_dir.push_back('/');
        }
    } else {
        // Validate if logger file was specified
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            logfile_dir = (std::string)env_rp + "/logs/";
        } else {
            std::cerr << "Please setup ROOTPATH environment variable or run as"
                      << "\"client_binary logfile_dir\"" << std::endl;
            exit(0);
        }
    }
    // Validate if directory
    struct stat sb;
    if (!(stat(logfile_dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))) {
        std::cout << "Either ROOTPATH set or argument passed is not a "
                  << "valid directory = " << logfile_dir << std::endl;
        exit(0);
    }

    // A well known Management Client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    AgentClient::create(grpc::CreateChannel(AGENT_SERVER_IP_PORT,
                        grpc::InsecureCredentials()),
                        mgmt_client_name,
                        global_id++,
                        logfile_dir);

    // Run a simple command line loop to test all
    parser = new AgentParser(logfile_dir);
    parser->parseLoop();

    return 0;
}
