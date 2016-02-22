//
//  AgentSystem.cpp
//  grpc
//
//  Created by NITIN KUMAR on 2/21/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystem.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>

void
AgentSystem::systemAdd (const agent::Path *request_path)
{
    // Operation
    std::string log_str;
    log_str = "SystemAdd:\n";
    
    // The gist of the request
    std::string request_str;
    google::protobuf::TextFormat::PrintToString(*request_path, &request_str);
    
    // Write it
    _logger->log(log_str + request_str);
    
    // One more
    _add_count++;
}

void
AgentSystem::systemRemove (const agent::Path *request_path)
{
    // Operation
    std::string log_str;
    log_str = "SystemRemove:\n";
    
    // The gist of the request
    std::string request_str;
    google::protobuf::TextFormat::PrintToString(*request_path, &request_str);
    
    // Write it
    _logger->log(log_str + request_str);
    
    // One more
    _remove_count++;
}
