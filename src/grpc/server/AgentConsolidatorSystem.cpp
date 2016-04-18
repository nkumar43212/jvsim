//
//  AgentConsolidatorSystem.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentConsolidatorSystem.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <functional>
#include <map>
#include "AgentSystem.hpp"

// For now defined as global but should make it part of Consolidator class ???
// TODO ABBAS
static AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                    InternalIdGenerator(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);

// The db of all requests into the system
// <request_path_str, ConsolidatorSystemHandlerPtr>
typedef std::map<std::string, AgentConsolidatorSystemHandlePtr> AgentSystemDB;
AgentSystemDB sysdb;

AgentConsolidatorSystemHandlePtr
AgentConsolidatorSystemHandle::create (AgentSystem *sys_handle,
                                       const Telemetry::Path *request_path)
{
    // Find if the request path already exist
    AgentConsolidatorSystemHandlePtr ptr = find(request_path);
    if (ptr) {
        // Just increment the reference counter and bail out
        ptr->incRef();
        return ptr;
    }

    // No dice. Allocate a new handle
    AgentConsolidatorSystemHandlePtr handle = std::make_shared<AgentConsolidatorSystemHandle>();

    // Insert in the DB
    handle->setRef(1);
    handle->setPath(request_path);
    // Allocate an internal subscriber id
    id_idx_t _i_s_id = InternalIdGenerator.allocate();
    sys_handle->getLogger()->log("Reserved Internal Id = " + std::to_string(_i_s_id));
    handle->setInternalSubscriptionId(_i_s_id);
    insert(sys_handle, request_path, handle);

    sys_handle->getLogger()->log("ConsolidatorSystemHandle create successful.");
    return handle;
}

bool
AgentConsolidatorSystemHandle::destroy (AgentSystem *sys_handle)
{
    // Ensure that we don't underflow
    if (!getRef()) {
        return false;
    }

    // Decerement the reference
    decRef();

    // If others still have a reference, we are done
    if (getRef()) {
        return true;
    }

    // Remove from the system
    remove(sys_handle, &_path);

    sys_handle->getLogger()->log("ConsolidatorSystemHandle destroy successful.");
    return true;
}

void
AgentConsolidatorSystemHandle::description (void)
{
}

AgentConsolidatorSystemHandlePtr
AgentConsolidatorSystemHandle::find (const Telemetry::Path *request_path)
{
    // Convert request to string
    std::string request_path_str;
    // Serialize the data in text format
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(*request_path, &request_path_str);

    // Does it exist
    AgentSystemDB::iterator itr = sysdb.find(request_path_str);
    if (itr == sysdb.end()) {
        return NULL;
    }

    // All is well
    return itr->second;
}

void
AgentConsolidatorSystemHandle::insert (AgentSystem *sys_handle,
                                       const Telemetry::Path *request_path,
                                       AgentConsolidatorSystemHandlePtr consolidatorsyshandle)
{
    // Convert request to string
    std::string request_path_str;
    // Serialize the data in text format
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(*request_path, &request_path_str);

    // Store it away
    sysdb[request_path_str] = consolidatorsyshandle;

    // Generate a request towards the system
    sys_handle->systemAdd(SystemId(consolidatorsyshandle->getInternalSubscriptionId()),
                          request_path);

    sys_handle->getLogger()->log("Inserted " + request_path_str);
}

void
AgentConsolidatorSystemHandle::remove (AgentSystem *sys_handle,
                                       const Telemetry::Path *request_path)
{
    // Convert request to string
    std::string request_path_str;
    // Serialize the data in text format
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(*request_path, &request_path_str);

    // Does it exist ?
    AgentSystemDB::iterator itr = sysdb.find(request_path_str);
    if (itr == sysdb.end()) {
        return;
    }

    sys_handle->systemRemove(SystemId(itr->second->getInternalSubscriptionId()),
                             request_path);
    // Delete the Internal ID
    id_idx_t _i_s_id = itr->second->getInternalSubscriptionId();
    sys_handle->getLogger()->log("Deleted Internal Id = " + std::to_string(_i_s_id));
    bool bad;
    InternalIdGenerator.deallocate(_i_s_id, &bad);
    
    // Detach from the sysdb
    sysdb.erase(itr);

    sys_handle->getLogger()->log("Removed " + request_path_str);
}

Telemetry::Path *
AgentConsolidatorSystemHandle::get (AgentSystem *sys_handle)
{
    // Convert request to string
    std::string request_path_str;
    // Serialize the data in text format
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(_path, &request_path_str);

    // Do we have this entry ?
    AgentSystemDB::iterator itr = sysdb.find(request_path_str);
    if (itr == sysdb.end()) {
        return NULL;
    }

    // Query the system
    return sys_handle->systemGet(SystemId(itr->second->getInternalSubscriptionId()));
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