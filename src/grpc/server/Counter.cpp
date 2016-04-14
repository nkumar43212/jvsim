//
//  Counter.cpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 1/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "Counter.hpp"

void *
incr_counter (void *context)
{
    while (true) {
        sleep(1);
        ((Counter *)context)->increment(100, 100);
    }
    return NULL;
}

void
Counter::tests ()
{
    // Simple counter
    Counter c1("c1");
    c1.increment(1, 100);
    c1.increment(10, 200);
    c1.description();
    c1.decrement(9, 10);
    c1.description();
    c1.decrement(100, 1000);
    c1.description();
    c1.reset();
    c1.description();
    
    // Rated
    Counter c2("c2");
    c2.increment(1, 100);
    c2.increment(10, 200);
    c2.description();
    c2.enableRate();
    c2.description();
    
    // Dynamic Counter
    Counter *c = new Counter("dyn");
    
    // Create a thread that updates c2
    pthread_t counter_test;
    pthread_create(&counter_test, NULL, incr_counter, (void *) c);
    
    while (true) {
        c->enableRate();
        sleep(5);
        c->description();
    }
}