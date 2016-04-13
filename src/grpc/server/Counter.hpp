//
//  Counter.hpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef Counter_hpp
#define Counter_hpp

#include <iostream>
#include <unistd.h>

class Counter {
    // Name of the counter
    std::string _name;
    
    // Packet counts. Reference values are needed for rate calculations
    uint64_t  _packets;
    uint64_t  _packets_reference;
    
    // Byte counts. Reference values are needed for rate calculations
    uint64_t  _bytes;
    uint64_t  _bytes_reference;
    
    // Instantaneous rates
    uint64_t  _packet_rate;
    uint64_t  _byte_rate;
    
    // The rate thread tracks when the last snapshot was taken
    time_t    _last_update_time;
    
    // Is rate calculation enabled
    pthread_t _rate_thread;
    
    // The scan internal of the rate thread, which wakes up every so often to compute deltas
    uint32_t  _rate_scan_interval;
    
    // Manage Rate calculation routines
    static void * CounterRateHandleCallback (void *context)
    {
        uint32_t interval = ((Counter *)context)->getRateInterval();
        
        while (true) {
            sleep(interval);
            ((Counter *)context)->computeRate(interval);
        }
    }
    
    void startRate(void *context)
    {
        if (!_rate_thread) {
            pthread_create(&_rate_thread, NULL, CounterRateHandleCallback, context);
        }
    }
    
    void stopRate(void)
    {
        pthread_cancel(_rate_thread);
        _rate_thread = NULL;
    }

public:
    Counter (const std::string name = "", bool enable_rate = false) : _name(name)
    {
        _packets = _packets_reference = 0;
        _bytes   = _bytes_reference   = 0;
        _packet_rate = 0;
        _byte_rate   = 0;
        _rate_scan_interval = 1;
        _rate_thread   = NULL;
        if (enable_rate) {
            startRate(this);
        }
    }
    
    ~Counter ()
    {
        stopRate();
    }
    
    // Accessors
    uint64_t getPackets (void)       { return _packets;       }
    uint64_t getBytes (void)         { return _bytes;         }
    uint64_t getPacketRate (void)    { return _packet_rate;   }
    uint64_t getByteRate (void)      { return _byte_rate;     }
    uint32_t getRateInterval (void)  { return _rate_scan_interval; }
    
    void increment (uint64_t n, uint64_t size)
    {
        _packets += n;
        _bytes += size;
    }
    
    void decrement (uint64_t n, uint64_t size)
    {
        if ((_packets < n) || (_bytes < size)) {
            return;
        }

        _packets -= n;
        _bytes -= size;
    }
    
    void computeRate (uint32_t seconds)
    {
        // What is the time now
        time_t current;
        time(&current);
        
        // How much time elapsed since we were last here
        double elapsed    = difftime(current, _last_update_time);
        _last_update_time = current;
       
        // Do the math
        _packet_rate       = (_packets - _packets_reference) / elapsed;
        _byte_rate         = (_bytes - _bytes_reference) / elapsed;
        _packets_reference = _packets;
        _bytes_reference   = _bytes;
    }
    
    void enableRate  ()
    {
        if (!_rate_thread) {
            startRate((void *) this);
        }
    }
    
    void disableRate ()
    {
        if (_rate_thread) {
            stopRate();
        }
    }
    
    void reset ()
    {
        _packets     = 0;
        _bytes       = 0;
        _packet_rate = 0;
        _byte_rate   = 0;
    }
    
    void description ()
    {
        std::cout << "Name = " << _name << "\n";
        std::cout << "packets = " << _packets << " , bytes = " << _bytes << "\n";
        if (_rate_thread) {
            std::cout << "pps     = " << _packet_rate << " , Bps = " << _byte_rate  << "\n";
        }
    }
    
    static void tests();
};

#endif /* Counter_hpp */
