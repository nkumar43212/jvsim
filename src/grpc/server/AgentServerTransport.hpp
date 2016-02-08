//
//  AgentServerTransport.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/20/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentServerTransport_h
#define AgentServerTransport_h

#include "AgentServer.h"

class AgentServerTransport {
    ServerContext *_context;
    ServerWriter<OpenConfigData>* _writer;
    
public:
    AgentServerTransport (ServerContext *server_context,
                          ServerWriter<OpenConfigData>* writer_context) : _context(server_context), _writer(writer_context)
    {
    }
    
    void write (const OpenConfigData *data)
    {
        if (!_writer) {
            return;
        }
        
        _writer->Write(*data);
    }
};



#endif /* AgentServerTransport_h */
