//
//  agent_test_consolidator.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/18/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_consolidator_hpp
#define agent_test_consolidator_hpp

#include <stdio.h>

// This is a test class
class AgentConsolidatorTest: public testing::Test {
public:
    AgentServerLog    *logger;
    AgentSystem       *sys_handle;
    AgentConsolidator *cons;

    void SetUp ()
    {
        logger     = new AgentServerLog;
        sys_handle = new AgentSystem(logger);
        cons       = new AgentConsolidator(sys_handle, logger);
    }
    
    void TearDown ()
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
    TestArgs(AgentConsolidator *cons_val, uint32_t id_val) : cons(cons_val), id(id_val) {}
};
#endif /* agent_test_consolidator_hpp */
