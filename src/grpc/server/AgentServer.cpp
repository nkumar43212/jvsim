//
//  AgentServer.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

// Header files
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include "AgentServer.h"
#include "OpenConfig.hpp"

Status
AgentServer::telemetrySubscribe (ServerContext *context,
                                 const SubscriptionRequest *request,
                                 ServerWriter<OpenConfigData>* writer)
{
    PathList    path_list;
    std::string log_str;
    AgentConsolidatorHandle *system_handle;
    SubscriptionReply reply;
    SubscriptionResponse *response = reply.mutable_response();

    // Get all the paths that we are interested in
    for (int i = 0; i < request->path_list_size(); i++) {
        path_list.push_back(request->path_list(i).path());
        log_str += request->path_list(i).path();
        log_str += ":";
    }

    // Make a note
    _logger->log("Subscription-Begin: " + log_str);

    // Allocate an ID
    id_idx_t id = _id_manager.allocate();
    if (id == _id_manager.getNullIdentifier()) {
        // Sent id error message and bail out.
        response->set_subscription_id(id);
        _logger->log("Subscription-stream-end: Error, ID Exhausted");
        return _sendMetaDataInfo(context, writer, reply);
    }

    // Create a subscription into the system
    system_handle = _consolidator.addRequest(std::to_string(id), request);
    if (!system_handle) {
        // Delete allocated id
        _id_manager.deallocate(id);
        // Sent id = 0 error message and bail out.
        response->set_subscription_id(_id_manager.getNullIdentifier());
        _logger->log("Subscription-stream-end: Error, Internal System Failure");
        return _sendMetaDataInfo(context, writer, reply);
    }

    // Create a subscription
    AgentServerTransport transport(context, writer);
    AgentSubscriptionLimits limits(request->additional_config().limit_records(), request->additional_config().limit_time_seconds());
    AgentSubscription *sub = AgentSubscription::createSubscription(id,
                                                                   system_handle,
                                                                   transport,
                                                                   path_list,
                                                                   limits);
    if (!sub) {
        // Delete allocated id
        _id_manager.deallocate(id);
        // Sent id = 0 error message and bail out.
        response->set_subscription_id(_id_manager.getNullIdentifier());
        _logger->log("Subscription-stream-end: Error, Subscription Creation Error");
        return _sendMetaDataInfo(context, writer, reply);
    }

    // Log the Subscription Identifier
    log_str = std::to_string(sub->getId());
    _logger->log("Subscription-Allocate: ID = " + log_str);

    // Send back the response on the metadata channel
    response->set_subscription_id(sub->getId());

    // Send path info back to the client
    // TODO ABBAS --- Need to add more than just path details based on capability
    PathList pList = sub->getPathList();
    for (PathList::iterator it = pList.begin(); it < pList.end(); it++) {
        Telemetry::Path *p_tpath = reply.add_path_list();
        p_tpath->set_path(*it);
    }
    // Don't start MQTT subscription till we send this response
    _sendMetaDataInfo(context, writer, reply);

    // Turn it on (Subscribe)
    sub->enable();
    sub->setActive(true);

    bool client_disconnected = false;
    // Wait till the subscription gets cancelled or it expires
    while (!sub->expired() && sub->getActive()) {
        if (sub->getTransport().getServerContext()->IsCancelled() == true) {
            // Client is disconnected
            client_disconnected = true;
            sub->setActive(false);
        }
        sleep(1);
    }

    // Guard the add request
    std::lock_guard<std::mutex> guard(_delete_initiate_mutex);

    // Streaming over. Either client terminated or sub expired
    if (client_disconnected || sub->expired()) {
        // cleanup subscription gracefully
        _cleanupSubscription(sub);
    }
    delete sub;

    log_str = std::to_string(sub->getId());
    _logger->log("Subscription-stream-end: ID = " + log_str);

    return Status::OK;
}


Status
AgentServer::cancelTelemetrySubscription (ServerContext* context, const CancelSubscriptionRequest* cancel_request,CancelSubscriptionReply* cancel_reply)
{
    // Make a note
    _logger->log("cancelTelemetrySubscription: ID = " + std::to_string(cancel_request->subscription_id()));

    // Guard the add request
    std::lock_guard<std::mutex> guard(_delete_initiate_mutex);

    // Lookup the subscription
    AgentSubscription *sub = AgentSubscription::findSubscription(cancel_request->subscription_id());
    if (!sub) {
        std::string err_str = "Subscription Not Found = " + std::to_string(cancel_request->subscription_id());
        cancel_reply->set_code(Telemetry::NO_SUBSCRIPTION_ENTRY);
        cancel_reply->set_code_str(err_str);
        _logger->log(err_str);
        return Status::OK;
    }

    // cleanup subscription gracefully
    _cleanupSubscription(sub);

    // Note, do not delete here, leave it to subscription thread
    // delete sub;

    cancel_reply->set_code(Telemetry::SUCCESS);
    cancel_reply->set_code_str("Subscription Successfully Deleted");
    _logger->log("cancelTelemetrySubscriptionEnd: ID = " + std::to_string(cancel_request->subscription_id()));

    return Status::OK;
}


Status
AgentServer::getTelemetrySubscriptions (ServerContext* context, const GetSubscriptionsRequest* get_request, GetSubscriptionsReply* get_reply)
{
    AgentSubscription *sub;
    id_idx_t subscription_id = get_request->subscription_id();

    _logger->log("getTelemetrySubscriptions ID = " + std::to_string(subscription_id));

    if (subscription_id != 0xFFFFFFFF) {
        // Lookup the subscription
        sub = AgentSubscription::findSubscription(subscription_id);
        if (!sub) {
            _logger->log("Subcription Not Found. ID = " + std::to_string(subscription_id));
            return Status::OK;
        } else {
            SubscriptionReply *sub_reply;
            sub_reply = get_reply->add_subscription_list();
            SubscriptionResponse *sub_resp = sub_reply->mutable_response();
            sub_resp->set_subscription_id(sub->getId());
            
            PathList pathList = sub->getPathList();
            
            for (PathList::iterator it = pathList.begin() ; it != pathList.end(); ++it) {
                Telemetry::Path *path = sub_reply->add_path_list();
                path->set_path(*it);
            }
        }
    } else {
        // Iterate the subscription data base
        sub = AgentSubscription::getFirst();
        while (sub) {
            // Add a new entry
            SubscriptionReply *sub_reply;
            sub_reply = get_reply->add_subscription_list();
            SubscriptionResponse *sub_resp = sub_reply->mutable_response();
            sub_resp->set_subscription_id(sub->getId());

            PathList pathList = sub->getPathList();

            for (PathList::iterator it = pathList.begin() ; it != pathList.end(); ++it) {
                Telemetry::Path *path = sub_reply->add_path_list();
                path->set_path(*it);
            }

            // Move to the next entry
            sub = AgentSubscription::getNext(sub->getId());
        }
    }

    return Status::OK;
}


Status
AgentServer::getTelemetryOperationalState (ServerContext* context, const GetOperationalStateRequest* operational_request, GetOperationalStateReply* operational_reply)
{
    id_idx_t subscription_id = operational_request->subscription_id();
    Telemetry::VerbosityLevel verbosity = operational_request->verbosity();
    Telemetry::KeyValue *kv;

    // Make a note
    _logger->log("getTelemetryOperationalState: ID = " + std::to_string(subscription_id) +
                 " Verbosity = " + std::to_string(verbosity));

    if (subscription_id != 0) {
        if (subscription_id != 0xFFFFFFFF) {
            // Get specific subscription stats
            kv = operational_reply->add_kv();
            kv->set_key("subscription_id");
            kv->set_int_value(subscription_id);
            // Lookup the subscription
            AgentSubscription *sub = AgentSubscription::findSubscription(subscription_id);
            if (!sub) {
                std::string err_str = "Subscription Not Found = " + std::to_string(subscription_id);
                _logger->log(err_str);
                // Lets fill KV as "Error" as value "Subscription Not Found"
                kv = operational_reply->add_kv();
                kv->set_key("error");
                kv->set_str_value("Subscription Not Found");
                return Status::OK;
            }
            // Get all the statistics for this subscription
            sub->getOperational(operational_reply, verbosity);
            return Status::OK;
        } else {
            // Get all subscription stats
            AgentSubscription *sub = AgentSubscription::getFirst();
            while (sub) {
                // Embed subscription id first
                kv = operational_reply->add_kv();
                kv->set_key("subscription_id");
                kv->set_int_value(sub->getId());
                // Get all the statistics for this subscription
                sub->getOperational(operational_reply, verbosity);

                // Move to the next entry
                sub = AgentSubscription::getNext(sub->getId());
            }
        }
    }

    // Agent level stats begin
    kv = operational_reply->add_kv();
    kv->set_key("agent-stats");
    kv->set_str_value("begin");

    // revisit --- TODO
    kv = operational_reply->add_kv();
    kv->set_key("total_subscriptions");
    kv->set_int_value(store.size());

    // Agent level stats end
    kv = operational_reply->add_kv();
    kv->set_key("agent-stats");
    kv->set_str_value("end");

    return Status::OK;
}


Status
AgentServer::getDataEncodings (ServerContext* context, const DataEncodingRequest* data_enc_request, DataEncodingReply* data_enc_reply)
{
    data_enc_reply->add_encoding_list(Telemetry::EncodingType::PROTO3);
    return Status::OK;
}


Status
AgentServer::_sendMetaDataInfo (ServerContext *context,
                                ServerWriter<OpenConfigData>* writer,
                                SubscriptionReply &reply)
{
    // Serialize the data in text format
    std::string init_data;
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(reply, &init_data);
    // std::cout << "String data = " << init_data << std::endl;

    // Send the meta data and log it
    context->AddInitialMetadata("init-response", init_data);
    writer->SendInitialMetadata();
    _logger->log("Subscribe-MetaData-Sent");
    return Status::OK;
}


void
AgentServer::_cleanupSubscription (AgentSubscription *sub)
{
    if (sub == NULL) {
        // Bail out
        return;
    }

    // Cancel the subscription on the bus
    sub->disable();

    // Disconnect and stop the MQTT loop
    sub->disconnect();
    sub->loop_stop();

    // Remove the subscription from the system
    _consolidator.removeRequest(sub->getSystemSubscription());
    
    // Remove the subscription
    AgentSubscription::deleteSubscription(sub->getId());
    _id_manager.deallocate(sub->getId());

    // Turn it off
    sub->setActive(false);
}