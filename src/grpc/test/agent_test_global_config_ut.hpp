//
//  agent_test_global_config_ut.hpp
//
//  Created: 5/27/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_global_config_ut_hpp
#define agent_test_global_config_ut_hpp

#include "GlobalConfig.hpp"

// This is a test class
class GlobalConfigTest: public testing::Test {
public:
    std::string         root_path;
    std::string         global_config_ini;

    void SetUp (void)
    {
        std::string path_validator_log;
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            root_path = (std::string)env_rp;
            global_config_ini = (std::string)env_rp +"/config/agent_server.ini";
        } else {
            std::cerr << "Please setup ROOTPATH environment variable."
            << std::endl;
            exit(1);
        }
    }

    void TearDown (void)
    {
    }

    static void *create(void *);
};

#endif /* agent_test_global_config_ut_hpp */
