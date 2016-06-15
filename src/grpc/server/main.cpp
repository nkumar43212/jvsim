//
//  main.cpp
//  Telemetry Agent
//
//  Created: 12/29/15.
//
//  Authors: NITIN KUMAR
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

// Header files
#include <iostream>
#include <thread>
#include "AgentServer.h"
#include "AgentServerCmdOptions.hpp"
#include "AgentSystemFactory.hpp"
#include "AgentRestart.hpp"
#include "OpenConfig.hpp"
#include "AgentSubscription.hpp"
#include "lib_oc.h"
#include "GlobalConfig.hpp"
#include "UdpReceiver.hpp"

extern int pid_lock (const char *filename);

// Class/Function Implementation
void
RunServer (AgentServerLog *logger, AgentSystem *sys_handle,
           PathValidator *path_validator)
{
    // This is NA grpc server address as configured by the user
    std::string my_server_address;
    if (global_config.running_mode == RUNNING_MODE_OFF_BOX) {
        my_server_address = global_config.grpc_server_ip + ":" +
                            std::to_string(global_config.grpc_server_port);
    } else {
        my_server_address = "unix:" + global_config.grpc_server_unix_socket;
    }
    AgentServer service(logger, sys_handle, path_validator);
    ServerBuilder builder;

    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(my_server_address,
                             grpc::InsecureServerCredentials());

    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);

    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << my_server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

AgentSystem *
CreateSystemHandle (AgentServerCmdOptions *cmd_options,
                    AgentServerLog *logger)
{
    AgentSystem *sys_handle;
    
    // Now look at the system mode provided
    if (cmd_options->isSystemModeFile()) {
        sys_handle = AgentSystemFactory::createFile(logger,
                                        cmd_options->getSystemFileName());
    } else if (cmd_options->isSystemModeProc()) {
        sys_handle = AgentSystemFactory::createSystemProcess(logger);
    } else {
        // Default Mode
        sys_handle = AgentSystemFactory::createNull(logger);
    }

    return sys_handle;
}

int
main (int argc, char * argv[])
{
    if (pid_lock("/var/run/na-grpcd.pid") < 0) {
      printf ("na-grpcd already running!");
      return -1;
    }

    // Get all command line options
    AgentServerCmdOptions opts;
    if (!opts.parseArgs(argc, argv)) {
        exit(0);
    }

    // Log file and config file pointers
    char *log_file;
    char *ini_config_file;

    // If ini file is defined, invoke Global Config Parser
    ini_config_file = opts.getIniConfigFile();
    if (ini_config_file != NULL) {
        bool parse_status = GlobalConfig::parse(std::string(ini_config_file),
                                                global_config);
        if (!parse_status) {
            std::cerr << "INI config file parse failed" << std::endl;
            exit(0);
        }

        // Set logger information
        std::string log_file_str = global_config.grpc_log_file_path + "/" +
                                   global_config.grpc_log_file_name;
        opts.setLogFile(log_file_str);

        // Set system mode information
        if (global_config.system_mode == SYSTEM_MODE_PROC) {
            opts.setProcMode(true);
        } else if (global_config.system_mode == SYSTEM_MODE_FILE) {
            opts.setFileMode(true);
            std::string file_str = global_config.system_file_path + "/" +
                                   global_config.system_file_name;
            opts.setFileName(file_str);
        } else {
            opts.setNullMode(true);
        }
    }

    // Validate if logger file was specified
    log_file = opts.getLogFile();
    if (log_file == NULL) {
        char *env_rp = std::getenv("ROOTPATH");
        if (env_rp != NULL) {
            // if ROOTPATH env variable is set, set default log path
            std::string log_file_str = (std::string)env_rp +
                                       "/logs/" + AGENTSERVER_LOGFILE;
            log_file = (char *)log_file_str.c_str();
            std::cout << "Log file not specified using -l option. " <<
                         "Default = " << log_file << std::endl;
        } else {
            std::cerr << "Please setup ROOTPATH environment variable " <<
                         "or use -l to set log file." << std::endl;
            exit(0);
        }
    }

    // Create a logger
    AgentServerLog *logger;
    logger = new AgentServerLog(log_file);
    logger->enable();

    // Initialize interface with Mosquitto Library
    mosqpp::lib_init();

    // Initialize all the oc translators
    lib_oc_init();
    OpenConfig::display(logger);

    // Create a handle for the system
    AgentSystem *sys_handle = CreateSystemHandle(&opts, logger);

    // On start up/restart execute necessary functionality
    if (!on_startup(logger, sys_handle)) {
        logger->log("Failed in startup module. Terminating");
        exit(0);
    }

    // Create a PathValidator object
    PathValidator *path_validator = NULL;
    if (global_config.validate_ocpaths) {
        path_validator = new PathValidator(logger);
        bool status = path_validator->build_path_information_db(
                                    global_config.ocpath_file_path + "/" +
                                    global_config.ocpath_file_name);
        if (!status) {
            logger->log("PathValidator build_path failed. Terminating");
            exit(0);
        }
        path_validator->print_path_information_db();
    }

    // Start a UDP thread if requested
    if (global_config.udp_server_module) {
        udpreceiver = new UdpReceiver(logger,
                                      std::stoi(global_config.udp_server_ip),
                                      global_config.udp_server_port);
        std::thread udpreceiverthread([]() { (*udpreceiver)(); });
        udpreceiverthread.detach();
    }

    // Start the server
    RunServer(logger, sys_handle, path_validator);
}
