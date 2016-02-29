//
//  AgentServerCmdOptions.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/26/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentServerCmdOptions_hpp
#define AgentServerCmdOptions_hpp

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

class AgentServerCmdOptions {
    char *_logfile;
    
    bool _file_mode;
    char *_system_file_name;
    
    bool _null_mode;
    bool _proc_mode;
    
public:
    AgentServerCmdOptions ()
    {
        _logfile = NULL;
        _system_file_name = NULL;
        _file_mode = _null_mode = _proc_mode = false;
    }
    
    void printUsage ()
    {
        std::cout << "usage: agent_server [-u] [-n] [-p] [-f] <filename> [-l] <logfile>" << "\n";
        std::cout << " -u  : usage\n";
        std::cout << " -n  : null mode\n";
        std::cout << " -f  : file mode\n";
        std::cout << " -p  : proc mode\n";
        std::cout << " -l  : log file\n";
    }
    
    bool parseArgs(int argc, char **argv)
    {
        int c;
        while((c =  getopt(argc, argv, "unpf:l:")) != -1)
        {
            switch (c)
            {
                case 'u':
                    printUsage();
                    break;
                    
                case 'n':
                    _null_mode = true;
                    break;
                    
                case 'p':
                    _proc_mode = true;
                    break;
                
                case 'f': {
                    std::string *str = new std::string(optarg);
                    _system_file_name = (char *) str->c_str();
                    _file_mode = true;
                    break;
                }
                    
                case 'l': {
                    std::string *str = new std::string(optarg);
                    _logfile = (char *) str->c_str();
                    break;
                }
                    
                case ':': {
                    std::cout << "Unknown Option\n";
                    break;
                }
            }
        }
        
        optind = 1;
        return true;
    }
    
    char *getLogFile ()
    {
        return _logfile;
    }
    
    char *getSystemFileName ()
    {
        return _system_file_name;
    }
    
    bool isSystemModeNull () { return _null_mode; }
    bool isSystemModeProc () { return _proc_mode; }
    bool isSystemModeFile () { return _file_mode; }
};
#endif /* AgentServerCmdOptions_hpp */
