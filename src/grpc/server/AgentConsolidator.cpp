//
//  AgentConsolidator.cpp
//  Telemetry Agent
//
//  Created: 2/19/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//
#include "AgentConsolidator.hpp"

// Create a request
AgentConsolidatorHandle *
AgentConsolidator::addRequest (const id_idx_t subscription_id,
                               const SubscriptionRequest *request,
                               SubscriptionRequest *system_accepted_request)
{
    AgentConsolidatorHandle *ptr = new AgentConsolidatorHandle(subscription_id);

    // Guard the add request
    std::lock_guard<std::mutex> guard(_consolidator_mutex);

    _logger->log("Add Request ID: " + std::to_string(subscription_id));

    // No memory, we are done
    if (!ptr) {
        _error_count_no_mem++;
        return NULL;
    }

    // Iterate through the paths in the requests
    for (int i = 0; i < request->path_list_size(); i++) {
        // Create a request in the system
        AgentConsolidatorSystemHandlePtr consolidatorsyshandle =
                AgentConsolidatorSystemHandle::create(getSystemHandle(),
                                                      &request->path_list(i));
        // Failure condition
        if (!consolidatorsyshandle) {
            continue;
        }

        telemetry::Path *p_path = system_accepted_request->add_path_list();
        p_path->CopyFrom(request->path_list(i));

        // Store the handle
        ptr->addHandle(consolidatorsyshandle);
    }

#if 0 /* We will let user explicitly delete subscription for now */
    // If atleast one path is accepted
    if (system_accepted_request->path_list_size() == 0) {
        removeRequest(ptr);
        _error_count_create++;
        return NULL;
    }
#endif

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
    _logger->log("Remove Request ID: " +
                 std::to_string(handle->getSubscriptionId()));

    // Iterate through the handle & remove the references to the system handles
    // The last reference will call the destructor of the system handle which
    // will remove the subscription from the system.
    for (int i = 0; i < handle->getHandleCount(); i++) {
        AgentConsolidatorSystemHandlePtr ptr = handle->getHandle(i);
        if (!ptr->destroy(getSystemHandle())) {
            _error_count_underflow++;
        }
    }

    delete handle;
    ++_remove_count;
}

SubscriptionRequest *
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
    _logger->log("Get request ID:" +
                 std::to_string(handle->getSubscriptionId()));

    // Build the answer
    SubscriptionRequest *request_list = new telemetry::SubscriptionRequest;
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
            telemetry::Path *path = request_list->add_path_list();
            path->CopyFrom(*ptr->getPath());
            continue;
        }

        // Query the system
        telemetry::Path *p = ptr->get(getSystemHandle());
        if (p) {
            telemetry::Path *path = request_list->add_path_list();
            path->CopyFrom(*p);
        }
    }

    return request_list;
}

uint32_t
AgentConsolidator::getSystemRequestCount (void)
{
    return AgentConsolidatorSystemHandle::getCount();
}

void
AgentConsolidator::description (void)
{
    std::cout << "Consolidator Statistics:" << std::endl;
    std::cout << "  Add : " << _add_count << std::endl;
    std::cout << "  Rem : " << _remove_count << std::endl;
    std::cout << "Errors:" << std::endl;
    std::cout << "  Mem : " << _error_count_no_mem << std::endl;
    std::cout << "  Add : " << _error_count_create << std::endl;
    std::cout << "  Hdl : " << _error_count_bad_handle << std::endl;
    std::cout << "  Ufl : " << _error_count_underflow << std::endl;
}