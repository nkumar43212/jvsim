//
//  AgentServerCmdOptions.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/26/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "AgentServerCmdOptions.hpp"

void
AgentServerCmdOptions::setLogFile (std::string logfile)
{
    std::string *str = new std::string(logfile);
    _logfile = (char *) str->c_str();
}

void
AgentServerCmdOptions::setFileMode (bool mode)
{
    _file_mode = mode;
}

void
AgentServerCmdOptions::setFileName (std::string file)
{
    std::string *str = new std::string(file);
    _system_file_name = (char *) str->c_str();
}

void
AgentServerCmdOptions::setNullMode (bool mode)
{
    _null_mode = mode;
}

void
AgentServerCmdOptions::setProcMode (bool mode)
{
    _proc_mode = mode;
}

void
AgentServerCmdOptions::printUsage (void)
{
    std::cout << "Usage: agent_server [-u/-h] [-n] [-p] [-f] <filename> "
                 "[-l] <logfile> "
                 "[-c] <ini config file>" << std::endl;
    std::cout << " -u/-h  : usage" << std::endl;
    std::cout << "    -n  : null mode" << std::endl;
    std::cout << "    -f  : file mode" << std::endl;
    std::cout << "    -p  : proc mode" << std::endl;
    std::cout << "    -l  : log file" << std::endl;
    std::cout << "    -c  : ini config file" << std::endl;
}

bool
AgentServerCmdOptions::parseArgs (int argc, char **argv)
{
    int c;
    while ((c =  getopt(argc, argv, "uhnpf:l:c:")) != -1) {
        switch (c) {
            case 'u':
            case 'h':
                printUsage();
                exit(0);
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

            case 'c': {
                std::string *str = new std::string(optarg);
                _ini_config_file = (char *) str->c_str();
                break;
            }

            case '?':
                if ((optopt == 'c') || optopt == 'l' || optopt == 'f') {
                    std::cout << "Option -" << char(optopt) <<
                                 " requires an argument." << std::endl;
                } else if (isprint (optopt)) {
                    std::cout << "Unknown Option" << std::endl;
                } else {
                    std::cout << "Unknown option character `\\x%x'.\n" << optopt;
                }
                return false;

            case ':': {
                std::cout << "Unknown Option" << std::endl;
                break;
            }
        }
    }

    optind = 1;
    // Return the status correctly
    return true;
}