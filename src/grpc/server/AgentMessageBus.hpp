//
//  AgentMessageBus.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/19/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentMessageBus_hpp
#define AgentMessageBus_hpp

#include <iostream>
#include <map>
#include <string>
#include <mosquittopp.h>

#include "AgentServerProtos.h"
#include "AgentSubscriptionLimits.hpp"
#include "Counter.hpp"

class Mqtt: public mosqpp::mosquittopp {

public:
    AgentSubscriptionLimits _limits;
    bool _limits_reached;
 
    uint64_t stats_connect;
    uint64_t stats_disconnect;
    uint64_t stats_subscribes;
    uint64_t stats_unscubscribes;
    Counter  messages;
    std::map<const std::string, Counter> stats_topics;

    Mqtt(const std::string name, AgentSubscriptionLimits limits) : mosquittopp::mosquittopp (name.c_str()), messages("MessageCount", true)
    {
        _limits = limits;
        _limits_reached = false;
        stats_connect = stats_disconnect = 0;
        stats_subscribes = stats_unscubscribes = 0;
    }
    
    ~Mqtt ()
    {
        messages.disableRate();
    }
    
    virtual void on_connect (int rc)
    {
        stats_connect++;
    }
    
    virtual void on_disconnect (int rc)
    {
        stats_disconnect++;
    }
    
    virtual void on_subscribe (int mid, int qos_count, const int *granted_qos)
    {
        stats_subscribes++;
    }
    
    virtual void on_unsubscribe(int mid)
    {
        stats_unscubscribes++;
    }
    
    virtual void on_message (const struct mosquitto_message* mosqmessage)
    {
        messages.increment(1, mosqmessage->payloadlen);
        if (stats_topics.count(mosqmessage->topic) == 0) {
            stats_topics[mosqmessage->topic] = Counter(mosqmessage->topic);
        }
        
        stats_topics[mosqmessage->topic].increment(1, mosqmessage->payloadlen);
        if (_limits.expired(messages.getPackets())) {
            _limits_reached = true;
        }
    }
    
    void getOperational (GetOperationalStateReply* operational_reply, Telemetry::VerbosityLevel verbosity)
    {
        Telemetry::KeyValue *kv;

        // Total Message Count
        kv = operational_reply->add_kv();
        kv->set_key("total_message_count");
        kv->set_int_value(messages.getPackets());
        kv = operational_reply->add_kv();
        kv->set_key("total_message_count_rate");
        kv->set_int_value(messages.getPacketRate());

        kv = operational_reply->add_kv();
        kv->set_key("total_message_bytes");
        kv->set_int_value(messages.getBytes());
        kv = operational_reply->add_kv();
        kv->set_key("total_message_byte_rate");
        kv->set_int_value(messages.getByteRate());

        // Continue only if verbose output is desired
        if (verbosity == Telemetry::VerbosityLevel::TERSE) {
            return;
        }

        // Broker Connections
        kv = operational_reply->add_kv();
        kv->set_key("message_bus_connects");
        kv->set_int_value(stats_connect);

        kv = operational_reply->add_kv();
        kv->set_key("message_bus_disconnects");
        kv->set_int_value(stats_disconnect);

        // All topic subscriptions
        for (std::map<const std::string, Counter>::iterator itr = stats_topics.begin();
             itr != stats_topics.end(); itr++) {
            kv = operational_reply->add_kv();
            kv->set_key("packets:xpath:" + itr->first);
            kv->set_int_value(itr->second.getPackets());

            kv = operational_reply->add_kv();
            kv->set_key("packet_rates:xpath:" + itr->first);
            kv->set_int_value(itr->second.getPacketRate());

            kv = operational_reply->add_kv();
            kv->set_key("bytes:xpath:" + itr->first);
            kv->set_int_value(itr->second.getBytes());

            kv = operational_reply->add_kv();
            kv->set_key("byte_rates:xpath:" + itr->first);
            kv->set_int_value(itr->second.getByteRate());
        }
    }
};


class MessageBus : public Mqtt {
   public:
    MessageBus (const std::string client_name,
                AgentSubscriptionLimits limits) : Mqtt(client_name, limits)
    {
        connect("127.0.0.1");
        loop_start();
    }

    void Subscribe (const std::string resource)
    {
        subscribe(0, resource.c_str());
    }

    void unSubscribe (const std::string resource)
    {
        unsubscribe(0, resource.c_str());
    }
};

#endif /* AgentMessageBus_hpp */