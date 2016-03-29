//
//  AgentSystemProc.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/29/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemProc_hpp
#define AgentSystemProc_hpp

#include <stdio.h>
#include "AgentSystem.hpp"
#include <grpc++/grpc++.h>
#include "junos_mgd.pb.h"
#include "junos_mgd.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using junos_mgd::OpenConfigRpcApi;
using junos_mgd::EditConfigRequest;
using junos_mgd::RequestHdr;
using junos_mgd::ReplyHdr;
using junos_mgd::ConfigCommand;
using junos_mgd::OpenConfigDataElement;
using junos_mgd::EditConfigCommands;
using junos_mgd::EditConfigResponse;
using junos_mgd::GetConfigRequest;
using junos_mgd::GetConfigResponse;


class AgentSystemProc : public AgentSystem {
    
    
public:
    // The stub for the RPC
    std::unique_ptr<OpenConfigRpcApi::Stub> stub_;
    
    AgentSystemProc (AgentServerLog *logger) : AgentSystem(logger)
    {
    }
    
    void systemAdd(SystemId id, const agent::Path *request_path);
    void systemRemove(SystemId id, const agent::Path *request_path);
    agent::Path * systemGet(SystemId sys_id);
    
    // Helper routines
    // Generate a name from the request
    void generateName(const agent::Path *request_path, std::string &name)
    {
        name += "path=";
        name += request_path->path();
        name += "-";
        
        name += "interval=";
        name += std::to_string(request_path->sample_frequency());
        name += "-";
        
        if (!request_path->filter().empty()) {
            name += "filter=";
            name += request_path->filter();
            name += "-";
        }
    }
    
    void parseNameField(std::string path_name, const std::string& field_name, char delim, std::string &value)
    {
        // Just in case
        value = "";
        
        // Look for the field name
        char *str = strstr((char *) path_name.c_str(), (char *) field_name.c_str());
        if (!str) {
            return;
        }
        
        // Copy over the path
        int i;
        std::string val;
        str += (strlen(field_name.c_str()) + 1);
        for (i = 0; str && (*str != delim) && (*str != '\0'); str++, i++) {
            value += *str;
        }
    }
    
    // From a name form the request path
    void parseName(const std::string &name, agent::Path *request_path)
    {
        std::string value;
        
        //path=xxx-interval=ddd-filter=dddd-
        
        // Look for path
        parseNameField(name, "path", '-', value);
        request_path->set_path(value);
        
        // Look for interval
        parseNameField(name, "interval", '-', value);
        if (value != "") {
            request_path->set_sample_frequency(atoi(value.c_str()));
        }
        
        // Look for resource filter
        parseNameField(name, "filter", '-', value);
        request_path->set_filter(value);
    }
};

#endif /* AgentSystemProc_hpp */
