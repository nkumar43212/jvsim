//
//  agent_test_consolidator.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 2/18/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_consolidator_hpp
#define agent_test_consolidator_hpp

#include "AgentSystemFactory.hpp"
#include "AgentConsolidator.hpp"

#define AGENT_SYSTEMFACTORY_LOG         "filemode_test"

// This is a test class
class AgentConsolidatorTest: public testing::Test {
public:
    AgentServerLog    *logger;
    AgentSystem       *sys_handle;
    AgentConsolidator *cons;

    void SetUp (void)
    {
        std::string agent_server_log, agent_systemfactory_log;
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            agent_server_log = (std::string)env_rp +
                               "/logs/" + AGENTSERVER_LOGFILE;
            agent_systemfactory_log = (std::string)env_rp +
                               "/logs/" + AGENT_SYSTEMFACTORY_LOG;
        } else {
            std::cerr << "Please setup ROOTPATH environment variable."
                      << std::endl;
            exit(1);
        }
        logger     = new AgentServerLog(agent_server_log);
        sys_handle = AgentSystemFactory::createFile(logger,
                                                    agent_systemfactory_log);
        cons       = new AgentConsolidator(sys_handle, logger);
    }

    void TearDown (void)
    {
        delete cons;
        delete logger;
    }

    static void *create(void *);
};

class TestArgs {
public:
    AgentConsolidator *cons;
    uint32_t id;
    TestArgs(AgentConsolidator *cons_val, uint32_t id_val):
             cons(cons_val), id(id_val) {}
};
#endif /* agent_test_consolidator_hpp */