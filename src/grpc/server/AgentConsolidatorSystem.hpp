//
//  AgentConsolidatorSystem.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentConsolidatorSystem_hpp
#define AgentConsolidatorSystem_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "AgentServerProtos.h"
#include "AgentSystem.hpp"

// Consolidates requests into the JUNOS system. Multiple subscriptions can
// request for the same data and streaming profiles. This interface ensures that
// we do not create multiple subscriptions on the JUNOS system and reuse the sensors

class AgentConsolidatorSystemHandle {
    Telemetry::Path _request;
    uint64_t    _refcount;
    
public:
    // Accessors
    void     setRef (uint64_t val)                  { _refcount = val;  }
    uint64_t getRef ()                              { return _refcount; }
    void     decRef ()                              { --_refcount;      }
    void     incRef ()                              { ++_refcount;      }
    void     setRequest(const Telemetry::Path *request) { _request.CopyFrom(*request); }
    const Telemetry::Path *getRequest ()                { return &_request; }
    
    // Object lifecycle
    static std::shared_ptr<AgentConsolidatorSystemHandle> create(AgentSystem *sys_handle,
                                                                 const Telemetry::Path *request_path);
    

    // Free up a system handle
    bool         destroy(AgentSystem *sys_handle);
    ~AgentConsolidatorSystemHandle() {}
    
    // Get the contents of the handle from the system. Used by the unit test infra
    Telemetry::Path *get(AgentSystem *sys_handle);

    // Class interface
    static std::shared_ptr<AgentConsolidatorSystemHandle> find(const Telemetry::Path *request_path,
                                                               bool *collision);
    static void insert(AgentSystem *sys_handle,
                       const Telemetry::Path *request_path,
                       std::shared_ptr<AgentConsolidatorSystemHandle> syshandle);
    static void remove(AgentSystem *sys_handle,
                       const Telemetry::Path *request_path);
    static uint32_t getCount();
    void description();
};

typedef std::shared_ptr<AgentConsolidatorSystemHandle> AgentConsolidatorSystemHandlePtr;

class AgentConsolidatorHandle {
    // Request Identifier
    std::string _request_id;
    
    // List of system handles allocated
    std::vector<AgentConsolidatorSystemHandlePtr> _syshandles;
    
public:
    AgentConsolidatorHandle (const std::string request_id) : _request_id(request_id) {}
    ~AgentConsolidatorHandle ()
    {
        _syshandles.clear();
    }
    
    const std::string getId ()
    {
        return _request_id;
    }
    
    uint32_t getHandleCount ()
    {
        return (uint32_t) _syshandles.size();
    }
    
    AgentConsolidatorSystemHandlePtr getHandle (uint32_t pos)
    {
        return pos < getHandleCount() ? _syshandles[pos] : NULL;
    }
    
    void addHandle (AgentConsolidatorSystemHandlePtr sys_handle)
    {
        _syshandles.push_back(sys_handle);
    }
};

#endif /* AgentConsolidatorSystem_hpp */
