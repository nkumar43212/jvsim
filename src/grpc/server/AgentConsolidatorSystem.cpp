//
//  AgentConsolidatorSystem.cpp
//  grpc
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

// The db of all requests into the system
typedef std::map<std::size_t, AgentConsolidatorSystemHandlePtr> AgentSystemDB;
AgentSystemDB sysdb;

AgentConsolidatorSystemHandlePtr
AgentConsolidatorSystemHandle::create (AgentSystem *sys_handle,
                                       const agent::Path *request_path)
{
    // Do we have this ?
    bool collision;
    AgentConsolidatorSystemHandlePtr ptr = find(request_path, &collision);
    if (ptr) {
        ptr->incRef();
        return ptr;
    }
    
    // No support to handle collisions yet
    if (collision) {
        return NULL;
    }
    
    // No dice. Allocate a new handle
    AgentConsolidatorSystemHandlePtr handle = std::make_shared<AgentConsolidatorSystemHandle>();
    
    // Insert in the DB
    handle->setRef(1);
    handle->setRequest(request_path);
    insert(sys_handle, request_path, handle);
    
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
    
    // If others still have a referece, we are done
    if (getRef()) {
        return true;
    }
    
    // Remove from the system
    remove(sys_handle, &_request);
    return true;
}

void
AgentConsolidatorSystemHandle::description()
{
}

AgentConsolidatorSystemHandlePtr
AgentConsolidatorSystemHandle::find (const agent::Path *request_path, bool *collision)
{
    // Assume that we won't collide. If we see this go up, we need to enhance the
    // implementation to maintain a bucket list over here
    *collision = false;
    
    // Generate a hash from the request message
    std::string request_str;
    google::protobuf::TextFormat::PrintToString(*request_path, &request_str);
    std::hash<std::string> hasher;
    auto hashed = hasher(request_str);
    
    // Does it exist
    AgentSystemDB::iterator itr = sysdb.find(hashed);
    if (itr == sysdb.end()) {
        return NULL;
    }
    
    // Make sure that there are no hash collisions and everything matches
    std::string db_str;
    const agent::Path *db_request = itr->second->getRequest();
    google::protobuf::TextFormat::PrintToString(*db_request, &db_str);
    if (db_str != request_str) {
        *collision = true;
        return NULL;
    }
 
    // All is well
    return itr->second;
}

void
AgentConsolidatorSystemHandle::insert (AgentSystem *sys_handle,
                                       const agent::Path *request_path,
                                       AgentConsolidatorSystemHandlePtr syshandle)
{
    // Generate a hash from the request message
    std::string request_str;
    google::protobuf::TextFormat::PrintToString(*request_path, &request_str);
    std::hash<std::string> hasher;
    auto hashed = hasher(request_str);
    
    // Store it away
    sysdb[hashed] = syshandle;
    
    // Generate a request towards the system
    sys_handle->systemAdd(request_path);
}

void
AgentConsolidatorSystemHandle::remove (AgentSystem *sys_handle,
                                       const agent::Path *request_path)
{
    // Generate a hash from the request message
    std::string request_str;
    google::protobuf::TextFormat::PrintToString(*request_path, &request_str);
    std::hash<std::string> hasher;
    auto hashed = hasher(request_str);
    
    AgentSystemDB::iterator itr = sysdb.find(hashed);
    if (itr == sysdb.end()) {
        return;
    }
    
    sys_handle->systemRemove(request_path);
    sysdb.erase(itr);
}

uint32_t
AgentConsolidatorSystemHandle::getCount ()
{
    AgentSystemDB::iterator itr;
    uint32_t count = 0;
    
    for (itr = sysdb.begin(); itr != sysdb.end(); itr++) {
        count++;
    }
    
    return count;
}