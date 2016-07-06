//
//  AgentConsolidatorSystem.cpp
//  Telemetry Agent
//
//  Created: 2/19/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <functional>
#include <map>
#include "AgentConsolidatorSystem.hpp"
#include "AgentSystem.hpp"
#include "AgentUtils.hpp"

// For now defined as global but should make it part of Consolidator class ???
// TODO ABBAS --- review with Nitin
static AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                    InternalIdGenerator(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);

// The db of all requests into the system
// <request_path_str, ConsolidatorSystemHandlerPtr>
typedef std::map<std::string, AgentConsolidatorSystemHandlePtr> AgentSystemDB;
AgentSystemDB sysdb;

AgentConsolidatorSystemHandlePtr
AgentConsolidatorSystemHandle::create (AgentSystem *sys_handle,
                                       const telemetry::Path *request_path)
{
    // Find if the request path already exist
    AgentConsolidatorSystemHandlePtr ptr = find(request_path);
    if (ptr) {
        // Just increment the reference counter and bail out
        ptr->incRef();
        sys_handle->getLogger()->log("ConsolidatorSystemHandle already exist ("
                    + std::to_string(ptr->getInternalSubscriptionId()) + "): "
                    + AgentUtils::getMessageString(*request_path));
        return ptr;
    }

    // No dice. Allocate a new handle
    AgentConsolidatorSystemHandlePtr handle =
                            std::make_shared<AgentConsolidatorSystemHandle>();

    // Insert in the DB
    handle->setRef(1);
    handle->setPath(request_path);
    // Allocate an internal subscriber id
    id_idx_t _i_s_id = InternalIdGenerator.allocate();
    sys_handle->getLogger()->log("Reserved Internal Id = " +
                                 std::to_string(_i_s_id));
    handle->setInternalSubscriptionId(_i_s_id);
    bool result = insert(sys_handle, request_path, handle);
    // If insert successful
    if (result) {
        sys_handle->getLogger()->log("ConsolidatorSystemHandle create successful.");
        return handle;
    }

    // Insert has failed, revert needed things again
    // Deallocate internal subscriber id
    sys_handle->getLogger()->log("Deleted Internal Id = " +
                                 std::to_string(_i_s_id));
    bool bad;
    InternalIdGenerator.deallocate(_i_s_id, &bad);

    return NULL;
}

bool
AgentConsolidatorSystemHandle::destroy (AgentSystem *sys_handle)
{
    // Ensure that we don't underflow
    if (!getRef()) {
        sys_handle->getLogger()->log("ConsolidatorSystemHandle has underflow: "
                                     + AgentUtils::getMessageString(_path));
        return false;
    }

    // Decerement the reference
    decRef();

    // If others still have a reference, we are done
    if (getRef()) {
        sys_handle->getLogger()->log("ConsolidatorSystemHandle still in use ("
                        + std::to_string(getInternalSubscriptionId()) + "): "
                        + AgentUtils::getMessageString(_path));
        return true;
    }

    // Remove from the system
    remove(sys_handle, &_path);

    sys_handle->getLogger()->log("ConsolidatorSystemHandle destroy successful");
    return true;
}

void
AgentConsolidatorSystemHandle::description (void)
{
}

AgentConsolidatorSystemHandlePtr
AgentConsolidatorSystemHandle::find (const telemetry::Path *request_path)
{
    // Convert request to string
    std::string request_path_str = AgentUtils::getMessageString(*request_path);

    // Does it exist
    AgentSystemDB::iterator itr = sysdb.find(request_path_str);
    if (itr == sysdb.end()) {
        return NULL;
    }

    // All is well
    return itr->second;
}

bool
AgentConsolidatorSystemHandle::insert (AgentSystem *sys_handle,
                        const telemetry::Path *request_path,
                        AgentConsolidatorSystemHandlePtr consolidatorsyshandle)
{
    // Convert request to string
    std::string request_path_str = AgentUtils::getMessageString(*request_path);

    // Store it away
    sysdb[request_path_str] = consolidatorsyshandle;

    // Generate a request towards the system
    bool result = sys_handle->systemAdd(
                SystemId(consolidatorsyshandle->getInternalSubscriptionId()),
                request_path);

    // request failed
    if (!result) {
        sys_handle->getLogger()->log("System add failed for " +
                                     request_path_str);
        // Detach from the sysdb
        sysdb.erase(request_path_str);
        return false;
    }

    sys_handle->getLogger()->log("Inserted " + request_path_str);
    return true;
}

bool
AgentConsolidatorSystemHandle::remove (AgentSystem *sys_handle,
                                       const telemetry::Path *request_path)
{
    // Convert request to string
    std::string request_path_str = AgentUtils::getMessageString(*request_path);

    // Does it exist ?
    AgentSystemDB::iterator itr = sysdb.find(request_path_str);
    if (itr == sysdb.end()) {
        sys_handle->getLogger()->log("Remove could not find: " +
                                     request_path_str);
        return false;
    }

    bool result = sys_handle->systemRemove(SystemId(itr->second->getInternalSubscriptionId()),
                             request_path);
    // request failed
    if (!result) {
        sys_handle->getLogger()->log("System remove failed for " +
                                     request_path_str);
    }
    // Delete the Internal ID
    id_idx_t _i_s_id = itr->second->getInternalSubscriptionId();
    sys_handle->getLogger()->log("Deleted Internal Id = " +
                                 std::to_string(_i_s_id));
    bool bad;
    InternalIdGenerator.deallocate(_i_s_id, &bad);

    // Detach from the sysdb
    sysdb.erase(itr);

    sys_handle->getLogger()->log("Removed " + request_path_str);

    return result;
}

telemetry::Path *
AgentConsolidatorSystemHandle::get (AgentSystem *sys_handle)
{
    // Convert request to string
    std::string request_path_str = AgentUtils::getMessageString(_path);

    // Do we have this entry ?
    AgentSystemDB::iterator itr = sysdb.find(request_path_str);
    if (itr == sysdb.end()) {
        sys_handle->getLogger()->log("Get could not find: " + request_path_str);
        return NULL;
    }

    // Query the system
    return sys_handle->systemGet(
                       SystemId(itr->second->getInternalSubscriptionId()));
}

uint32_t
AgentConsolidatorSystemHandle::getCount (void)
{
    AgentSystemDB::iterator itr;
    uint32_t count = 0;
    
    for (itr = sysdb.begin(); itr != sysdb.end(); itr++) {
        count++;
    }
    
    return count;
}
