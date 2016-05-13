//
//  agent_test_oc.hpp
//  agent-jv
//
//  Created by ABBAS SAKARWALA on 5/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_oc_hpp
#define agent_test_oc_hpp

#include "agent_test.hpp"

// This is a test class
class AgentClientOpenConfigTest: public testing::Test {

public:
    AgentServerLog *logger;

    void SetUp (void)
    {
        std::string CLIENT_LOGDIR;
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            CLIENT_LOGDIR = (std::string)env_rp;
        } else {
            std::cerr << "Please setup ROOTPATH environment variable."
            << std::endl;
            exit(1);
        }
        std::string logfile = CLIENT_LOGDIR + "/logs/agent_test_oc.log";
        logger = new AgentServerLog(logfile);
        logger->enable();
    }

    void TearDown (void)
    {
    }

    std::string CLIENT_LOGDIR;
    static void *create_subscriptions(void *args);
    static void *delete_subscriptions(void *args);
};

#endif /* agent_test_oc_hpp */