//
//  agent_test_consolidator_ut.hpp
//
//  Created: 2/18/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_consolidator_ut_hpp
#define agent_test_consolidator_ut_hpp

#include "AgentServerLog.hpp"
#include "AgentConsolidator.hpp"
#include "AgentSystemFactory.hpp"

#define AGENTCONSOLIDATORNULLLOG            "consolidator_null"

// This is a test class
class AgentConsolidatorNullTest: public testing::Test {
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
                               "/logs/" + AGENTCONSOLIDATORNULLLOG;
        } else {
            std::cerr << "Please setup ROOTPATH environment variable."
                      << std::endl;
            exit(1);
        }
        logger     = new AgentServerLog(agent_server_log);
        sys_handle = AgentSystemFactory::createNull(logger);
        cons       = new AgentConsolidator(sys_handle, logger);
    }

    void TearDown (void)
    {
        delete cons;
        delete sys_handle;
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

#endif /* agent_test_consolidator_ut_hpp */
