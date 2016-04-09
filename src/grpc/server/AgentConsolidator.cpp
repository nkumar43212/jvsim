//
//  AgentConsolidator.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//
#include "AgentConsolidator.hpp"

// Create a request
AgentConsolidatorHandle *
AgentConsolidator::addRequest (const std::string request_id,
                               const Telemetry::SubscriptionRequest *request)

{
    AgentConsolidatorHandle *ptr = new AgentConsolidatorHandle(request_id);
    
    
    // Guard the add request
    std::lock_guard<std::mutex> guard(_consolidator_mutex);
    _logger->log("Add Request:" + request_id);
    
    // No memory, we are done
    if (!ptr) {
        _error_count_no_mem++;
        return NULL;
    }
    
    // Iterate through the paths in the requests
    for (int i = 0; i < request->path_list_size(); i++) {
        // Create a request in the system
        AgentConsolidatorSystemHandlePtr syshandle = AgentConsolidatorSystemHandle::create(getSystemHandle(),
                                                                                           &request->path_list(i));
        if (!syshandle) {
            removeRequest(ptr);
            _error_count_create++;
            return NULL;
        }
        
        // Store the handle
        ptr->addHandle(syshandle);
    }
    
    ++_add_count;
    return ptr;
}

void
AgentConsolidator::removeRequest (AgentConsolidatorHandle *handle)
{
    // Guard the add request
    std::lock_guard<std::mutex> guard(_consolidator_mutex);
    
    // Is the handle valid ?
    if (!handle) {
        _error_count_bad_handle++;
        return;
    }
    
    // Make a note
    _logger->log("Remove request:" + handle->getId());
    
    // Iterate through the handle and remove the references to the system handles
    // The last reference will call the destructor of the system handle which will
    // remove the subscription from the system.
    for (int i = 0; i < handle->getHandleCount(); i++) {
        AgentConsolidatorSystemHandlePtr ptr = handle->getHandle(i);
        if (!ptr->destroy(getSystemHandle())) {
            _error_count_underflow++;
        }
    }
    
    delete handle;
    ++_remove_count;
}

Telemetry::SubscriptionRequest *
AgentConsolidator::getRequest (AgentConsolidatorHandle *handle, bool cached)
{
    // Guard the add request
    std::lock_guard<std::mutex> guard(_consolidator_mutex);
    
    // Is the handle valid ?
    if (!handle) {
        _error_count_bad_handle++;
        return NULL;
    }
    
    // Make a note
    _logger->log("Get request:" + handle->getId());
    
    // Build the answer
    Telemetry::SubscriptionRequest *request_list = new Telemetry::SubscriptionRequest;
    if (!request_list) {
        return NULL;
    }
    
    // Iterate through the handle
    for (int i = 0; i < handle->getHandleCount(); i++) {
        AgentConsolidatorSystemHandlePtr ptr = handle->getHandle(i);
        if (!ptr) {
            continue;
        }
        
        // Return the local state in the consolidator
        if (cached) {
            Telemetry::Path *path = request_list->add_path_list();
            path->CopyFrom(*ptr->getRequest());
            continue;
        }
        
        // Query the system
        Telemetry::Path *p = ptr->get(getSystemHandle());
        if (p) {
            Telemetry::Path *path = request_list->add_path_list();
            path->CopyFrom(*p);
        }
    }
    
    return request_list;
}

uint32_t
AgentConsolidator::getSystemRequestCount ()
{
    return AgentConsolidatorSystemHandle::getCount();
}
void
AgentConsolidator::description ()
{
    std::cout << "Consolidator Statistics:\n";
    std::cout << "  Add : " << _add_count << "\n";
    std::cout << "  Rem : " << _remove_count << "\n";
    std::cout << "Errors:\n";
    std::cout << "  Mem : " << _error_count_no_mem << "\n";
    std::cout << "  Add : " << _error_count_create << "\n";
    std::cout << "  Hdl : " << _error_count_bad_handle << "\n";
}
