//
//  AgentServerLog.hpp
//  Telemetry Agent
//
//  Created: 1/24/16.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentServerLog_hpp
#define AgentServerLog_hpp

#include <iostream>
#include <cstring>
#include <fstream>
#include <mutex>
#include <ctime>

#define AGENTSERVER_LOGFILE "agent_server.log"

class AgentServerLog {
    std::string   _logfile;
    uint64_t      _sequence_number;
    uint8_t       _level;
    std::ofstream _outputFile;
    std::mutex    _log_mutex;
    const char    *time_format = "%h %d %H:%M:%S  ";

public:
    AgentServerLog () : _outputFile(AGENTSERVER_LOGFILE)
    {
        _logfile = std::string(AGENTSERVER_LOGFILE);
        _sequence_number = 0;
        _level = 1;
    }

    AgentServerLog (const std::string logfile) : _logfile(logfile),
                                                 _outputFile(_logfile)
    {
        _sequence_number = 0;
        _level = 0;
    }

    // Write to the logger
    void log (std::string message, bool raw = false)
    {
        if (_level == 0) {
            return;
        }

        std::lock_guard<std::mutex> guard(_log_mutex);
        if (raw) {
            _outputFile << message << std::endl;
            _outputFile.flush();
            return;
        }

        std::time_t now = std::time(NULL);
        char timestr[20] = {0};
        std::strftime(timestr, sizeof(timestr),
                      time_format, std::localtime(&now));
        _outputFile << timestr << "#" << _sequence_number << "# "
                    << message << std::endl;
        _outputFile.flush();
        _sequence_number++;
    }

    // Write to logger conditionally
    void logfilter (std::string message, std::string filter)
    {
        std::size_t found = message.find(filter);

        if (found != std::string::npos) {
            log(message);
        }
    }

    void enable ()
    {
        _level = 1;
        log("Logging enabled --\n");
    }

    void disable ()
    {
        log("Logging disabled --\n");
        _level = 0;
    }
};

#endif /* AgentServerLog_hpp */
