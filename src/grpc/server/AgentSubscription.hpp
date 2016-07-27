//
//  AgentSubscription.hpp
//  Telemetry Agent
//
//  Created: 1/19/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSubscription_hpp
#define AgentSubscription_hpp

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "AgentServerIdManager.hpp"
#include "AgentMessageBus.hpp"
#include "AgentServerTransport.hpp"
#include "AgentSubscriptionLimits.hpp"
#include "AgentConsolidator.hpp"

// List of Paths
typedef std::vector<std::string> PathList;

// Store for all subscriptions
class AgentSubscription;
extern std::map<id_idx_t, AgentSubscription *> store;


class AgentSubscription : public MessageBus {
    // Subscription Identifier
    id_idx_t       _id;

    // Path list
    PathList       _path_list;

    // Transport Handle
    AgentServerTransport *_transport;

    // Consolidation handle that tracks the subscription request towards
    // JUNOS system
    AgentConsolidatorHandle *_system_subscription;

    // Misc subscription statistics
    uint64_t      _oc_lookup_failures;
    uint64_t      _stream_alloc_failures;
    uint64_t      _stream_parse_failures;

    // Current state of subscription
    bool          _active;

public:

    // Accessors
    id_idx_t    getId()                              { return _id;        }
    std::string getName()                            { return _name;      }
    PathList    getPathList()                        { return _path_list; }
    bool        getActive()                          { return _active;    }
    AgentConsolidatorHandle *getSystemSubscription()
                                           { return _system_subscription; }
    AgentServerTransport* getTransport()             { return _transport; }
    bool        getClientDisconnects(void);
    void        getOperational(GetOperationalStateReply* operational_reply,
                               telemetry::VerbosityLevel verbosity);

    // Construction
    AgentSubscription (std::string name,
                       AgentServerTransport *transport,
                       AgentSubscriptionLimits limits,
                       AgentServerLog *logger) :
                       MessageBus(name, limits, logger),
                       _transport(transport)
    {
        _oc_lookup_failures = _stream_alloc_failures = _stream_parse_failures = 0;
    }

    ~AgentSubscription () {
        _path_list.clear();
    }

    static AgentSubscription* createSubscription (id_idx_t id,
                                    AgentConsolidatorHandle *system_handle,
                                    AgentServerTransport *transport,
                                    PathList path_list,
                                    AgentSubscriptionLimits limits,
                                    AgentServerLog *logger,
                                    char *name = NULL)
    {
        std::string client_name = name ? name : "client-" + std::to_string(id);

        // Allocate the object
        AgentSubscription *sub;
        sub = new AgentSubscription(client_name, transport, limits, logger);
        if (!sub) {
            return NULL;
        }

        sub->_system_subscription = system_handle;
        sub->_id                  = id;
        sub->_path_list           = path_list;
        store[sub->_id]           = sub;

        return sub;
    }

    static void deleteSubscription (id_idx_t id)
    {
        std::map<id_idx_t, AgentSubscription *>::iterator itr = store.find(id);

        // Did we find it ?
        if (itr != store.end()) {
            store.erase(itr);
        }
    }

    static AgentSubscription* findSubscription (id_idx_t id)
    {
        if (store.count(id) == 0) {
            return NULL;
        }

        return store[id];
    }

    static AgentSubscription *getFirst ()
    {
        return store.begin() != store.end() ? store.begin()->second : NULL;
    }
    
    static AgentSubscription *getNext (id_idx_t id)
    {
        std::map<id_idx_t, AgentSubscription *>::iterator itr;

        itr = store.upper_bound(id);
        if (itr != store.end()) {
            return itr->second;
        }

        return NULL;
    }

    void enable ()
    {
        // Connect to the message bus and then start subscribing for topics
        BusConnect();

#if 0
        // Add all the corresponding subscriptions
        if (global_config.subscribe_topic_name == TOPIC_PATH) {
            for (PathList::iterator itr = _path_list.begin();
                 itr != _path_list.end(); itr++) {
                Subscribe(*itr);
            }
        } else if (global_config.subscribe_topic_name 
                                                == TOPIC_INTERNAL_SUB_ID) {
            int total_handles = _system_subscription->getHandleCount();
            for (int i = 0; i < total_handles; i++) {
                AgentConsolidatorSystemHandlePtr csh =
                                            _system_subscription->getHandle(i);
                id_idx_t isubid = csh->getInternalSubscriptionId();
                Subscribe("/" + std::to_string(isubid));
           }
        }
#endif
    }

    void disable ()
    {
        // Delete the corresponding subscriptions
        if (global_config.subscribe_topic_name == TOPIC_PATH) {
            for (PathList::iterator itr = _path_list.begin();
                 itr != _path_list.end();
                 itr++) {
                unSubscribe(*itr);
            }
        } else if (global_config.subscribe_topic_name == TOPIC_INTERNAL_SUB_ID) {
            int total_handles = _system_subscription->getHandleCount();
            for (int i = 0; i < total_handles; i++) {
                AgentConsolidatorSystemHandlePtr csh =
                                            _system_subscription->getHandle(i);
                id_idx_t isubid = csh->getInternalSubscriptionId();
                unSubscribe("/" + std::to_string(isubid));
            }
        }
    }

    bool expired ()
    {
        return _limits_reached;
    }

    void setActive (bool value)
    {
        _active = value;
    }

    void _count_message(const struct mosquitto_message* mosqmessage);
    virtual void on_message(const struct mosquitto_message* mosqmessage);

    void on_connect (int rc);
};

#endif /* AgentSubscription_hpp */
