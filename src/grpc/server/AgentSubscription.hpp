//
//  AgentSubscription.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSubscription_hpp
#define AgentSubscription_hpp

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "AgentMessageBus.hpp"
#include "AgentServerTransport.hpp"
#include "AgentSubscriptionLimits.hpp"

// List of Paths
typedef std::vector<std::string> PathList;

// Store for all subscriptions
class AgentSubscription;
extern std::map<uint32_t, AgentSubscription *> store;


class AgentSubscription : public MessageBus {
    // Subscription Identifier
    uint32_t       _id;
    
    // Name to identify the subscription
    std::string    _name;
    PathList       _path_list;
    
    // Transport Handle
    AgentServerTransport _transport;
    
    // Misc subscription statistics
    uint64_t       _oc_lookup_failures;
    uint64_t       _stream_alloc_failures;
    uint64_t       _stream_parse_failures;
    
    // Current state of subscription
    bool          _active;
   
public:
    
    // Accessors
    uint32_t    getId()       { return _id;        }
    std::string getName()     { return _name;      }
    PathList    getPathList() { return _path_list; }
    bool        getActive()   { return _active;    }
    uint32_t    getErrorId()  { return getErrorIdentifier(); }
    
    // Construction
    AgentSubscription (std::string name,
                       AgentServerTransport transport,
                       AgentSubscriptionLimits limits) : MessageBus(name, limits), _transport(transport)
    {
        _oc_lookup_failures = _stream_alloc_failures = _stream_parse_failures = 0;
    }
    
    static AgentSubscription* createSubscription (AgentServerTransport transport,
                                                  PathList path_list,
                                                  AgentSubscriptionLimits limits,
                                                  char *name = NULL)
    {
        uint32_t id             = allocateIdentifier();
        std::string client_name = name ? name : "client-" + std::to_string(id);
        
        AgentSubscription *sub;
        sub = new AgentSubscription(client_name, transport, limits);
        if (!sub) {
            return NULL;
        }
        
        sub->_id        = id;
        sub->_name      = client_name;
        sub->_path_list = path_list;
        store[sub->_id] = sub;
        
        return sub;
    }
    
    static void               deleteSubscription (uint32_t id)
    {
        std::map<uint32_t, AgentSubscription *>::iterator itr = store.find(id);
        
        if (itr != store.end()) {
            store.erase(itr);
        }
    }
    
    static AgentSubscription* findSubscription (uint32_t id)
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
    
    static AgentSubscription *getNext (uint32_t id)
    {
        std::map<uint32_t, AgentSubscription *>::iterator itr;
        
        itr = store.upper_bound(id);
        if (itr != store.end()) {
            return itr->second;
        }
        
        return NULL;
    }
    
    void enable ()
    {
        // Add all the corresponding subscriptions
        for (PathList::iterator itr = _path_list.begin(); itr != _path_list.end(); itr++) {
            Subscribe(*itr);
        }
        _active = true;
    }
    
    void disable ()
    {
        // Add all the corresponding subscriptions
        for (PathList::iterator itr = _path_list.begin(); itr != _path_list.end(); itr++) {
            unSubscribe(*itr);
        }
        _active = false;
    }
    
    bool expired ()
    {
        return _limits_reached;
    }
    
    void setActive (bool value)
    {
        _active = value;
    }
    
    void getOperational (OpenConfigData *datap, uint32_t verbosity = 0)
    {
        // Get stats from the message bus interface
        MessageBus::getOperational(datap, verbosity);

        // If verbose mose is not set, we are done
        if (!verbosity) {
            return;
        }
        
        // Failures
        KeyValue *kv;
        kv = datap->add_kv();
        kv->set_key("translation_failures");
        kv->set_int_value(_oc_lookup_failures);
        
        kv = datap->add_kv();
        kv->set_key("stream_open_failures");
        kv->set_int_value(_stream_alloc_failures);
        
        kv = datap->add_kv();
        kv->set_key("stream_parse_failures");
        kv->set_int_value(_stream_parse_failures);
    }
    
    virtual void on_message(const struct mosquitto_message* mosqmessage);
    
    static uint32_t allocateIdentifier();
    static uint32_t getErrorIdentifier();
};

#endif /* AgentSubscription_hpp */
