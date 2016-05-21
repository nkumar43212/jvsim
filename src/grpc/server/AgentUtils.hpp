//
//  AgentSystem.hpp
//  Telemetry Agent
//
//  Created: 4/21/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentUtils_hpp
#define AgentUtils_hpp

#include <string>
#include <google/protobuf/message.h>

// All Utils functions will be defined in this class 
class AgentUtils {

public:
    static std::string getMessageString(const google::protobuf::Message& message,
                                        bool single_line = true);
    static void SearchNReplaceString (std::string& subject,
                                      const std::string& search,
                                      const std::string& replace);
};

#endif /* AgentUtils_hpp */