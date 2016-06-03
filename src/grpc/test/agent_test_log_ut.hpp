//
//  agent_test_log_ut.hpp
//
//  Created: 6/2/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_log_ut_hpp
#define agent_test_log_ut_hpp

// This is a test class
class LogTest: public testing::Test {
public:
    std::string         root_path;
    std::string         log_file_name;

    void SetUp (void)
    {
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            root_path = (std::string)env_rp;
            log_file_name = (std::string)env_rp + "/logs/log_test.log";
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

#endif /* agent_test_log_ut_hpp */
