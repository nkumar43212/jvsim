//
//  GlobalConfig.cpp
//  Telemetry Agent
//
//  Created: 4/27/16.
//
//  Authors: ABBAS SAKARWALA
//
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

bool
GlobalConfig::is_valid_running_mode (std::string running_mode)
{
    if (running_mode == RUNNING_MODE_ON_BOX) {
        return true;
    }
    if (running_mode == RUNNING_MODE_OFF_BOX) {
        return true;
    }
    return false;
}

std::ostream&
operator<<(std::ostream& os, GlobalConfig& gc)
{
    os << "GLOBAL CONFIGURATION:" << std::endl;
    os << "--------------------"  << std::endl;

    os << "Telemetry GRPC server unix s : "
                                << gc.grpc_server_unix_socket << std::endl;
    os << "Telemetry GRPC server ip     : "
                                << gc.grpc_server_ip << std::endl;
    os << "Telemetry GRPC server port   : "
                                << gc.grpc_server_port << std::endl;
    os << "Telemetry GRPC log file path : "
                                << gc.grpc_log_file_path << std::endl;
    os << "Telemetry GRPC log file name : "
                                << gc.grpc_log_file_name << std::endl;

    os << "Device MGD Unix socket       : "
                                << gc.device_mgd_unix_socket << std::endl;
    os << "Device MGD ip                : "
                                << gc.device_mgd_ip << std::endl;
    os << "Device MGD port              : "
                                << gc.device_mgd_port << std::endl;
    os << "Device Username              : "
                                << gc.device_user_name << std::endl;
    os << "Device Password              : "
                                << gc.device_password << std::endl;

    os << "MQTT broker ip               : "
                                << gc.mqtt_broker_ip << std::endl;
    os << "MQTT broker port             : "
                                << gc.mqtt_broker_port << std::endl;

    os << "Invoke UDP server            : "
                                << gc.udp_server_module << std::endl;
    os << "UDP server ip                : "
                                << gc.udp_server_ip << std::endl;
    os << "UDP server port              : "
                                << gc.udp_server_port << std::endl;

    os << "JSD init registration        : "
                                << gc.jsd_init_registration << std::endl;
    if (gc.jsd_init_registration) {
        os << "JSD number of retries        : "
                                << gc.jsd_num_of_retries << std::endl;
        os << "JSD json file path           : "
                                << gc.jsd_json_file_path << std::endl;
        os << "JSD json file name           : "
                                << gc.jsd_json_file_name << std::endl;
    }

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
    os << "Running Mode                 : "
                                << gc.running_mode << std::endl;
    os << "Validate OCPaths             : "
                                << gc.validate_ocpaths << std::endl;
    if (gc.validate_ocpaths) {
        os << "OCPath json file path        : "
                                << gc.ocpath_file_path << std::endl;
        os << "OCPath json file name        : "
                                << gc.ocpath_file_name << std::endl;
    }

    os << std::endl;
    return os;
}

bool
GlobalConfig::parse(std::string filename, GlobalConfig &global_config)
{
    INIReader reader(filename);
    
    if (reader.ParseError() < 0) {
        std::cout << "Can't load INI file = " << filename << std::endl;
        // parse error
        return false;
    }

    // INI_SECTION_TELEMETRY_GRPC
    global_config.grpc_server_unix_socket = reader.Get(
                                        INI_SECTION_TELEMETRY_GRPC,
                                        "grpc_server_unix_socket",
                                        global_config.grpc_server_unix_socket);
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
    global_config.device_mgd_unix_socket = reader.Get(INI_SECTION_JUNOS_DEVICE,
                                          "device_mgd_unix_socket",
                                          global_config.device_mgd_unix_socket);
    global_config.device_mgd_ip = reader.Get(INI_SECTION_JUNOS_DEVICE,
                                             "device_mgd_ip",
                                             global_config.device_mgd_ip);
    global_config.device_mgd_port = (int)reader.GetInteger(
                                             INI_SECTION_JUNOS_DEVICE,
                                             "device_mgd_port",
                                             global_config.device_mgd_port);
    global_config.device_user_name = reader.Get(INI_SECTION_JUNOS_DEVICE,
                                             "device_user_name",
                                             global_config.device_user_name);
    global_config.device_password = reader.Get(INI_SECTION_JUNOS_DEVICE,
                                             "device_password",
                                             global_config.device_password);

    // INI_SECTION_MQTT
    global_config.mqtt_broker_ip = reader.Get(INI_SECTION_MQTT,
                                              "mqtt_broker_ip",
                                              global_config.mqtt_broker_ip);
    global_config.mqtt_broker_port = (int)reader.GetInteger(
                                              INI_SECTION_MQTT,
                                              "mqtt_broker_port",
                                              global_config.mqtt_broker_port);

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

    // INI_SECTION_JSD
    global_config.jsd_init_registration = reader.GetBoolean(INI_SECTION_JSD,
                                            "init_registration",
                                            global_config.jsd_init_registration);
    global_config.jsd_num_of_retries = (int)reader.GetInteger(INI_SECTION_JSD,
                                            "number_of_retries",
                                            global_config.jsd_num_of_retries);
    global_config.jsd_json_file_path = reader.Get(INI_SECTION_JSD,
                                            "json_file_path",
                                            global_config.jsd_json_file_path);
    global_config.jsd_json_file_name = reader.Get(INI_SECTION_JSD,
                                            "json_file_name",
                                            global_config.jsd_json_file_name);

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

    std::string running_mode = reader.Get(INI_SECTION_OTHER_KNOBS,
                                          "running_mode",
                                          global_config.running_mode);
    if (GlobalConfig::is_valid_running_mode(running_mode)) {
        global_config.running_mode = running_mode;
    }
    global_config.validate_ocpaths = reader.GetBoolean(INI_SECTION_OTHER_KNOBS,
                                            "validate_ocpaths",
                                            global_config.validate_ocpaths);
    global_config.ocpath_file_path = reader.Get(INI_SECTION_OTHER_KNOBS,
                                            "ocpath_file_path",
                                            global_config.ocpath_file_path);
    global_config.ocpath_file_name = reader.Get(INI_SECTION_OTHER_KNOBS,
                                            "ocpath_file_name",
                                            global_config.ocpath_file_name);

    std::cout << global_config;

    // parse successful
    return true;
}
