//
//  AgentClientParser.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/22/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentClient.hpp"
#include "AgentClientParser.hpp"
#include "AgentClientLag.hpp"

// Subscription ID
uint32_t global_id;

std::string CLIENT_LOGDIR("/Users/nitin/jvsim/logs");

void
handle_subscribe (int argc, const char *argv[])
{
    std::string client_name(argv[1]);
    AgentClient *client;
    
    // Create a client
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 client_name,
                                 global_id++,
                                 CLIENT_LOGDIR);
    
    // collect the list of paths
    std::vector<std::string> path_list;
    for (int i = 2; argv[i]; i++) {
        path_list.push_back(argv[i]);
    }
    
    client->subscribeTelemetry(path_list);
}

void *
proc (void *args)
{
    CommandContext *context = (CommandContext *) args;
    const char **argv = context->getArgv();
    static uint64_t some_id = 0;
    std::string client_name(argv[2] + std::to_string(some_id++));
    AgentClient *client;
    
    // Create a client
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 client_name, global_id++, CLIENT_LOGDIR);
    
    // collect the list of paths
    std::vector<std::string> path_list;
    for (int i = 3; argv[i]; i++) {
        path_list.push_back(argv[i]);
    }
    
    client->subscribeTelemetry(path_list);
    return NULL;
}

void
handle_subscribe_multiple (int argc, const char *argv[])
{
    uint32_t       count = atoi(argv[1]);
    pthread_t     *tid;
    CommandContext context(argc, argv, NULL);
    
    // Create N threads
    tid = new pthread_t[count];
    for (int i = 0; i < count; i++) {
        pthread_create(&tid[i], NULL, proc, (void *) &context);
    }
    
    // Wait for them to finish
    for (int i = 0; i < count; i++) {
        pthread_join(tid[i], NULL);
    }
}

void
handle_subscribe_limits (int argc, const char *argv[])
{
    std::string client_name(argv[1]);
    AgentClient *client;
    
    // Create a client
    client = AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                                 client_name, global_id++, CLIENT_LOGDIR);
    
    // collect the list of paths
    std::vector<std::string> path_list;
    for (int i = 2; argv[i]; i++) {
        path_list.push_back(argv[i]);
    }
    
    client->setDebug(true);
    client->subscribeTelemetry(path_list, 10, 10);
}

void
handle_unsubscribe (int argc, const char *argv[])
{
    AgentClient *client;
    std::string client_name((const char *) argv[1]);
        
    // Find this client
    client = AgentClient::find(client_name);
    if (!client) {
        std::cout << "Failed to find client:" << client_name << "\n";
        return;
    }
        
    client->unSubscribeTelemetry();
    delete client;
}

void
handle_print (int argc, const char *argv[])
{
    AgentClient::print();
}

void
handle_list (int argc, const char *argv[])
{
    AgentClient *mgmt = AgentClient::find(AGENTCLIENT_MGMT);
    mgmt->listSubscriptions(argv[1] ? atoi(argv[1]) : 1);
}

void
handle_get (int argc, const char *argv[])
{
    AgentClient *client;
    std::string client_name((const char *) argv[1]);
        
    // Find this client
    client = AgentClient::find(client_name);
    if (!client) {
        std::cout << "Failed to find client:" << client_name << "\n";
        return;
    }
        
    // Verbosity level
    uint32_t verbosity = argv[2] ? atoi(argv[2]) : 0;
    client->getOperational(verbosity);
}

void
handle_build_lag (int argc, const char *argv[])
{
    AgentLag::build(atoi(argv[1]), atoi(argv[2]));
    AgentLag::print();
}

void
handle_clean_lag (int argc, const char *argv[])
{
    AgentLag::reset();
}

void
handle_print_lag (int argc, const char *argv[])
{
    AgentLag *lagp = AgentLag::findLag(argv[1]);
    if (!lagp) {
        std::cout << "Could not find LAG = " << argv[1] << "\n";
        return;
    }

    uint32_t repeat_count = argv[2] ? atoi(argv[2]) : 1;
    
    for (int i = 0; i < repeat_count; i++) {
        lagp->printMembers();
        sleep(5);
    }
}

// Add new commands here
entry_t agent_client_commands [] = {    
    {
        .e_cmd     = std::string("subscribe"),
        .e_argc    = 3,
        .e_help    = std::string("Subscribe to a jvision sensor by specifying a list of paths"),
        .e_usage   = std::string("subscribe <subscription-name> <path>+"),
        .e_handler = handle_subscribe
    },
    
    {
        .e_cmd     = std::string("subscribe_limits"),
        .e_argc    = 3,
        .e_help    = std::string("Subscribe with a limit to the session"),
        .e_usage   = std::string("subscribe_limits <subscription-name> <path>+"),
        .e_handler = handle_subscribe_limits
    },
   
    {
        .e_cmd     = std::string("subscribe_n"),
        .e_argc    = 3,
        .e_help    = std::string("Create N subscriptions"),
        .e_usage   = std::string("subscribe_n <count> <subscription-name> <path>+"),
        .e_handler = handle_subscribe_multiple
    },

    {
        .e_cmd     = std::string("unsubscribe"),
        .e_argc    = 2,
        .e_help    = std::string("Unsubscribe from an existing request by specifying the subscription ID"),
        .e_usage   = std::string("unsubscribe <subscription-name>"),
        .e_handler = handle_unsubscribe
    },
    
    {
        .e_cmd     = std::string("list"),
        .e_argc    = 1,
        .e_help    = std::string("Get all active subscriptions from the server"),
        .e_usage   = std::string("list <verbosity>*"),
        .e_handler = handle_list
    },
    
    {
        .e_cmd     = std::string("get"),
        .e_argc    = 1,
        .e_help    = std::string("Get operational state from the server"),
        .e_usage   = std::string("get <verbosity>*"),
        .e_handler = handle_get
    },
    
    {
        .e_cmd     = std::string("print"),
        .e_argc    = 1,
        .e_help    = std::string("Print all subscriptions known to the client"),
        .e_usage   = std::string("subscribe <subscription-name> <path>+"),
        .e_handler = handle_print
    },
    
    {
        .e_cmd     = std::string("build-lag"),
        .e_argc    = 3,
        .e_help    = std::string("Build the LAG database"),
        .e_usage   = std::string("build-lag <total-lag-count> <number-of-lcs>"),
        .e_handler = handle_build_lag
    },
   
    {
        .e_cmd     = std::string("clean-lag"),
        .e_argc    = 1,
        .e_help    = std::string("Clean the LAG database"),
        .e_usage   = std::string("clean-lag"),
        .e_handler = handle_clean_lag
    },

    {
        .e_cmd     = std::string("print-lag"),
        .e_argc    = 2,
        .e_help    = std::string("Print Stats for given LAG"),
        .e_usage   = std::string("print-lag <name> <repeat-count>"),
        .e_handler = handle_print_lag
    },

};

u_int32_t agent_client_commands_count = sizeof(agent_client_commands)/sizeof(entry_t);

