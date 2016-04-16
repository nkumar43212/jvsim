//
//  AgentConsolidator.hpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentConsolidator_hpp
#define AgentConsolidator_hpp

#include <iostream>
#include <string>
#include <mutex>
#include "AgentServerProtos.h"
#include "AgentConsolidatorSystem.hpp"
#include "AgentSystem.hpp"

class AgentConsolidator {
    // Logging machine
    AgentServerLog  *_logger;

    // Handle to the system which creates all telemetry requests
    AgentSystem     *_sys_handle;

    // Global lock so that we serialize access to the consolidator
    std::mutex      _consolidator_mutex;

    // Statistics to track what is going on
    uint64_t _add_count;
    uint64_t _remove_count;
    uint64_t _error_count_no_mem;
    uint64_t _error_count_create;
    uint64_t _error_count_bad_handle;
    uint64_t _error_count_underflow;

public:
    // Construction
    AgentConsolidator (AgentSystem *sys_handle, AgentServerLog *logger) :
                       _sys_handle(sys_handle), _logger(logger)
    {
        _add_count    = 0;
        _remove_count = 0;
        _error_count_bad_handle  = 0;
        _error_count_create      = 0;
        _error_count_no_mem      = 0;
        _error_count_underflow   = 0;
    }
    ~AgentConsolidator () {}

    // Accessors
    AgentServerLog  *getLogger (void)       { return _logger; }
    AgentSystem *getSystemHandle (void)     { return _sys_handle;   }
    uint64_t     getAddCount (void)         { return _add_count;    }
    uint64_t     getRemCount (void)         { return _remove_count; }
    uint64_t     getErrors   (void)
    {
        return (_error_count_bad_handle + _error_count_create +
                _error_count_no_mem + _error_count_underflow);
    }

    // Create a request into the system.
    AgentConsolidatorHandle * addRequest(const std::string request_id,
                                         const SubscriptionRequest *request);

    // Remove an earlier created request
    void removeRequest(AgentConsolidatorHandle *handle);

    // Query the system using the consolidator handle to get the original request
    // Used by testing infra to ensure that the system has what was asked for
    // The API returns either the requests cached in the consolidator or queries
    // the system.
    SubscriptionRequest *getRequest(AgentConsolidatorHandle *handle,
                                    bool cached = true);

    // Number of total requests made on the system.
    uint32_t getSystemRequestCount(void);

    // Status of all requests
    void description(void);
};

#endif /* AgentConsolidator_hpp */
