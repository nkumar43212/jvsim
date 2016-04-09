//
//  AgentClientLag.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/17/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentClientLag.hpp"

// Lags known to the collector
std::map<std::string, AgentLag *> lag_db;

// Build a LAG database
void
AgentLag::build (uint32_t lag_count, uint32_t lc_count)
{
    for (int i = 0; i < lag_count; i++) {
        std::string name("ae");
        AgentLag *lag = new AgentLag(name + std::to_string(i));
        for (int j = 0; j < lc_count; j++) {
            AgentMemberInterface *member;
            std::string cname = "oc-path/interfaces/xe-" + std::to_string(j) + "/0/" + std::to_string(i) + "/";
            member = new AgentMemberInterface(cname);
            lag->addMember(member);
        }
        AgentLag::addLag(lag);
    }
}

void
AgentLag::reset ()
{
    lag_db.clear();
    AgentLag::print();
}

void
AgentLag::addLag (AgentLag *lag)
{
    lag_db[lag->getName()] = lag;
}

AgentLag *
AgentLag::findLag(const std::string lag_name)
{
    if (lag_db.count(lag_name) == 0) {
        return NULL;
    }
    
    return lag_db[lag_name];
}

void
AgentLag::print ()
{
    std::map<std::string, AgentLag *>::iterator itr;
    
    for (itr = lag_db.begin(); itr != lag_db.end(); itr++) {
        std::cout << itr->first << "\n";
        itr->second->printMembers();
    }
}

void
AgentLag::updateStats(Telemetry::OpenConfigData *data)
{
    uint32_t    size = data->kv_size();
    std::string member_name;
    std::string lag_name;
    uint64_t    packets = 0;
    
    for (int i = 0; i < size; i++) {
        const Telemetry::KeyValue &kv = data->kv(i);
        
        bool found_all   = false;
        
        // Child interface name
        if (kv.key() == "__prefix__") {
            member_name = kv.str_value();
        }
        
        if (kv.key() == std::string("parent_ae_name")) {
            lag_name = kv.str_value();
        }
        
        if (kv.key() == "output_packets") {
            packets = kv.int_value();
            found_all = true;
        }
        
        if (!found_all) {
            continue;
        }
        
        found_all = false;
        AgentLag *lag = AgentLag::findLag(lag_name);
        if (!lag) {
            continue;
        }
        
        // Find the member
        AgentMemberInterface *memberp = lag->findMember(member_name);
        if (!memberp) {
            continue;
        }
        
        // Update the stats
        memberp->incPackets(packets);
    }
}
