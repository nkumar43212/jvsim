//
//  PathValidator.hpp
//  Telemetry Agent
//
//  Created: 5/24/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef PathValidator_hpp
#define PathValidator_hpp

#include <string>
#include <memory>
#include "agent.pb.h"
#include "json/json.h"
#include "AgentServerLog.hpp"

#define SAMPLE_FREQUENCY_MIN                2000
#define SAMPLE_FREQUENCY_MAX             3600000

class PathInformation {
public:
    std::string path;
    bool filter_support;
    bool suppress_unchanged_support;
    bool max_silent_interval_support;
    uint32_t sample_frequency_min;
    uint32_t sample_frequency_max;

    PathInformation (std::string path) : path(path)
    {
        filter_support = true;
        suppress_unchanged_support = false;
        max_silent_interval_support = false;
        sample_frequency_min = SAMPLE_FREQUENCY_MIN;
        sample_frequency_max = SAMPLE_FREQUENCY_MAX;
    }

    ~PathInformation () {}
};

// PathInformation store
typedef std::shared_ptr<PathInformation> PathInformationPtr;
typedef std::map<std::string, PathInformationPtr> PathInformationDB;

class PathValidator {
    AgentServerLog *_logger;
    void _read_path_information_file(std::string filename,
                                     Json::Value &path_information_json);
    uint32_t _roundToThousandths(uint32_t num, uint32_t max);
public:
    PathInformationDB path_information_db;

    PathValidator (AgentServerLog *logger) : _logger(logger) {};
    
    ~PathValidator ()
    {
        // Delete the store
        delete_path_information_db();
    }
    void build_path_information_db(std::string filename);
    void print_path_information_db(void);
    void delete_path_information_db(void);
    void validate_path(Telemetry::Path &path);
};

#endif /* PathValidator_hpp */
