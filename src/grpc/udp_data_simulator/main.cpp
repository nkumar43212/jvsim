//
//  main.cpp
//  udp_data_simulator
//
//  Created by Abbas Sakarwala on 5/17/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//


#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "agent.pb.h"
#include "Sensor_keys.h"
#include "AgentServerLog.hpp"

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     10000

// Message format expected
#define BUF_SIZE        9000
typedef struct _message {
    uint32_t        i_sub_id;
    char            buffer[BUF_SIZE];
} message;

message msg;

void
generate_oc_data (const std::string *keys, int keys_count,
                  telemetry::OpenConfigData &oc_data)
{
    // Fill in the common header
    oc_data.set_system_id("1.1.1.1");
    oc_data.set_component_id(0);
    oc_data.set_sub_component_id(1);
    // TODO ABBAS --- Change this to extracted path from sensor_name
    oc_data.set_path("/junos/system/linecard/cpu/memory/");
    oc_data.set_sequence_number(100);
    oc_data.set_timestamp(10000000);
    
    telemetry::KeyValue *kv;
    for (int i = 0; i < keys_count; i++) {
        kv = oc_data.add_kv();
        kv->set_key(keys[i]);
        kv->set_int_value(rand());
    }
}

int main()
{
    AgentServerLog *_log = new AgentServerLog("/tmp/udp_data_simulator.log");
    _log->enable();
    
    // Create socket fd
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Fill in server info
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);
    serv.sin_addr.s_addr = inet_addr(SERVER_IP);
    socklen_t m = sizeof(serv);

    int send = 0;
    // Fill in data and send over socket
    while (1) {
        int total_so_far = 0;
        
        // Fill the subscription id
        int sub_id = 1000 + (send % 4);
        msg.i_sub_id = htonl(sub_id);
        total_so_far += sizeof(int);

        // Fill the message buffer with OC Data
        char *pbuffer = &msg.buffer[0];

        // For all messages
        {
            telemetry::OpenConfigData oc_data;
            generate_oc_data(cpu_memory_keys,
                             sizeof(cpu_memory_keys)/sizeof(std::string),
                             oc_data);
            std::string oc_data_string;
            oc_data.SerializeToString(&oc_data_string);
            
            // Add to buffer
            if (total_so_far + oc_data_string.size() < BUF_SIZE) {
                strncpy(pbuffer, oc_data_string.c_str(), oc_data_string.size());
                total_so_far += oc_data_string.size();
            }
        }
        _log->log("Send one pkt sub_id " + std::to_string(sub_id) + " of size "
                  + std::to_string(total_so_far));
        sendto(sockfd, &msg, total_so_far, 0, (struct sockaddr *)&serv, m);
        // std::cout << "total_so_far: " << total_so_far << std::endl;
        send++;
        sleep(1);
    }
}
