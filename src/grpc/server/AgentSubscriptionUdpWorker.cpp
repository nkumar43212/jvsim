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
                //std::unique_lock<std::mutex> lz(_mm);
                // std::cerr << _q.size() << "#" << _id << " " << _q.front() << std::endl;
                // std::cerr << "#" << _id << ":" << _q.front().size() << std::endl;
                _messages.increment(1, _q.front().size());
                _total_pkt_recvd++;

                // TODO ABBAS - ----- ????
                OpenConfigData *oc_data = new OpenConfigData;

                oc_data->ParseFromString(_q.front());

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
        // std::unique_lock<std::mutex> lz(_mm);
        // std::cerr << n << ": " << _q.size() << std::endl;
    }
}