//
//  AgentClient.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#ifndef AgentClient_hpp
#define AgentClient_hpp

#include <stdio.h>
#include <iostream>
#include <string>

#include <grpc++/grpc++.h>
#include "agent.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using agent::Agent;
using agent::OpenConfigData;
using agent::SubscriptionRequest;
using agent::UnSubscribeRequest;
using agent::GetRequest;
using agent::Path;
using agent::Reply;

// Location of logs
#define AGENTCLIENT_LOG_DIR "/Users/nitin/test/logs/"

// Name of Management Client
#define AGENTCLIENT_MGMT "Management-Client"

class AgentClient {
    std::string _name;
    std::string _logfile;
    bool _active;
    bool _debug_log;
    
    // ID used by the client
    uint32_t _id;
    
    // ID allocated by the server
    bool     _subscription_id_valid;
    uint32_t _subscription_id;
    
   
public:
    std::unique_ptr<Agent::Stub> stub_;
    AgentClient(std::shared_ptr<Channel> channel,
                const std::string& name,
                uint32_t id) : stub_(Agent::NewStub(channel)), _name(name), _active(true), _id(id)
    {
        std::string s(AGENTCLIENT_LOG_DIR);
        _logfile = s + _name;
        _subscription_id_valid = false;
        _subscription_id       = 0;
        _debug_log = false;
    }   
    
    ~AgentClient();
    
    static AgentClient *create(std::shared_ptr<Channel> channel, std::string& name, uint32_t id);
    static AgentClient *find(std::string name);
    static void         print();
    
    std::string getName()          { return _name;                  }
    bool        getActive()        { return _active;                }
    uint32_t    getId()            { return _id;                    }
    uint32_t    getServerId()      { return _subscription_id;       }
    bool        getServerIdValid() { return _subscription_id_valid; }
    bool        getDebug ()        { return _debug_log;             }
    void        setDebug(bool val) { _debug_log = val;              }
    
    void subscribeTelemetry(std::vector<std::string> path_list, uint32_t limit_records = 0, uint32_t limit_seconds = 0);
    void unSubscribeTelemetry();
    void listSubscriptions(uint32_t verbosity);
    void getOperational(uint32_t verbosity);
    void errorMsg(std::string err_str, Status code)
    {
        std::cout << err_str << "\n";
    }
};



#endif /* AgentClient_hpp */
