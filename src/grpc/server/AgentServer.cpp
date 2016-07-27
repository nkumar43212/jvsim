//
//  AgentServer.cpp
//  Telemetry Agent
//
//  Created: 12/29/15.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

// Header files
#include "AgentServer.h"
#include "AgentServerTransport.hpp"
#include "AgentUtils.hpp"
#include "AgentSubscriptionUdpWorker.hpp"
#include "UdpReceiver.hpp"

Status
AgentServer::telemetrySubscribe (ServerContext *context,
                                 const SubscriptionRequest *request,
                                 ServerWriter<OpenConfigData>* writer)
{
    std::string log_str;
    PathList    final_path_list;
    AgentConsolidatorHandle *system_handle;
    SubscriptionRequest *validated_request;
    SubscriptionRequest *system_accepted_request;
    SubscriptionReply reply;
    SubscriptionResponse *response = reply.mutable_response();

    // Log this event
    log_str = __FUNCTION__;
    log_str.append("->");
    log_str.append(AgentUtils::getMessageString(*request));
    _logger->log(log_str);

    // Validate path list
    if (request->path_list_size() == 0) {
        // Sent id error message and bail out.
        response->set_subscription_id(_id_manager.getNullIdentifier());
        _logger->log("Subscription-stream-end: Error, Path List is Zero");
        return _sendMetaDataInfo(context, writer, reply);
    }

    // Make a note
    _logger->log("Subscription-Begin");

    // Allocate an ID
    id_idx_t id = _id_manager.allocate();
    if (id == _id_manager.getNullIdentifier()) {
        // Sent id error message and bail out.
        response->set_subscription_id(id);
        _logger->log("Subscription-stream-end: Error, ID Exhausted");
        return _sendMetaDataInfo(context, writer, reply);
    }

    // Log the Subscription Identifier
    log_str = std::to_string(id);
    _logger->log("Subscription-Allocate: ID = " + log_str);

    // Validate request against capability (path validator fields)
    // (for now capability is hardcoded in a file)
    validated_request = new SubscriptionRequest();
    if (global_config.validate_ocpaths) {
        telemetry::SubscriptionAdditionalConfig *add_config =
                                new telemetry::SubscriptionAdditionalConfig();
        add_config->CopyFrom(request->additional_config());
        validated_request->set_allocated_additional_config(add_config);
        _logger->log("Validating paths ...");
        for (int i = 0; i < request->path_list_size(); i++) {
            telemetry::Path path = request->path_list(i);
            _path_validator->validate_path(path);
            if (path.ByteSize() != 0) {
                telemetry::Path *p_path = validated_request->add_path_list();
                p_path->CopyFrom(path);
            }
        }
        log_str = "Validated Paths->";
        log_str.append(AgentUtils::getMessageString(*validated_request));
        _logger->log(log_str);
    } else {
        validated_request->CopyFrom(*request);
    }

    // Create a subscription into the system
    system_accepted_request = new SubscriptionRequest();
    telemetry::SubscriptionAdditionalConfig *add_config =
                                new telemetry::SubscriptionAdditionalConfig();
    add_config->CopyFrom(validated_request->additional_config());
    system_accepted_request->set_allocated_additional_config(add_config);
    system_handle = _consolidator.addRequest(id, validated_request,
                                             system_accepted_request);
    log_str = "System Accepted Paths->";
    log_str.append(AgentUtils::getMessageString(*system_accepted_request));
    _logger->log(log_str);
    if (!system_handle) {
        // Delete allocated id
        _id_manager.deallocate(id);
        // Sent id = 0 error message and bail out.
        response->set_subscription_id(_id_manager.getNullIdentifier());
        _logger->log(
                "Subscription-stream-end: Error, Internal System Failure");
        delete validated_request;
        delete system_accepted_request;
        return _sendMetaDataInfo(context, writer, reply);
    }

    // Get all the paths that we are interested in
    log_str = "";
    for (int i = 0; i < system_accepted_request->path_list_size(); i++) {
        final_path_list.push_back(system_accepted_request->path_list(i).path());
        log_str += system_accepted_request->path_list(i).path();
        log_str += ":";
    }
    _logger->log("Final-path-list: " + log_str);

    // Create a subscription
    AgentServerTransport *transport = new AgentServerTransport(context, writer);
    AgentSubscriptionLimits limits(
            system_accepted_request->additional_config().limit_records(),
            system_accepted_request->additional_config().limit_time_seconds());
    AgentSubscription *sub = AgentSubscription::createSubscription(id,
                                                            system_handle,
                                                            transport,
                                                            final_path_list,
                                                            limits,
                                                            _logger);
    if (!sub) {
        // Delete allocated id
        _id_manager.deallocate(id);
        // Sent id = 0 error message and bail out.
        response->set_subscription_id(_id_manager.getNullIdentifier());
        _logger->log(
                "Subscription-stream-end: Error, Subscription Creation Error");
        delete validated_request;
        delete system_accepted_request;
        delete transport;
        return _sendMetaDataInfo(context, writer, reply);
    }

    // Now do things for Udp receiver
    AgentSubscriptionUdpWorker *sub_udp_worker = NULL;
    if (global_config.udp_server_module) {
        sub_udp_worker =
                    AgentSubscriptionUdpWorker::createSubscriptionUdpWorker(id,
                                                    system_handle,
                                                    transport,
                                                    final_path_list);
        // Add worker in udpreceiver
        udpreceiver->add_worker(sub->getId(), sub_udp_worker);

        // Add external to internal list mappings
        for (int i = 0; i < system_handle->getHandleCount(); i++) {
            udpreceiver->add_mapping(sub_udp_worker->getId(),
                    system_handle->getHandle(i)->getInternalSubscriptionId());
        }
    }

    // Send back the response on the metadata channel
    response->set_subscription_id(sub->getId());

    // Send path info back to the client
    for (int i = 0; i < system_accepted_request->path_list_size(); i++) {
        telemetry::Path *p_tpath = reply.add_path_list();
        p_tpath->CopyFrom(system_accepted_request->path_list(i));
    }

    // Don't start MQTT subscription till we send this response
    _sendMetaDataInfo(context, writer, reply);

    // Turn it on (Subscribe)
    sub->enable();
    sub->setActive(true);

    // Wait till the subscription expires or client disconnects or
    // cancel subscription was invoked
    while (!sub->expired() && !sub->getClientDisconnects() && sub->getActive()){
        sleep(1);
    }

    // Guard the add request
    std::lock_guard<std::mutex> guard(_delete_initiate_mutex);

    // Streaming over. Handle either client disconnects or sub expired
    // Subscription cancel was already handled
    if (sub->getActive()) {
        if (sub->expired() || sub->getClientDisconnects()) {
            _logger->log("Channel disconnected or Subscription expired: ID = " +
                 std::to_string(id) + ", Expired : " + 
                 std::to_string(sub->expired())  + ", Client Disconnects : "+
                 std::to_string(sub->getClientDisconnects()));
            // cleanup udp worker subscription gracefully
            if (global_config.udp_server_module) {
                _cleanupSubscriptionUdpWorker(sub_udp_worker);
            }

            // cleanup subscription gracefully
            _cleanupSubscription(sub);
        }
    }

    log_str = std::to_string(sub->getId());
    _logger->log("Subscription-stream-end: ID = " + log_str);


    // Delete validated_request
    delete validated_request;

    // Delete system_accepted_request
    delete system_accepted_request;

    // Delete transport object
    delete transport;

    // Delete udp worker subscriber object
    if (global_config.udp_server_module) {
        delete sub_udp_worker;
    }

    // Delete subscriber object
    delete sub;

    return Status::OK;
}


Status
AgentServer::cancelTelemetrySubscription (ServerContext* context,
                                const CancelSubscriptionRequest* cancel_request,
                                CancelSubscriptionReply* cancel_reply)
{
    std::string log_str;

    // Log this event
    log_str = __FUNCTION__;
    log_str.append("->");
    log_str.append(AgentUtils::getMessageString(*cancel_request));
    _logger->log(log_str);

    // Guard the add request
    std::lock_guard<std::mutex> guard(_delete_initiate_mutex);

    // Lookup the udp worker subscription
    if (global_config.udp_server_module) {
        AgentSubscriptionUdpWorker *sub_udp_worker =
                    AgentSubscriptionUdpWorker::findSubscription(
                                            cancel_request->subscription_id());
        if (!sub_udp_worker) {
            std::string err_str = "UDP Subscription Not Found. ID = " +
            std::to_string(cancel_request->subscription_id());
            cancel_reply->set_code(telemetry::ReturnCode::NO_SUBSCRIPTION_ENTRY);
            cancel_reply->set_code_str(err_str);
            _logger->log(err_str);
            return Status::OK;
        }

        // cleanup udp worker subscription gracefully
        _cleanupSubscriptionUdpWorker(sub_udp_worker);
    }

    // Lookup the subscription
    AgentSubscription *sub = AgentSubscription::findSubscription(
                                            cancel_request->subscription_id());
    if (!sub) {
        std::string err_str = "Subscription Not Found. ID = " +
                            std::to_string(cancel_request->subscription_id());
        cancel_reply->set_code(telemetry::ReturnCode::NO_SUBSCRIPTION_ENTRY);
        cancel_reply->set_code_str(err_str);
        _logger->log(err_str);
        return Status::OK;
    }

    // cleanup subscription gracefully
    _cleanupSubscription(sub);

    // Note, do not delete here, leave it to subscription thread
    // delete sub;

    cancel_reply->set_code(telemetry::SUCCESS);
    cancel_reply->set_code_str("Subscription Successfully Deleted");
    _logger->log("cancelTelemetrySubscriptionEnd: ID = " +
                 std::to_string(cancel_request->subscription_id()));

    return Status::OK;
}


Status
AgentServer::getTelemetrySubscriptions (ServerContext* context,
                            const GetSubscriptionsRequest* get_request,
                            GetSubscriptionsReply* get_reply)
{
    std::string log_str;
    AgentSubscription *sub;
    id_idx_t subscription_id = get_request->subscription_id();

    // Log this event
    log_str = __FUNCTION__;
    log_str.append("->");
    log_str.append(AgentUtils::getMessageString(*get_request));
    _logger->log(log_str);

    if (subscription_id != 0xFFFFFFFF) {
        // Lookup the subscription
        sub = AgentSubscription::findSubscription(subscription_id);
        if (!sub) {
            _logger->log("Subcription Not Found. ID = " +
                         std::to_string(subscription_id));
            return Status::OK;
        } else {
            SubscriptionReply *sub_reply;
            sub_reply = get_reply->add_subscription_list();
            SubscriptionResponse *sub_resp = sub_reply->mutable_response();
            sub_resp->set_subscription_id(sub->getId());

            // Grab the consolidator handle for the subsription
            AgentConsolidatorHandle *conHandle = sub->getSystemSubscription();

            // Iterate through the handle
            for (int i = 0; i < conHandle->getHandleCount(); i++) {
                AgentConsolidatorSystemHandlePtr ptr = conHandle->getHandle(i);
                if (!ptr) {
                    continue;
                }

                // Return the local state in the consolidator
                telemetry::Path *path = sub_reply->add_path_list();
                path->CopyFrom(*ptr->getPath());
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

            // Grab the consolidator handle for the subsription
            AgentConsolidatorHandle *conHandle = sub->getSystemSubscription();

            // Iterate through the handle
            for (int i = 0; i < conHandle->getHandleCount(); i++) {
                AgentConsolidatorSystemHandlePtr ptr = conHandle->getHandle(i);
                if (!ptr) {
                    continue;
                }

                // Return the local state in the consolidator
                telemetry::Path *path = sub_reply->add_path_list();
                path->CopyFrom(*ptr->getPath());
            }

            // Move to the next entry
            sub = AgentSubscription::getNext(sub->getId());
        }
    }

    return Status::OK;
}


Status
AgentServer::getTelemetryOperationalState (ServerContext* context,
                        const GetOperationalStateRequest* operational_request,
                        GetOperationalStateReply* operational_reply)
{
    std::string log_str;
    id_idx_t subscription_id = operational_request->subscription_id();
    telemetry::VerbosityLevel verbosity = operational_request->verbosity();
    telemetry::KeyValue *kv;

    // Log this event
    log_str = __FUNCTION__;
    log_str.append("->");
    log_str.append(AgentUtils::getMessageString(*operational_request));
    _logger->log(log_str);

    if (subscription_id != 0) {
        if (subscription_id != 0xFFFFFFFF) {
            // Get specific subscription stats
            kv = operational_reply->add_kv();
            kv->set_key("subscription_id");
            kv->set_int_value(subscription_id);
            // Lookup the subscription
            AgentSubscription *sub =
                        AgentSubscription::findSubscription(subscription_id);
            if (!sub) {
                std::string err_str = "Subscription Not Found. ID = " +
                std::to_string(subscription_id);
                _logger->log(err_str);
                // Lets fill KV as "Error" as value "Subscription Not Found"
                kv = operational_reply->add_kv();
                kv->set_key("error");
                kv->set_str_value("Subscription Not Found");
                return Status::OK;
            }
            // Get all the statistics for this subscription
            sub->getOperational(operational_reply, verbosity);

            // Now lookup the udp worker subscription
            if (global_config.udp_server_module) {
                AgentSubscriptionUdpWorker *sub_udp_worker =
                AgentSubscriptionUdpWorker::findSubscription(subscription_id);
                if (!sub_udp_worker) {
                    std::string err_str = "UDP Subscription Not Found. ID = " +
                    std::to_string(subscription_id);
                    _logger->log(err_str);
                    kv = operational_reply->add_kv();
                    kv->set_key("error");
                    kv->set_str_value("UDP Subscription Not Found");
                    return Status::OK;
                }
                // Get all the statistics for this subscription
                sub_udp_worker->getOperational(operational_reply, verbosity);
            }
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

                // Now lookup the udp worker subscription
                if (global_config.udp_server_module) {
                    AgentSubscriptionUdpWorker *sub_udp_worker =
                     AgentSubscriptionUdpWorker::findSubscription(sub->getId());
                    if (!sub_udp_worker) {
                        std::string err_str = "UDP Subscription Not Found. ID = "
                                             + std::to_string(subscription_id);
                        _logger->log(err_str);
                        kv = operational_reply->add_kv();
                        kv->set_key("error");
                        kv->set_str_value("UDP Subscription Not Found");
                    } else {
                        // Get all the statistics for this subscription
                        sub_udp_worker->getOperational(operational_reply,
                                                       verbosity);
                    }
                }

                // Move to the next entry
                sub = AgentSubscription::getNext(sub->getId());
            }
        }
    }

    // Agent level stats begin
    kv = operational_reply->add_kv();
    kv->set_key("agent-stats");
    kv->set_str_value("begin");

    // TODO ABBAS --- revisit
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
AgentServer::getDataEncodings (ServerContext* context,
                               const DataEncodingRequest* data_enc_request,
                               DataEncodingReply* data_enc_reply)
{
    std::string log_str;

    // Log this event
    log_str = __FUNCTION__;
    log_str.append("->");
    log_str.append(AgentUtils::getMessageString(*data_enc_request));
    _logger->log(log_str);

    data_enc_reply->add_encoding_list(telemetry::EncodingType::PROTO3);
    return Status::OK;
}


Status
AgentServer::_sendMetaDataInfo (ServerContext *context,
                                ServerWriter<OpenConfigData>* writer,
                                SubscriptionReply &reply)
{
    // Serialize the data in text format
    std::string init_data = AgentUtils::getMessageString(reply);
    // std::cout << "String data = " << init_data << std::endl;

    // Send the meta data and log it
    context->AddInitialMetadata("init-response", init_data);
    writer->SendInitialMetadata();
    _logger->log("Subscribe-MetaData-Sent: ID = " +
                 std::to_string(reply.response().subscription_id()));
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
    sub->loop_stop(true); // Force loop termination --- TODO ABBAS

    // Remove the subscription from the system
    _consolidator.removeRequest(sub->getSystemSubscription());

    // Remove the subscription
    AgentSubscription::deleteSubscription(sub->getId());
    _id_manager.deallocate(sub->getId());

    // Turn it off
    sub->setActive(false);
}

void
AgentServer::_cleanupSubscriptionUdpWorker (
                                AgentSubscriptionUdpWorker *sub_udp_worker)
{
    if (sub_udp_worker == NULL) {
        // Bail out
        return;
    }

    // Cancel the subscription on the udp receiver
    for (int i = 0;
         i < sub_udp_worker->getSystemSubscription()->getHandleCount(); i++) {
        udpreceiver->del_mapping(sub_udp_worker->getId(),
                                 sub_udp_worker->getSystemSubscription()->
                                    getHandle(i)->getInternalSubscriptionId());
    }

    // Remove worker thread from udp receiver
    udpreceiver->del_worker(sub_udp_worker->getId());
    sub_udp_worker->setTerminate();
    std::thread *sub_udp_worker_thr = sub_udp_worker->getThread();

    if (sub_udp_worker_thr->joinable())
        sub_udp_worker_thr->join();

    // Remove the udp worker subscription
    AgentSubscriptionUdpWorker::deleteSubscription(sub_udp_worker->getId());
}
