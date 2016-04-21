//
//  AgentSystem.hpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/21/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentUtils_hpp
#define AgentUtils_hpp

#include <google/protobuf/message.h>

// All Utils functions will be defined in this class 
class AgentUtils {

public:
    static std::string getMessageString(const google::protobuf::Message& message);
};

#endif /* AgentUtils_hpp */
