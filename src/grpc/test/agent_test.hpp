//
//  agent_test.hpp
//
//  Created: 1/31/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_hpp
#define agent_test_hpp

#include "AgentClient.hpp"
#include "AgentServerLog.hpp"

typedef ClientReader<OpenConfigData> AgentClientReader;

// This is a test class
class AgentClientTest: public testing::Test {
public:
    std::string CLIENT_LOGDIR;
    std::string GRPC_SERVER_IP_PORT;

    void SetUp (void)
    {
        std::string CLIENT_LOGDIR;
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            CLIENT_LOGDIR = (std::string)env_rp + "/logs/";
        } else {
            std::cerr << "Please setup ROOTPATH environment variable."
                      << std::endl;
            exit(1);
        }
        GRPC_SERVER_IP_PORT = std::string("localhost:50051");
    }

    void TearDown (void)
    {
    }

    static void *create_subscriptions(void *args);
    static void *delete_subscriptions(void *args);
};

class TestArgs {
public:
    TestArgs (int i, Telemetry::OpenConfigData *d, int N, std::string cd,
              std::string grpc_server_ip_port):
             index(i), data(d), max_data_size(N), client_logdir(cd),
             grpc_server_ip_port(grpc_server_ip_port),
             data_size(0), limit_record(0),
             return_before_graceful_terminate(false) {}

    int    index;
    Telemetry::OpenConfigData *data;
    int max_data_size;
    int data_size;
    int limit_record;
    bool return_before_graceful_terminate;
    std::string client_logdir;
    std::string grpc_server_ip_port;
    uint32_t subscription_id;
    AgentClient *client;
};

#endif /* agent_test_hpp */