//
//  AgentUtils.cpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/21/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentUtils.hpp"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>

std::string
AgentUtils::getMessageString (const google::protobuf::Message& message)
{
    // Convert request to string
    std::string message_str;
    // Serialize the data in text format
    google::protobuf::TextFormat::Printer printer;
    // Use single line mode
    printer.SetSingleLineMode(true);
    printer.PrintToString(message, &message_str);

    return message_str;
}