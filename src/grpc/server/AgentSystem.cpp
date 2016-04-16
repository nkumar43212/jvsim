//
//  AgentSystem.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/21/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentSystem.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>

void
AgentSystem::systemAdd (SystemId id, const Telemetry::Path *request_path)
{
    // Operation
    std::string log_str = "SystemAdd: ";

    // The gist of the request
    // Convert request to string
    std::string request_path_str;
    // Serialize the data in text format
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(*request_path, &request_path_str);

    // Write it
    _logger->log(log_str + request_path_str);

    // One more
    _add_system_count++;
}

void
AgentSystem::systemRemove (SystemId id, const Telemetry::Path *request_path)
{
    // Operation
    std::string log_str = "SystemRemove: ";

    // The gist of the request
    // Convert request to string
    std::string request_path_str;
    // Serialize the data in text format
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(*request_path, &request_path_str);

    // Write it
    _logger->log(log_str + request_path_str);

    // One more
    _remove_system_count++;
}