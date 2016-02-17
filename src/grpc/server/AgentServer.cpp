//
//  AgentServer.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <stdio.h>
#include "AgentServer.h"
#include "OpenConfig.hpp"
#include "AgentSubscription.hpp"

Status
AgentServer::telemetrySubscribe (ServerContext *context,
                                 const SubscriptionRequest *args,
                                 ServerWriter<OpenConfigData>* writer)
{
    PathList    path_list;
    std::string log_str;
    
    // Get all the paths that we are interested in
    for (int i = 0; i < args->path_list_size(); i++) {
        path_list.push_back(args->path_list(i).path());
        log_str += args->path_list(i).path();
        log_str += ":";
    }
   
    // Make a note
    _logger->log("Subscribe-Begin:" + log_str);
    
    // Allocate an ID
    uint32_t id = _id_manager.allocate();
    if (id == _id_manager.getNullIdentifier()) {
        std::string id_str = std::to_string(_id_manager.getNullIdentifier());
        context->AddInitialMetadata("subscription-id", id_str);
        context->AddInitialMetadata("subscription-name", "__agent_error__id_exhausted");
        _logger->log("Subscribe-stream-end: Error, ID Exhausted");
        return Status::OK;
    }
    
    // Create a subscription
    AgentServerTransport transport(context, writer);
    AgentSubscriptionLimits limits(args->limit_records(), args->limit_time_seconds());
    AgentSubscription *sub = AgentSubscription::createSubscription(id,
                                                                   transport,
                                                                   path_list,
                                                                   limits);
    if (!sub) {
        std::string id_str = std::to_string(_id_manager.getNullIdentifier());
        context->AddInitialMetadata("subscription-id", id_str);
        context->AddInitialMetadata("subscription-name", "__agent_error__");
        _logger->log("Subscribe-stream-end: Error");
        return Status::OK;
    }
    
    // Log the subscription Identifier
    log_str = std::to_string(sub->getId());
    _logger->log("Subscription-Allocate:" + log_str);
    
    // Send back the response on the metadata channel
    std::string id_str = std::to_string(sub->getId());
    context->AddInitialMetadata("subscription-id", id_str);
    context->AddInitialMetadata("subscription-name", sub->getName());
    for (int i = 0; i < args->path_list_size(); i++) {
        std::string key_str = "path-" + std::to_string(i);
        context->AddInitialMetadata(key_str, args->path_list(i).path());
    }
    context->AddInitialMetadata("limit_records", std::to_string(limits.getRecords()));
    context->AddInitialMetadata("limit_seconds", std::to_string(limits.getSeconds()));
                                
    // Turn it on
    sub->enable();
    
    // Wait till the subscription gets cancelled or it expires
    while (!sub->expired() && sub->getActive()) {
        sleep(1);
    }
    
    // Streaming over. The subscription will be deleted by unSubscribe
    sub->setActive(false);
    _logger->log("Subscribe-stream-end:");
   
    return Status::OK;
}

Status
AgentServer::telemetryUnSubscribe (ServerContext *context, const agent::UnSubscribeRequest *args, Reply *reply)
{
    // Make a note
    _logger->log("Unsubscribe:ID = " + std::to_string(args->id()));
    
    // Lookup the subscription
    AgentSubscription *sub = AgentSubscription::findSubscription(args->id());
    if (!sub) {
        std::string err_str = "Subscription Not Found = " + std::to_string(args->id());
        reply->set_code(agent::NO_SUBSCRIPTION_ENTRY);
        reply->set_code_str(err_str);
        _logger->log(err_str);
        return Status::OK;
    }
    
    // Cancel the subscription on the bus
    sub->disable();
    
    // Remove the subscription
    AgentSubscription::deleteSubscription(sub->getId());
    _id_manager.deallocate(sub->getId());
    delete sub;
    
    reply->set_code(agent::OK);
    reply->set_code_str("Subscription Deleted");
    _logger->log("UnsubscribeEnd");
    
    return Status::OK;
}

Status
AgentServer::telemetrySubscriptionsGet (ServerContext *context, const agent::GetRequest *args, agent::OpenConfigData *datap)
{
    AgentSubscription *sub;
    
    // Fill in the base
    // Fill in the common header
    datap->set_system_id("1.1.1.1");
    datap->set_component_id(1);
    datap->set_timestamp(100);

    // Iterate the subscription data base
    sub = AgentSubscription::getFirst();
    while (sub) {
        // Add a new entry
        KeyValue *kv;
        kv = datap->add_kv();
        kv->set_key(sub->getName());
        kv->set_int_value(sub->getId());
        
        // Move to the next entry
        sub = AgentSubscription::getNext(sub->getId());
    }
    
    return Status::OK;
}

Status
AgentServer::telemetryOperationalStateGet (ServerContext *context, const agent::GetRequest *args, OpenConfigData *datap)
{
    // Make a note
    _logger->log("Operational Get:ID = " + std::to_string(args->id()));
    
    // If a subscription ID hasn't been set, then we are done
    if (!args->id()) {
        return Status::OK;
    }
    
    // Lookup the subscription
    AgentSubscription *sub = AgentSubscription::findSubscription(args->id());
    if (!sub) {
        std::string err_str = "Subscription Not Found=" + std::to_string(args->id());
        _logger->log(err_str);
        return Status::OK;
    }

    // Get all the statistics for this subscription
    uint32_t verbosity = args->verbosity() ? args->verbosity() : 0;
    sub->getOperational(datap, verbosity);
    
    return Status::OK;
}




