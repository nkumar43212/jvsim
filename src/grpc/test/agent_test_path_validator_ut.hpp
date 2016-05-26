//
//  agent_test_path_validator_ut.hpp
//
//  Created: 5/25/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef agent_test_path_validator_ut_hpp
#define agent_test_path_validator_ut_hpp

#include "PathValidator.hpp"

#define PATHVALIDATORLOG                    "pathvalidator"

// This is a test class
class PathValidatorTest: public testing::Test {
public:
    PathValidator     *path_validator;
    AgentServerLog    *logger;
    std::string       root_path;

    void SetUp (void)
    {
        std::string path_validator_log;
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            root_path = (std::string)env_rp;
            path_validator_log = (std::string)env_rp +
                                 "/logs/" + PATHVALIDATORLOG;
        } else {
            std::cerr << "Please setup ROOTPATH environment variable."
            << std::endl;
            exit(1);
        }
        logger     = new AgentServerLog(path_validator_log);
        logger->enable();
        path_validator = new PathValidator(logger);
        bool status = path_validator->build_path_information_db(root_path +
                                                    "/config/ocpaths.json");
        EXPECT_TRUE(status);
        if (!status) {
            exit(0);
        }
        path_validator->print_path_information_db();
    }

    void TearDown (void)
    {
        delete path_validator;
        delete logger;
    }

    static void *create(void *);
};

#endif /* agent_test_path_validator_ut_hpp */
