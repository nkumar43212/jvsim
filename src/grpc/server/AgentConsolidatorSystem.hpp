//
//  AgentConsolidatorSystem.hpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentConsolidatorSystem_hpp
#define AgentConsolidatorSystem_hpp

#include <iostream>
#include <vector>
#include "AgentServerProtos.h"
#include "AgentSystem.hpp"
#include "AgentServerIdManager.hpp"

// Consolidates requests into the JUNOS system. Multiple subscriptions can
// request for the same data and streaming profiles. This interface ensures that
// we do not create multiple subscriptions on the JUNOS system
// and reuse the sensors
class AgentConsolidatorSystemHandle {
    Telemetry::Path         _path;
    id_idx_t                _internal_subscription_id;
    uint64_t                _refcount;

public:
    // Accessors
    void     setInternalSubscriptionId (id_idx_t val)
                                        { _internal_subscription_id = val; }
    id_idx_t getInternalSubscriptionId (void)
                                        { return _internal_subscription_id; }
    void     setRef (uint64_t val)      { _refcount = val;  }
    uint64_t getRef (void)              { return _refcount; }
    void     decRef (void)              { --_refcount;      }
    void     incRef (void)              { ++_refcount;      }
    void     setPath(const Telemetry::Path *request)
                                        { _path.CopyFrom(*request); }
    const Telemetry::Path *getPath (void)
                                        { return &_path; }

    ~AgentConsolidatorSystemHandle() {}

    // Object lifecycle
    static std::shared_ptr<AgentConsolidatorSystemHandle>
    create(AgentSystem *sys_handle, const Telemetry::Path *request_path);

    // Free up a system handle
    bool destroy(AgentSystem *sys_handle);

    // Get the contents of the handle from the system. Used by the unit test infra
    Telemetry::Path *get(AgentSystem *sys_handle);

    // Class interface
    static std::shared_ptr<AgentConsolidatorSystemHandle>
    find(const Telemetry::Path *request_path);
    static void insert(AgentSystem *sys_handle,
                       const Telemetry::Path *request_path,
                       std::shared_ptr<AgentConsolidatorSystemHandle> syshandle);
    static void remove(AgentSystem *sys_handle,
                       const Telemetry::Path *request_path);
    static uint32_t getCount(void);
    void description(void);
};


// Consolidator Handle
typedef std::shared_ptr<AgentConsolidatorSystemHandle>
                                            AgentConsolidatorSystemHandlePtr;

class AgentConsolidatorHandle {
    // Subscription Identifier
    id_idx_t _subscription_id;

    // List of system handles allocated
    std::vector<AgentConsolidatorSystemHandlePtr> _consolidatorsyshandles;

public:
    AgentConsolidatorHandle (const id_idx_t subscription_id) :
                             _subscription_id(subscription_id) {}

    ~AgentConsolidatorHandle ()
    {
        _consolidatorsyshandles.clear();
    }

    const id_idx_t getSubscriptionId ()
    {
        return _subscription_id;
    }

    uint32_t getHandleCount (void)
    {
        return (uint32_t) _consolidatorsyshandles.size();
    }

    AgentConsolidatorSystemHandlePtr getHandle (uint32_t pos)
    {
        return pos < getHandleCount() ? _consolidatorsyshandles[pos] : NULL;
    }

    void addHandle (AgentConsolidatorSystemHandlePtr consolidatorsyshandles)
    {
        _consolidatorsyshandles.push_back(consolidatorsyshandles);
    }
};

#endif /* AgentConsolidatorSystem_hpp */