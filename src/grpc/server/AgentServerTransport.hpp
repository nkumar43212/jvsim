//
//  AgentServerTransport.hpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 1/20/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentServerTransport_hpp
#define AgentServerTransport_hpp

#include "AgentServerProtos.h"

class AgentServerTransport {
    ServerContext *_context;
    ServerWriter<OpenConfigData>* _writer;

public:
    AgentServerTransport (ServerContext *server_context,
                          ServerWriter<OpenConfigData>* writer_context) :
                          _context(server_context),
                          _writer(writer_context)  {}

    ServerContext * getServerContext (void) {
        return _context;
    }

    void write (const OpenConfigData *data)
    {
        if (!_writer) {
            return;
        }

        // TODO ABBAS === Add a mutex protection here for UDP/MQTT
        _writer->Write(*data);
    }
};

#endif /* AgentServerTransport_hpp */