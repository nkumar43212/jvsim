//
//  AgentSubscriptionLimits.hpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 1/30/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentSubscriptionLimits_hpp
#define AgentSubscriptionLimits_hpp

#include <iostream>

class AgentSubscriptionLimits {
    uint64_t _records;
    uint32_t _seconds;
    time_t   _start_time;
    
public:
    AgentSubscriptionLimits (uint64_t records = 0, uint32_t seconds = 0) : _records(records), _seconds(seconds)
    {
        _start_time = NULL;
    }
    
    uint32_t getSeconds () { return _seconds; }
    uint64_t getRecords () { return _records; }
    
    void start ()
    {
        time(&_start_time);
    }
    
    void reset ()
    {
        start();
    }
    
    bool expired (uint32_t current_records)
    {
        bool time_expired = expiredTime();
        bool records_expired = expiredCount(current_records);
        
        return time_expired || records_expired;
    }
    
    bool expiredTime ()
    {
        if (_seconds == 0) {
            return false;
        }
        
        time_t current_time;
        time(&current_time);
        double elapsed = difftime(current_time, _start_time);
        if (elapsed < _seconds) {
            return false;
        }
        return true;
    }
    
    bool expiredCount (uint32_t current_records)
    {
        if (_records == 0) {
            return false;
        }
        
        return (current_records < _records) ? false : true;
    }
    
    void description ()
    {
        time_t current_time;
        
        std::cout << "Records = " << _records << "\n";
        std::cout << "Seconds = " << _seconds << "\n";
        if (!_seconds) {
            return;
        }
        
        // Time left ?
        time(&current_time);
        double elapsed = difftime(current_time, _start_time);
        if (elapsed < _seconds) {
            std::cout << "Time left = " << (_seconds - elapsed) << "secs\n";
        } else {
            std::cout << "Time expired = " << (elapsed - _seconds) << "secs\n";
        }
    }
};


#endif /* AgentSubscriptionLimits_hpp */
