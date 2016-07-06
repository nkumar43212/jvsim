//
//  AgentTelegrafLog
//  agent-jv-client
//
//  Created by NITIN KUMAR on 5/31/15.
//
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <string>
#include <map>
#include <iostream>

class AgentTelegrafLog {
    AgentServerLog *_logger;

    public:
        AgentTelegrafLog(const std::string logfile)
        {
            _logger = new AgentServerLog(logfile + ".teldata");
            _logger->enable();
        }

        ~AgentTelegrafLog() 
        {
            delete _logger;
        }

        void updateStats(telemetry::OpenConfigData *data)
        {
            // Should we even process this element sample ?
            if (data->kv_size() < 2) {
                return;
            }

            // Get the timestamp
            uint64_t ts = data->timestamp();
            const telemetry::KeyValue &kv = data->kv(0);
            if (kv.key() == "__timestamp__") {
                ts = kv.uint_value();
            }

            // Get the interface name
            const telemetry::KeyValue &kv1 = data->kv(1);
            if (kv1.key() != "__prefix__") {
                return;
            }

            // Find the interface name
            std::string port_name = kv1.str_value();
            std::string ifname;
            size_t pos = port_name.find("=", 0);
            if (pos != std::string::npos) {
                // Found matching "="
                ifname = port_name.substr(pos+1);
                size_t pos = ifname.find("]", 0);
                if (pos != std::string::npos) {
                    ifname.erase(pos);
                }
            } else {
                ifname = "XXXX";
            }

            // Build a line
            std::string resource("interface");
            std::string sysname = "sysname=" + data->system_id();
            std::string name("ifname=");
            buildLine(resource, sysname, name + ifname, ts);
        }

        void buildLine(const std::string resource, const std::string sysname,
                       const std::string ifname, uint64_t ts)
        {
            std::string logStr = resource  + "," + sysname + "," +
                                 ifname + " " + std::to_string(ts);
            _logger->log(logStr, true);
        }
};
