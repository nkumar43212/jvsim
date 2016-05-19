//
//  UdpReceiver.hpp
//  Telemetry Agent
//
//  Created: 5/11/16.
//
//  Authors: ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef UdpReceiver_hpp
#define UdpReceiver_hpp

#include <mutex>
#include <map>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>

#include "AgentSubscriptionUdpWorker.hpp"
#include "AgentServerLog.hpp"
#include "Counter.hpp"

// TODO --- revisit
typedef uint32_t id_idx_t;

// External subscription id to Workerthread store
typedef std::map<id_idx_t, AgentSubscriptionUdpWorker *>     Ext_id_Worker_DB;
// Internal subscription id to list of external subscription id store
typedef std::map<id_idx_t, std::vector<int> *>               Int_Ext_List_DB;

// Per-internal-sub-id counters
typedef std::map<const id_idx_t, Counter>                    IntSubIdCounterMap;

#define PACKET_SIZE     65535

// Packet buffer
struct Request {
    socklen_t fromlen;
    struct sockaddr_in from;
    char buf[PACKET_SIZE];
};

class UdpReceiver  {
private:
    // Logging machine
    AgentServerLog          *_logger;

    // Data store
    Ext_id_Worker_DB        _ext_id_worker_store;
    Int_Ext_List_DB         _int_to_ext_list_store;

    // Guards against addition/deletion
    std::mutex              _udpreceiver_mutex;

    // UDP server info
    struct sockaddr_in      _server;
    int                     _sock;
    Request                 *_request;

    // Counters
    Counter                 _messages;
    IntSubIdCounterMap      _stats_topics;
    uint64_t                _total_pkts_received;
    uint64_t                _worker_not_found;
    uint64_t                _int_to_ext_list_not_found;
    uint64_t                _recvfrom_error;

    uint64_t getTotalPktsRecvd(void)    { return _total_pkts_received; }
    uint64_t getWorkerErr(void)         { return _worker_not_found; }
    uint64_t getInttoExtListErr(void)   { return _int_to_ext_list_not_found; }
    uint64_t getRecvFromError(void)     { return _recvfrom_error; }

    uint64_t _worker_add_error;
    uint64_t _worker_del_error;

    uint64_t getExtIdWorkerStoreSize(void)
                                    { return _ext_id_worker_store.size(); }
    uint64_t getIntExtListStoreSize(void)
                                    { return _int_to_ext_list_store.size(); }

public:
    UdpReceiver(AgentServerLog *logger,
                in_addr_t SERVER_IP,
                in_port_t SERVER_PORT):
                _logger(logger),
                _messages("UdpReceiver-MessageCounter", true)
    {
        _total_pkts_received = 0;
        _worker_not_found = _int_to_ext_list_not_found = _recvfrom_error = 0;
        _worker_add_error = _worker_del_error = 0;

        // Define server contents
        int length = sizeof(_server);
        bzero(&_server, length);
        _server.sin_family = AF_INET;
        _server.sin_addr.s_addr = SERVER_IP; // INADDR_ANY
        _server.sin_port=htons(SERVER_PORT);

        // Lets look here
        _request = (struct Request*) malloc(sizeof(Request));
        _request->fromlen = sizeof(struct sockaddr_in);
    }
    
    ~UdpReceiver()
    {
        if (_request) {
            free(_request);
        }
    }

    void operator()();
    void add_worker(int ext_id, AgentSubscriptionUdpWorker *wp);
    void del_worker(int ext_id, AgentSubscriptionUdpWorker *wp);
    void add_mapping(int ext_id, int int_id);
    void del_mapping(int ext_id, int int_id);
};

extern UdpReceiver *udpreceiver;

#endif /* UdpReceiver_hpp */