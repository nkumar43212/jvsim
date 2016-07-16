//
//  AgentSubscriptionUdpWorker.cpp
//  Telemetry Agent
//
//  Created: 5/11/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "AgentSubscriptionUdpWorker.hpp"

// Store of all active subscriptions
std::map<id_idx_t, AgentSubscriptionUdpWorker *> store2;

void
AgentSubscriptionUdpWorker::operator()()
{
    // Loop until not asked to terminate
    while (_dont_terminate) {
        std::unique_lock<std::mutex> l(_m);
        while (_q.empty() && _dont_terminate) {
            _c.wait(l);
        }
        // TODO ABBAS --- revisit if needed till debugging
        int curr_cnt = 0;
        if (!_q.empty() && curr_cnt < 10) {
            {
                // std::cerr << _q.size()  << "#" << _id << " "
                // << _q.front() << "#" << _q.front().size() << std::endl;
                _messages.increment(1, _q.front().size());
                _total_pkt_recvd++;

                // TODO ABBAS - ----- ????
                OpenConfigData *oc_data = new OpenConfigData;

                // DeSerialize
                oc_data->ParseFromArray(_q.front().data(),(int)_q.front().size());

                // Send it over to the server via the transport channel
                if (_dont_terminate) {
                    if (_transport) {
                        _transport->write(oc_data);
                    }
                }
            }
            _q.pop();
            curr_cnt++;
        }
    }
}
void
AgentSubscriptionUdpWorker::_getOperational_path (
                             GetOperationalStateReply* operational_reply,
                             telemetry::VerbosityLevel verbosity)
{
    telemetry::KeyValue *kv;
    // All topic subscriptions
    for (topicCounterMap::iterator itr = stats_topics.begin();
         itr != stats_topics.end(); itr++) {
        kv = operational_reply->add_kv();
        kv->set_key("udp-packets:path:" + itr->first);
        kv->set_int_value(itr->second.getPackets());

        kv = operational_reply->add_kv();
        kv->set_key("udp-packet_rates:path:" + itr->first);
        kv->set_int_value(itr->second.getPacketRate());

        kv = operational_reply->add_kv();
        kv->set_key("udp-bytes:path:" + itr->first);
        kv->set_int_value(itr->second.getBytes());

        kv = operational_reply->add_kv();
        kv->set_key("udp-byte_rates:path:" + itr->first);
        kv->set_int_value(itr->second.getByteRate());
    }
}

void
AgentSubscriptionUdpWorker::getOperational (
                            GetOperationalStateReply* operational_reply,
                            telemetry::VerbosityLevel verbosity)
{
    // Total Message Count
    telemetry::KeyValue *kv;
    kv = operational_reply->add_kv();
    kv->set_key("udp-total_pkt_received");
    kv->set_int_value(_total_pkt_recvd);

    kv = operational_reply->add_kv();
    kv->set_key("udp-total_message_count");
    kv->set_int_value(_messages.getPackets());

    kv = operational_reply->add_kv();
    kv->set_key("udp-total_message_count_rate");
    kv->set_int_value(_messages.getPacketRate());

    kv = operational_reply->add_kv();
    kv->set_key("udp-total_message_bytes");
    kv->set_int_value(_messages.getBytes());

    kv = operational_reply->add_kv();
    kv->set_key("udp-total_message_byte_rate");
    kv->set_int_value(_messages.getByteRate());

    // If verbose mose is not set, we are done
    if (verbosity == telemetry::VerbosityLevel::TERSE) {
        return;
    }

    // Get stats from the message bus interface
    _getOperational_path(operational_reply, verbosity);
}
