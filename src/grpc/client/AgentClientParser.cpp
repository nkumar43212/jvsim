//
//  AgentClientParser.cpp
//  agent-jv-client
//
//  Created by NITIN KUMAR on 1/22/16.
//  CoAuthors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <unistd.h>
#include <atomic>
#include "AgentClient.hpp"
#include "AgentClientParser.hpp"
#include "AgentClientLag.hpp"

// Subscription ID
uint32_t global_id;

// The global parser object
extern AgentParser *parser;

// Id for multiple subscription
std::atomic_int some_id(0);

void
handle_subscribe (int argc, const char *argv[])
{
    std::string client_name(argv[1]);
    AgentClient *client;

    // Find this client
    client = AgentClient::find(client_name);
    if (client != NULL) {
        std::cout << "Client already exist: " << client_name << std::endl;
        return;
    }

    // Create a client
    client = AgentClient::create(grpc::CreateChannel(AGENT_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 client_name,
                                 global_id++,
                                 parser->getLogDir());

    // Sample Frequency
    uint32_t sample_frequency = atoi(argv[2]);

    // collect the list of paths
    std::vector<std::string> path_list;
    for (int i = 3; argv[i]; i++) {
        path_list.push_back(argv[i]);
    }

    client->subscribeTelemetry(path_list, sample_frequency);
    delete client;
}

void
handle_subscribe_limits (int argc, const char *argv[])
{
    std::string client_name(argv[1]);
    AgentClient *client;

    // Find this client
    client = AgentClient::find(client_name);
    if (client != NULL) {
        std::cout << "Client already exist: " << client_name << std::endl;
        return;
    }

    // Create a client
    client = AgentClient::create(grpc::CreateChannel(AGENT_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 client_name,
                                 global_id++,
                                 parser->getLogDir());

    // Sample Frequency
    uint32_t sample_frequency = atoi(argv[2]);
    uint32_t limit_records = atoi(argv[3]);
    uint32_t limit_seconds = atoi(argv[4]);

    // collect the list of paths
    std::vector<std::string> path_list;
    for (int i = 5; argv[i]; i++) {
        path_list.push_back(argv[i]);
    }

    client->subscribeTelemetry(path_list, sample_frequency,
                               limit_records, limit_seconds);
    delete client;
}


void *
proc (void *args)
{
    CommandContext *context = (CommandContext *) args;
    const char **argv = context->getArgv();
    std::string client_name(argv[2] + std::to_string(some_id++));
    AgentClient *client;

    // Find this client
    client = AgentClient::find(client_name);
    if (client != NULL) {
        std::cout << "Client already exist: " << client_name << std::endl;
        return NULL;
    }

    // Create a client
    client = AgentClient::create(grpc::CreateChannel(AGENT_SERVER_IP_PORT,
                                 grpc::InsecureCredentials()),
                                 client_name,
                                 global_id++,
                                 parser->getLogDir());

    // Sample Frequency
    uint32_t sample_frequency = atoi(argv[3]);

    // collect the list of paths
    std::vector<std::string> path_list;
    for (int i = 4; argv[i]; i++) {
        path_list.push_back(argv[i]);
    }

    client->subscribeTelemetry(path_list, sample_frequency);
    delete client;
    return NULL;
}

void
handle_subscribe_multiple (int argc, const char *argv[])
{
    uint32_t      count = atoi(argv[1]);
    pthread_t     *tid;
    CommandContext context(argc, argv, NULL);

    // Create N threads
    tid = new pthread_t[count];
    for (int i = 0; i < count; i++) {
        pthread_create(&tid[i], NULL, proc, (void *) &context);
    }

    // Give few mins to subscribe all
    sleep(5);
    // Reset id
    some_id.store(0);

    // Wait for them to finish
    for (int i = 0; i < count; i++) {
        pthread_join(tid[i], NULL);
    }
}

void
handle_unsubscribe (int argc, const char *argv[])
{
    std::string client_name((const char *) argv[1]);
    AgentClient *client;

    if (strcmp(argv[1], AGENTCLIENT_MGMT) == 0) {
        std::cout << AGENTCLIENT_MGMT << " cannot be deleted" << std::endl;
        return;
    }

    // Find this client
    client = AgentClient::find(client_name);
    if (!client) {
        std::cout << "Failed to find client: " << client_name << std::endl;
        return;
    }

    client->cancelSubscribeTelemetry();
    // Leave the delete of client in the subscription thread
}

void
handle_unsubscribe_id (int argc, const char *argv[])
{
    uint32_t subscription_id = atoi(argv[1]);
    std::string client_name(AGENTCLIENT_MGMT);
    AgentClient *client;

    // Find this client
    client = AgentClient::find(client_name);
    if (!client) {
        std::cout << "Failed to find client: " << client_name << std::endl;
        return;
    }

    client->cancelSubscribeTelemetryId(subscription_id);
    // Leave the delete of client in the subscription thread
}

void
handle_list_all (int argc, const char *argv[])
{
    AgentClient *mgmt = AgentClient::find(AGENTCLIENT_MGMT);
    mgmt->listSubscriptions(0xFFFFFFFF);
}

void
handle_list (int argc, const char *argv[])
{
    AgentClient *mgmt = AgentClient::find(AGENTCLIENT_MGMT);
    mgmt->listSubscriptions(argv[1] ? atoi(argv[1]) : 0);
}

void
handle_get_oper_all (int argc, const char *argv[])
{
    Telemetry::VerbosityLevel verbosity = Telemetry::VerbosityLevel::DETAIL;
    // Verbosity level
    if (argv[1] != NULL) {
        if (strcmp(argv[1], "brief")) {
            verbosity = Telemetry::VerbosityLevel::BRIEF;
        } else if (strcmp(argv[1], "terse")) {
            verbosity = Telemetry::VerbosityLevel::TERSE;
        }
    }
    AgentClient *mgmt = AgentClient::find(AGENTCLIENT_MGMT);
    mgmt->getOperational(0xFFFFFFFF, verbosity);
}

void
handle_get_oper (int argc, const char *argv[])
{
    uint32_t subscription_id = (argv[1] ? atoi(argv[1]) : 0);
    if (subscription_id == 0) {
        std::cout << "Invalid Subscription id = 0" << std::endl;
        return;
    }

    Telemetry::VerbosityLevel verbosity = Telemetry::VerbosityLevel::DETAIL;
    // Verbosity level
    if (argv[2] != NULL) {
        if (strcmp(argv[1], "brief")) {
            verbosity = Telemetry::VerbosityLevel::BRIEF;
        } else if (strcmp(argv[1], "terse")) {
            verbosity = Telemetry::VerbosityLevel::TERSE;
        }
    }
    AgentClient *mgmt = AgentClient::find(AGENTCLIENT_MGMT);
    mgmt->getOperational(subscription_id, verbosity);
}

void
handle_get_oper_mgmt (int argc, const char *argv[])
{
    Telemetry::VerbosityLevel verbosity = Telemetry::VerbosityLevel::DETAIL;
    // Verbosity level
    if (argv[1] != NULL) {
        if (strcmp(argv[1], "brief")) {
            verbosity = Telemetry::VerbosityLevel::BRIEF;
        } else if (strcmp(argv[1], "terse")) {
            verbosity = Telemetry::VerbosityLevel::TERSE;
        }
    }
    AgentClient *mgmt = AgentClient::find(AGENTCLIENT_MGMT);
    mgmt->getOperational(0, verbosity);
}

void
handle_print (int argc, const char *argv[])
{
    AgentClient::print();
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
        .e_argc    = 4,
        .e_help    = std::string("Subscribe to a jvision sensor by specifying a list of paths"),
        .e_usage   = std::string("subscribe <subscription-name> <sample-frequency> <path>+"),
        .e_handler = handle_subscribe
    },

    {
        .e_cmd     = std::string("subscribe_limits"),
        .e_argc    = 6,
        .e_help    = std::string("Subscribe with record and second limits to the session"),
        .e_usage   = std::string("subscribe_limits <subscription-name> <sample-frequency> <record-limit> <second-limit> <path>+"),
        .e_handler = handle_subscribe_limits
    },

    {
        .e_cmd     = std::string("subscribe_n"),
        .e_argc    = 4,
        .e_help    = std::string("Create N subscriptions"),
        .e_usage   = std::string("subscribe_n <count> <subscription-name> <sample-frequency> <path>+"),
        .e_handler = handle_subscribe_multiple
    },

    {
        .e_cmd     = std::string("unsubscribe"),
        .e_argc    = 2,
        .e_help    = std::string("Unsubscribe an existing request by specifying the subscription name"),
        .e_usage   = std::string("unsubscribe <subscription-name>"),
        .e_handler = handle_unsubscribe
    },

    {
        .e_cmd     = std::string("unsubscribe_id"),
        .e_argc    = 2,
        .e_help    = std::string("Unsubscribe an existing request by specifying the subscription id"),
        .e_usage   = std::string("unsubscribe <subscription-id>"),
        .e_handler = handle_unsubscribe_id
    },

    {
        .e_cmd     = std::string("list-all"),
        .e_argc    = 1,
        .e_help    = std::string("Get all active subscriptions from the server"),
        .e_usage   = std::string("list-all"),
        .e_handler = handle_list_all
    },

    {
        .e_cmd     = std::string("list"),
        .e_argc    = 2,
        .e_help    = std::string("Get subscription id details from the server"),
        .e_usage   = std::string("list <subscription-id>"),
        .e_handler = handle_list
    },

    {
        .e_cmd     = std::string("get-oper-all"),
        .e_argc    = 1,
        .e_help    = std::string("Get all operational state from the server"),
        .e_usage   = std::string("get-oper-all <verbosity=terse,brief,detail>*"),
        .e_handler = handle_get_oper_all
    },

    {
        .e_cmd     = std::string("get-oper"),
        .e_argc    = 2,
        .e_help    = std::string("Get subscription id operational state from the server"),
        .e_usage   = std::string("get-oper <subscription-id> <verbosity=terse,brief,detail>*"),
        .e_handler = handle_get_oper
    },

    {
        .e_cmd     = std::string("get-oper-mgmt"),
        .e_argc    = 1,
        .e_help    = std::string("Get agent operational state from the server"),
        .e_usage   = std::string("get-oper-mgmt <subscription-id> <verbosity=terse,brief,detail>*"),
        .e_handler = handle_get_oper_mgmt
    },

    {
        .e_cmd     = std::string("print"),
        .e_argc    = 1,
        .e_help    = std::string("Print all subscriptions known to the client"),
        .e_usage   = std::string("print"),
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

uint32_t agent_client_commands_count =
                                sizeof(agent_client_commands)/sizeof(entry_t);