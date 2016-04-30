//
//  AgentSystem.hpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/21/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentUtils_hpp
#define AgentUtils_hpp

#include <string>
#include <google/protobuf/message.h>

// All Utils functions will be defined in this class 
class AgentUtils {

public:
    static std::string getMessageString(const google::protobuf::Message& message);
    static void SearchNReplaceString (std::string& subject,
                                      const std::string& search,
                                      const std::string& replace);
};

#endif /* AgentUtils_hpp */