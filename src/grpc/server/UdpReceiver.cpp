//
//  UdpReceiver.cpp
//  Telemetry Agent
//
//  Created: 5/11/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <algorithm>
#include <string>
#include <arpa/inet.h>
#include "UdpReceiver.hpp"

// Global instance
UdpReceiver* udpreceiver;

void
UdpReceiver::operator()()
{
    ssize_t no_of_bytes;

    /* create the socket */
    _sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (_sock < 0) {
        _logger->log("Opening socket error on port " +
                     std::to_string(_server.sin_port));
        exit(1);
    }
    
    /* bind port to the IP */
    if(bind(_sock, (struct sockaddr *)&_server, sizeof(_server))<0) {
        _logger->log("Bind socket error on port " +
                     std::to_string(_server.sin_port));
        exit(1);
    }

    char server_ip[20];
    inet_ntop(AF_INET, &(_server.sin_addr), &server_ip[0], INET_ADDRSTRLEN);
    std::string server_ip_str(server_ip);
    _logger->log("UDP Receiver ready. Listening on IP=" + server_ip_str +
                 ", Port=" + std::to_string(_server.sin_port));

    /* continuously listen on the specified port */
    while (true) {
        no_of_bytes = recvfrom(_sock, _request->buf, PACKET_SIZE, 0,
                     (struct sockaddr *)&_request->from, &_request->fromlen);
        if(no_of_bytes < 0) {
            _recvfrom_error++;
            // TODO ABBAS --- supress this later
            _logger->log("Error: recvfrom no_of_bytes < 0");
        }
        else {
            // Increment valid pkts counter
            _total_pkts_received++;
            _messages.increment(1, no_of_bytes);

            // TODO ABBAS --- supress this later
            if (_total_pkts_received % 1000 == 0) {
                _logger->log("Count = " + std::to_string(_total_pkts_received));
            }

            // Extract the subscription id
            // first four bytes is subscription id
            id_idx_t *pIntid = (id_idx_t *)_request->buf;
            id_idx_t int_id = ntohl(*pIntid);

            // Per int_id counter
            if (_stats_topics.count(int_id) == 0) {
                _stats_topics[int_id] = Counter(std::to_string(int_id));
            }
            _stats_topics[int_id].increment(1, no_of_bytes-sizeof(id_idx_t));

            // std::cout << "Int id = " << int_id << std::endl;
            std::lock_guard<std::mutex> guard(_udpreceiver_mutex);

            // Find the external id vector list
            Int_Ext_List_DB::iterator itr = _int_to_ext_list_store.find(int_id);
            std::vector<int> *v;

            // Did we find it ?
            if (itr != _int_to_ext_list_store.end()) {
                v = itr->second;
            } else {
                // std::cout << "Error ... missing internal id mapping to "
                //           << "external id mapping: " << n << std::endl;
                _int_to_ext_list_not_found++;
                continue;
            }

            // Iterate through the external id vector list
            for (auto &ext_id : *v) {
                Ext_id_Worker_DB::iterator itr = _ext_id_worker_store.find(ext_id);
                AgentSubscriptionUdpWorker *wp;
                // Did we find it ?
                if (itr != _ext_id_worker_store.end()) {
                    wp = itr->second;
                } else {
                    // std::cout << "Error cannot find worker for "
                    //           << ext_id << std::endl;
                    _worker_not_found++;
                    continue;
                }
                {
                    std::unique_lock<std::mutex> l(wp->_m);
                    std::vector<unsigned char> vec(_request->buf+sizeof(id_idx_t),
                                                   _request->buf+no_of_bytes);
                    wp->_q.push(vec);
                    l.unlock();
                    wp->_c.notify_one();
                }
            }
        }
    
    }
}

void
UdpReceiver::add_worker (int ext_id, AgentSubscriptionUdpWorker *wp)
{
    _logger->log("Add Worker Mapping Ext id = " + std::to_string(ext_id));

    // Grab the mutex
    std::lock_guard<std::mutex> guard(_udpreceiver_mutex);

    // Find if mapping already exist
    Ext_id_Worker_DB::iterator itr = _ext_id_worker_store.find(ext_id);

    // Did we find it ?
    if (itr != _ext_id_worker_store.end()) {
        _worker_add_error++;
        _logger->log("Error: Mapping of external id " + std::to_string(ext_id) +
                     " to worker thread already found ");
    } else {
        // Add to the store
        _ext_id_worker_store[ext_id] = wp;
    }
}

void
UdpReceiver::del_worker(int ext_id, AgentSubscriptionUdpWorker *wp)
{
    _logger->log("Delete Worker Mapping Ext id = " + std::to_string(ext_id));
    
    // Grab the mutex
    std::lock_guard<std::mutex> guard(_udpreceiver_mutex);

    // Find if mapping already exist
    Ext_id_Worker_DB::iterator itr = _ext_id_worker_store.find(ext_id);
    
    // Did we find it ?
    if (itr != _ext_id_worker_store.end()) {
        _ext_id_worker_store.erase(ext_id);
    } else {
        _worker_del_error++;
        _logger->log("Error: Mapping of external id " + std::to_string(ext_id) +
                     " to worker thread not found ");
    }
}

void
UdpReceiver::add_mapping (int ext_id, int int_id)
{
    _logger->log("Add Mapping Ext id = " + std::to_string(ext_id) +
                 " Int id = " + std::to_string(int_id));

    // Grab the mutex
    std::lock_guard<std::mutex> guard(_udpreceiver_mutex);

    // Find if mapping already exist
    Int_Ext_List_DB::iterator itr = _int_to_ext_list_store.find(int_id);

    // Ext id is a list
    std::vector<int> *v;

    // Did we find it ?
    if (itr != _int_to_ext_list_store.end()) {
        // Add this member in the list
        v = itr->second;
        v->push_back(ext_id);
    } else {
        // create new list
        v = new std::vector<int>;
        v->push_back(ext_id);

        _int_to_ext_list_store[int_id] = v;
    }
    
    _logger->log("Add Successful");
}

void
UdpReceiver::del_mapping (int ext_id, int int_id)
{
    _logger->log("Delete Mapping Ext id = " + std::to_string(ext_id) +
                 " Int id = " + std::to_string(int_id));
    
    // Grab the mutex
    std::lock_guard<std::mutex> guard(_udpreceiver_mutex);

    // Find if mapping already exist
    Int_Ext_List_DB::iterator itr = _int_to_ext_list_store.find(int_id);

    // Ext id is a list
    std::vector<int> *v;

    // Did we find it ?
    if (itr != _int_to_ext_list_store.end()) {
        // Search and remove from the list
        v = itr->second;
        v->erase(std::remove(v->begin(), v->end(), ext_id), v->end());

        // If list is empty, delete list, remove entry
        if (v->size() == 0) {
            delete v;
            _int_to_ext_list_store.erase(int_id);
        }
        _logger->log("Delete Successful");
    } else {
        _logger->log("Nothing to delete");
    }
}