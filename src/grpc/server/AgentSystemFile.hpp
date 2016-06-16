//
//  AgentSystemFile.hpp
//  Telemetry Agent
//
//  Created: 2/27/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSystemFile_hpp
#define AgentSystemFile_hpp

#include "AgentSystem.hpp"

class AgentSystemFile: public AgentSystem {
    // Destination file
    std::ofstream _outputFile;
    
public:
    AgentSystemFile (AgentServerLog *logger, std::string file_name) :
                                AgentSystem(logger), _outputFile(file_name) {}
    ~AgentSystemFile() {}

    bool systemAdd(SystemId id, const Telemetry::Path *request_path);
    bool systemRemove(SystemId id, const Telemetry::Path *request_path);
    Telemetry::Path * systemGet(SystemId sys_id);
    bool systemClearAll(void);
};

#endif /* AgentSystemFile_hpp */
