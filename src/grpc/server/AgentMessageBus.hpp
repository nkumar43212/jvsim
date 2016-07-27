//
//  AgentMessageBus.hpp
//  Telemetry Agent
//
//  Created: 1/19/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentMessageBus_hpp
#define AgentMessageBus_hpp

#include <map>
#include <string>
#include <mosquittopp.h>

#include "AgentServerProtos.h"
#include "AgentSubscriptionLimits.hpp"
#include "Counter.hpp"
#include "AgentServerLog.hpp"
#include "GlobalConfig.hpp"

typedef std::map<const std::string, Counter>        topicCounterMap;

class Mqtt: public mosqpp::mosquittopp {

public:
    // Identify the name of Mqtt subscriber
    std::string _name;

    AgentSubscriptionLimits _limits;
    bool _limits_reached;

    uint64_t stats_connect;
    uint64_t stats_disconnect;
    uint64_t stats_subscribes;
    uint64_t stats_unsubscribes;
    Counter  messages;
    topicCounterMap stats_topics;

    // Log reference
    AgentServerLog *_logger;

    Mqtt(const std::string name, AgentSubscriptionLimits limits,
         AgentServerLog *logger) :
        mosquittopp::mosquittopp (name.c_str()),
        _name(name),
        _logger(logger),
        messages("MessageCount-" + name, true)
    {
        _limits = limits;
        _limits_reached = false;
        stats_connect = stats_disconnect = 0;
        stats_subscribes = stats_unsubscribes = 0;
    }

    ~Mqtt ()
    {
        messages.disableRate();
    }

    virtual void on_connect (int rc)
    {
        stats_connect++;
        _logger->log("Subscription " + _name + " MQTT bus connected.");
    }

    virtual void on_disconnect (int rc)
    {
        stats_disconnect++;
        _logger->log("Subscription " + _name + " MQTT bus disconnected.");
    }

    virtual void on_subscribe (int mid, int qos_count, const int *granted_qos)
    {
        stats_subscribes++;
    }

    virtual void on_unsubscribe(int mid)
    {
        stats_unsubscribes++;
    }

    // Unused now
    virtual void on_message (const struct mosquitto_message* mosqmessage)
    {
        messages.increment(1, mosqmessage->payloadlen);
        if (stats_topics.count(mosqmessage->topic) == 0) {
            stats_topics[mosqmessage->topic] = Counter(mosqmessage->topic);
        }

        stats_topics[mosqmessage->topic].increment(1, mosqmessage->payloadlen);
        if (_limits.expired(messages.getPackets())) {
            _logger->log("Subscription " + _name + " limits expired");
            _limits_reached = true;
        }
    }

    // Topic supplied here was converted to path name for isid mode
    void on_message_count (std::string topic,
                           const struct mosquitto_message* mosqmessage)
    {
        // Global
        messages.increment(1, mosqmessage->payloadlen);

        // Per-topic
        if (stats_topics.count(topic) == 0) {
            stats_topics[topic] = Counter(topic);
        }
        stats_topics[topic].increment(1, mosqmessage->payloadlen);

        // Check limit
        if (_limits.expired(messages.getPackets())) {
            _logger->log("Subscription " + _name + " limits expired");
            _limits_reached = true;
        }
    }

    void getOperational (GetOperationalStateReply* operational_reply,
                         telemetry::VerbosityLevel verbosity)
    {
        telemetry::KeyValue *kv;

        // Total Message Count
        kv = operational_reply->add_kv();
        kv->set_key("mqtt-total_message_count");
        kv->set_int_value(messages.getPackets());
        kv = operational_reply->add_kv();
        kv->set_key("mqtt-total_message_count_rate");
        kv->set_int_value(messages.getPacketRate());

        kv = operational_reply->add_kv();
        kv->set_key("mqtt-total_message_bytes");
        kv->set_int_value(messages.getBytes());
        kv = operational_reply->add_kv();
        kv->set_key("mqtt-total_message_byte_rate");
        kv->set_int_value(messages.getByteRate());

        // Continue only if verbose output is desired
        if (verbosity == telemetry::VerbosityLevel::TERSE) {
            return;
        }

        // Broker Connections
        kv = operational_reply->add_kv();
        kv->set_key("mqtt-message_bus_connects");
        kv->set_int_value(stats_connect);

        kv = operational_reply->add_kv();
        kv->set_key("mqtt-message_bus_disconnects");
        kv->set_int_value(stats_disconnect);

        // All topic subscriptions
        for (topicCounterMap::iterator itr = stats_topics.begin();
             itr != stats_topics.end(); itr++) {
            kv = operational_reply->add_kv();
            kv->set_key("mqtt-packets:path:" + itr->first);
            kv->set_int_value(itr->second.getPackets());

#if 0 /* unsupported for now ... discussed with Nitin */
            kv = operational_reply->add_kv();
            kv->set_key("mqtt-packet_rates:path:" + itr->first);
            kv->set_int_value(itr->second.getPacketRate());
#endif

            kv = operational_reply->add_kv();
            kv->set_key("mqtt-bytes:path:" + itr->first);
            kv->set_int_value(itr->second.getBytes());

#if 0 /* unsupported for now ... discussed with Nitin */
            kv = operational_reply->add_kv();
            kv->set_key("mqtt-byte_rates:path:" + itr->first);
            kv->set_int_value(itr->second.getByteRate());
#endif
        }
    }
};


class MessageBus : public Mqtt {
   public:
    MessageBus (const std::string client_name,
                AgentSubscriptionLimits limits,
                AgentServerLog *logger) :
                Mqtt(client_name, limits, logger)
    {
    }

    void Subscribe (const std::string resource)
    {
        _logger->log("Subscription " + _name +
                     " MQTT subscribe path: " + resource);
        subscribe(0, resource.c_str());
    }

    void unSubscribe (const std::string resource)
    {
        _logger->log("Subscription " + _name +
                     " MQTT unsubscribe path: " + resource);
        unsubscribe(0, resource.c_str());
    }

    void BusConnect (void)
    {
        _logger->log("Subscription " + _name +
                     " initiating MQTT bus connection.");
        connect(global_config.mqtt_broker_ip.c_str(),
                global_config.mqtt_broker_port);
        loop_start();
    }
};

#endif /* AgentMessageBus_hpp */
