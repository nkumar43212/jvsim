//
//  GlobalConfig.cpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "GlobalConfig.hpp"
#include "INIReader.h"


// Global variable
GlobalConfig global_config;


bool
GlobalConfig::is_valid_subscription_topic_name (std::string sub_topic)
{
    if (sub_topic == TOPIC_INTERNAL_SUB_ID) {
        return true;
    }
    if (sub_topic == TOPIC_PATH) {
        return true;
    }
    return false;
}

bool
GlobalConfig::is_valid_system_mode (std::string system_mode)
{
    if (system_mode == SYSTEM_MODE_NULL) {
        return true;
    }
    if (system_mode == SYSTEM_MODE_FILE) {
        return true;
    }
    if (system_mode == SYSTEM_MODE_PROC) {
        return true;
    }
    return false;
}

std::ostream&
operator<<(std::ostream& os, GlobalConfig& gc)
{
    os << "GLOBAL CONFIGURATION:" << std::endl;
    os << "--------------------"  << std::endl;

    os << "Telemetry GRPC server ip     : "
                                << gc.grpc_server_ip << std::endl;
    os << "Telemetry GRPC server port   : "
                                << gc.grpc_server_port << std::endl;
    os << "Telemetry GRPC log file path : "
                                << gc.grpc_log_file_path << std::endl;
    os << "Telemetry GRPC log file name : "
                                << gc.grpc_log_file_name << std::endl;

    os << "Device MGD ip                : "
                                << gc.device_mgd_ip << std::endl;
    os << "Device MGD port              : "
                                << gc.device_mgd_port << std::endl;

    os << "Invoke UDP server            : "
                                << gc.udp_server_module << std::endl;
    os << "UDP server ip                : "
                                << gc.udp_server_ip << std::endl;
    os << "UDP server port              : "
                                << gc.udp_server_port << std::endl;
    
    os << "Subscribe topic name         : "
                                << gc.subscribe_topic_name << std::endl;
    os << "System Mode                  : "
                                << gc.system_mode << std::endl;
    if (gc.system_mode == SYSTEM_MODE_FILE) {
        os << "System file path             : "
                                << gc.system_file_path << std::endl;
        os << "System file name             : "
                                << gc.system_file_name << std::endl;
    }

    os << std::endl;
    return os;
}

void
GlobalConfig::parse(std::string filename, GlobalConfig &global_config)
{
    INIReader reader(filename);
    
    if (reader.ParseError() < 0) {
        std::cout << "Can't load INI file = " << filename << std::endl;
        exit(1);
    }

    // INI_SECTION_TELEMETRY_GRPC
    global_config.grpc_server_ip = reader.Get(INI_SECTION_TELEMETRY_GRPC,
                                              "grpc_server_ip",
                                              global_config.grpc_server_ip);
    global_config.grpc_server_port = (int)reader.GetInteger(
                                              INI_SECTION_TELEMETRY_GRPC,
                                              "grpc_server_port",
                                              global_config.grpc_server_port);
    global_config.grpc_log_file_path = reader.Get(INI_SECTION_TELEMETRY_GRPC,
                                              "grpc_log_file_path",
                                              global_config.grpc_log_file_path);
    global_config.grpc_log_file_name = reader.Get(INI_SECTION_TELEMETRY_GRPC,
                                              "grpc_log_file_name",
                                              global_config.grpc_log_file_name);

    // INI_SECTION_JUNOS_DEVICE
    global_config.device_mgd_ip = reader.Get(INI_SECTION_JUNOS_DEVICE,
                                             "device_mgd_ip",
                                             global_config.device_mgd_ip);
    global_config.device_mgd_port = (int)reader.GetInteger(
                                             INI_SECTION_JUNOS_DEVICE,
                                             "device_mgd_port",
                                             global_config.device_mgd_port);

    // INI_SECTION_UDP
    global_config.udp_server_module = reader.GetBoolean(INI_SECTION_UDP,
                                             "udp_server_module",
                                             global_config.udp_server_module);
    global_config.udp_server_ip = reader.Get(INI_SECTION_UDP,
                                             "udp_server_ip",
                                             global_config.udp_server_ip);
    global_config.udp_server_port = (int)reader.GetInteger(
                                             INI_SECTION_UDP,
                                             "udp_server_port",
                                             global_config.udp_server_port);

    // INI_SECTION_OTHER_KNOBS
    std::string subscribe_topic_name = reader.Get(
                                             INI_SECTION_OTHER_KNOBS,
                                             "subscribe_topic_name",
                                             global_config.subscribe_topic_name);
    if (GlobalConfig::is_valid_subscription_topic_name(subscribe_topic_name)) {
        global_config.subscribe_topic_name = subscribe_topic_name;
    }

    std::string system_mode = reader.Get(INI_SECTION_OTHER_KNOBS,
                                         "system_mode",
                                         global_config.system_mode);
    if (GlobalConfig::is_valid_system_mode(system_mode)) {
        global_config.system_mode = system_mode;
    }
    if (global_config.system_mode == SYSTEM_MODE_FILE) {
        global_config.system_file_path = reader.Get(INI_SECTION_OTHER_KNOBS,
                                               "system_file_path",
                                               global_config.system_file_path);
        global_config.system_file_name = reader.Get(INI_SECTION_OTHER_KNOBS,
                                               "system_file_name",
                                               global_config.system_file_name);
    }
    
    std::cout << global_config;
}