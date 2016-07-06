//
//  AgentSubscriptionUdpWorker.hpp
//  Telemetry Agent
//
//  Created: 5/11/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSubscriptionUdpWorker_hpp
#define AgentSubscriptionUdpWorker_hpp

#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <map>
#include <thread>
#include "AgentServerIdManager.hpp"
#include "AgentServerTransport.hpp"
#include "AgentConsolidatorSystem.hpp"
#include "Counter.hpp"

// List of Paths
typedef std::vector<std::string> PathList;

// Store for all subscriptions
class AgentSubscriptionUdpWorker;
extern std::map<id_idx_t, AgentSubscriptionUdpWorker *> store2;

// Counter
typedef std::map<const std::string, Counter>        topicCounterMap;

// AgentSubscriptionUdpWorker definition
class AgentSubscriptionUdpWorker {
private:
    std::mutex                  _m;
    std::condition_variable     _c;
    std::queue<std::vector<unsigned char>>     _q;
    bool                        _dont_terminate;

    // Subscription Identifier
    id_idx_t       _id;

    // Name to identify the subscription
    std::string    _name;
    PathList       _path_list;

    // Transport Handle
    AgentServerTransport *_transport;

    // Consolidation handle that tracks the subscription request towards
    // JUNOS system
    AgentConsolidatorHandle *_system_subscription;

    // test code
    std::thread     *_sub_udp_worker_thr;
    
    // Counter
    uint64_t        _total_pkt_recvd;
    
    Counter         _messages;
    topicCounterMap stats_topics;

    // Reader may write into our queue
    friend class UdpReceiver;

public:
    AgentSubscriptionUdpWorker(id_idx_t id,
                               AgentConsolidatorHandle *system_handle,
                               AgentServerTransport *transport,
                               PathList path_list):
                               _id(id),
                               _system_subscription(system_handle),
                               _transport(transport),
                               _path_list(path_list),
                               _name("client-" + std::to_string(_id)),
                               _messages("MessageCount-" + _name, true)
    {
        _dont_terminate = true;
        _total_pkt_recvd = 0;
        _sub_udp_worker_thr = new std::thread([this]() { (*this)(); });
    };

    ~AgentSubscriptionUdpWorker() {
        _messages.disableRate();
    }

    void setTerminate (void) {
        _dont_terminate = false;
        _c.notify_one();
    }

    void operator()();
    
    id_idx_t    getId()                              { return _id;        }
    std::string getName()                            { return _name;      }
    PathList    getPathList()                        { return _path_list; }
    AgentConsolidatorHandle *getSystemSubscription()
                                           { return _system_subscription; }
    std::thread *getThread()               { return _sub_udp_worker_thr; }
    void        getOperational(GetOperationalStateReply* operational_reply,
                               telemetry::VerbosityLevel verbosity);
    void        _getOperational_path(GetOperationalStateReply* operational_reply,
                                     telemetry::VerbosityLevel verbosity);

    static AgentSubscriptionUdpWorker* createSubscriptionUdpWorker (id_idx_t id,
                                        AgentConsolidatorHandle *system_handle,
                                        AgentServerTransport *transport,
                                        PathList path_list,
                                        char *name = NULL)
    {
        std::string client_name = name ? name : "client-" + std::to_string(id);

        // Allocate the object
        AgentSubscriptionUdpWorker *sub_udp_worker =
                        new AgentSubscriptionUdpWorker(id,
                                                       system_handle,
                                                       transport,
                                                       path_list);
        if (!sub_udp_worker) {
            return NULL;
        }

        // Store in global storage
        store2[sub_udp_worker->_id]           = sub_udp_worker;

        return sub_udp_worker;
    }

    static void deleteSubscription (id_idx_t id)
    {
        std::map<id_idx_t, AgentSubscriptionUdpWorker *>::iterator itr =
                                                                store2.find(id);

        // Did we find it ?
        if (itr != store2.end()) {
            store2.erase(itr);
        }
    }

    static AgentSubscriptionUdpWorker* findSubscription (id_idx_t id)
    {
        if (store2.count(id) == 0) {
            return NULL;
        }

        return store2[id];
    }

    static AgentSubscriptionUdpWorker *getFirst ()
    {
        return store2.begin() != store2.end() ? store2.begin()->second : NULL;
    }

    static AgentSubscriptionUdpWorker *getNext (id_idx_t id)
    {
        std::map<id_idx_t, AgentSubscriptionUdpWorker *>::iterator itr;

        itr = store2.upper_bound(id);
        if (itr != store2.end()) {
            return itr->second;
        }

        return NULL;
    }
};

#endif /* AgentSubscriptionUdpWorker_hpp */