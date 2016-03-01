//
//  AgentSystemFile.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemFile_hpp
#define AgentSystemFile_hpp

#include <stdio.h>
#include "AgentSystem.hpp"

class AgentSystemFile: public AgentSystem {
    // Destination file
    std::ofstream _outputFile;
   
    // Mapping to the system paths
    std::map<std::string, std::string> _system_map;
    
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
    
    // Get the system resource
    const std::string getSystemResource(const std::string path_name)
    {
        std::map<std::string, std::string>::iterator itr = _system_map.find(path_name);
        
        if (itr == _system_map.end()) {
            return path_name;
        } else {
            return itr->second;
        }
    }
    
    void append (std::string &base, std::string trailer, bool newline = false)
    {
        if ((trailer == std::string("")) || trailer.empty()) {
            return;
        }
        
        base += trailer;
        base += " ";
        
        if (newline) {
            base += " \n";
        }
    }
   
    void append_int (std::string &base, uint32_t int_val, bool newline = false)
    {
        base += std::to_string(int_val);
        
        if (newline) {
            base += "\n";
        }
    }
    
    // Get the message towards the system file
    void generateAddMessage (const agent::Path *request_path,
                             std::string sensor_name,
                             std::string &message)
    {

        std::string system_resource = getSystemResource(request_path->path());
        append(message, "set services analytics sensor");
        append(message, sensor_name);
        append(message, "resource");
        append(message, system_resource, true);
        append(message, "set services analytics sensor");
        append(message, sensor_name);
        append(message, "reporting-interval");
        append_int(message, request_path->sample_frequency(), true);
        append(message, "set services analytics sensor");
        append(message, sensor_name);
        append(message, "export-to-re", true);
        if (!request_path->filter().empty()) {
            append(message, "set services analytics sensor");
            append(message, sensor_name);
            append(message, "resource-filter");
            append(message, request_path->filter(), true);
        }
    }
    
    // Get the message towards the system file
    void generateRemoveMessage (const agent::Path *request_path,
                                std::string sensor_name,
                                std::string &message)
    {
        append(message, "delete services analytics sensor");
        append(message, sensor_name, true);
    }
    

public:
    AgentSystemFile (AgentServerLog *logger, std::string file_name) : AgentSystem(logger), _outputFile(file_name)
    {
        // Load the system map
        _system_map["port"]       = std::string("/junos/system/linecard/interface/");
        _system_map["firewall"]   = std::string("/junos/system/linecard/firewall/");
        _system_map["interfaces"] = std::string("/junos/system/linecard/interface/logical/usage/");
    }
    
    // Add into the system
    void systemAdd(const agent::Path *request_path);
    
    // Remove from the system
    void systemRemove(const agent::Path *request_path);
};


#endif /* AgentSystemFile_hpp */
