//
//  agent_test_consolidator_proc.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 3/23/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_consolidator_proc_hpp
#define agent_test_consolidator_proc_hpp

#include "AgentSystemFactory.hpp"
#include "AgentConsolidator.hpp"

// This is a test class
class AgentConsolidatorProcTest: public testing::Test {
public:
    AgentServerLog    *logger;
    AgentSystem       *sys_handle;
    AgentConsolidator *cons;

    void SetUp (void)
    {
        std::string agent_server_log;
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            agent_server_log = (std::string)env_rp +
                               "/logs/" + AGENTSERVER_LOGFILE;
        } else {
            std::cerr << "Please setup ROOTPATH environment variable."
                      << std::endl;
            exit(1);
        }
        logger     = new AgentServerLog(agent_server_log);
        sys_handle = AgentSystemFactory::createSystemProcess(logger);
        cons       = new AgentConsolidator(sys_handle, logger);
    }

    void TearDown (void)
    {
        delete cons;
        delete logger;
    }

    static void *create(void *);
};

#endif /* agent_test_consolidator_proc_hpp */