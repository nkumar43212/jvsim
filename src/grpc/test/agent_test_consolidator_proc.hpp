//
//  agent_test_consolidator_proc.hpp
//  grpc
//
//  Created by NITIN KUMAR on 3/23/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_consolidator_proc_hpp
#define agent_test_consolidator_proc_hpp

#include <stdio.h>
#include "AgentSystemFactory.hpp"

// This is a test class
class AgentConsolidatorProcTest: public testing::Test {
public:
    AgentServerLog    *logger;
    AgentSystem       *sys_handle;
    AgentConsolidator *cons;
    
    void SetUp ()
    {
        logger     = new AgentServerLog;
        sys_handle = AgentSystemFactory::createSystemProcess(logger);
        cons       = new AgentConsolidator(sys_handle, logger);
    }
    
    void TearDown ()
    {
        delete cons;
        delete logger;
    }
    
    static void *create(void *);
};


#endif /* agent_test_consolidator_proc_hpp */
