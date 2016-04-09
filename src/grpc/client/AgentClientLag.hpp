//
//  AgentClientLag.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/17/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentClientLag_hpp
#define AgentClientLag_hpp

#include <string>
#include <map>
#include <iostream>
#include "AgentClient.hpp"

// A Member Iterface of a LAG
class AgentMemberInterface {
    std::string _name;
    uint64_t    _packets;
    
public:
    uint64_t    getPackets ()          { return _packets; }
    void        incPackets(uint64_t n) { _packets += n; }
    std::string getName()              { return _name;    }
    
    AgentMemberInterface(const std::string name) : _name(name), _packets(0) {}
};

// A Lag interface
class AgentLag {
    std::string _name;
    std::map<std::string, AgentMemberInterface *> _interfaces;
    
public:
    std::string getName () { return _name; }
    AgentLag (const std::string name) : _name(name) {}
    
    static void           build(uint32_t lag_count, uint32_t lc_count);
    static void           reset();
    static void           addLag(AgentLag *lag);
    static AgentLag * findLag(const std::string lag_name);
    static void           print();
    static void           updateStats(Telemetry::OpenConfigData *data);
    
    void addMember (AgentMemberInterface *child)
    {
        _interfaces[child->getName()] = child;
    }
    
    void deleteMember (AgentMemberInterface *member)
    {
        if (_interfaces.count(member->getName())) {
            _interfaces[member->getName()] = NULL;
        }
    }
  
    AgentMemberInterface * findMember(const std::string name)
    {
        return _interfaces.count(name) != 0 ? _interfaces[name] : NULL;
    }
    
    void printMembers ()
    {
        std::map<std::string, AgentMemberInterface *>::iterator itr;
        
        std::cout << "-----------\n";
        uint64_t count = 0;
        for (itr = _interfaces.begin(); itr != _interfaces.end(); itr++) {
            AgentMemberInterface *childp = itr->second;
            count += childp->getPackets();
            std::cout << "  " << childp->getName() << ":" << childp->getPackets() << "\n";
        }
        std::cout << "Total = " << count << "\n";
        std::cout << "-----------\n";
    }
    
};



#endif /* AgentClientLag_hpp */
