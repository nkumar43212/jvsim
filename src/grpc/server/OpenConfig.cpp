//
//  OpenConfig.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "OpenConfig.hpp"

void
OpenConfig::load()
{
}

std::map<std::string, OpenConfig *> openconfig_registry;

void
OpenConfig::registerSelf (const std::string path_name, OpenConfig *handler)
{
    openconfig_registry[path_name] = handler;
}

OpenConfig *
OpenConfig::find (const std::string name)
{
    if (openconfig_registry.count(name) == 0) {
        return NULL;
    }
    
    return openconfig_registry[name];
}

void
OpenConfig::display (AgentServerLog *logger)
{
    std::map<std::string, OpenConfig *>::iterator itr;
    
    logger->log("Registered Translators:");
    for (itr = openconfig_registry.begin(); itr != openconfig_registry.end();
         itr++) {
        std::string msg = "OC Translator = " + itr->first;
        logger->log(msg);
    }
}

void 
OpenConfig::insertExportTimestamp (Telemetry::OpenConfigData *datap)
{
    struct timeval tv;
    Telemetry::KeyValue *kv;

    // What is the time now
    gettimeofday(&tv, NULL); 

    // Add the key
    kv = datap->add_kv();
    kv->set_key("__timestamp__");
    kv->set_uint_value(tv.tv_sec * 1000000 + tv.tv_usec);
}
