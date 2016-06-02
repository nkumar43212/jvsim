//
//  agent_test_counter_ut.cpp
//
//  Created: 6/2/16.
//
//  Authors: ABBAS SAKARWALA
//           NITIN KUMAR
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "agent_test_counter_ut.hpp"
#include "Counter.hpp"

TEST(Counter, static_counter_1) {
    // Create counter
    Counter c1("static_counter_1");
    EXPECT_EQ(1, c1.getRateInterval());

    // Increment the counter packets and bytes
    c1.increment(1, 100);
    c1.increment(10, 200);
    EXPECT_EQ(11, c1.getPackets());
    EXPECT_EQ(300, c1.getBytes());

    // Decrement the counter packets and bytes
    c1.decrement(9, 50);
    EXPECT_EQ(2, c1.getPackets());
    EXPECT_EQ(250, c1.getBytes());

    // Decrement the counter packets and bytes by huge number
    // Should not decrement anything
    c1.decrement(100, 1000);
    EXPECT_EQ(2, c1.getPackets());
    EXPECT_EQ(250, c1.getBytes());

    // Reset the counter
    c1.reset();
    c1.description();
    EXPECT_EQ(0, c1.getPackets());
    EXPECT_EQ(0, c1.getBytes());
}

TEST(Counter, static_counter_2) {
    // Create counter
    Counter c2("static_counter_2");

    // Enable the rate thread
    c2.enableRate();

    // Increment values
    c2.increment(100, 10000);
    sleep(1);
    EXPECT_EQ(100, c2.getPackets());
    EXPECT_EQ(10000, c2.getBytes());

    // Increment values cont'd
    c2.increment(500, 50000);
    sleep(1);
    EXPECT_EQ(600, c2.getPackets());
    EXPECT_EQ(60000, c2.getBytes());

    // Increment values cont'd
    c2.increment(100, 10000);
    sleep(1);
    EXPECT_EQ(700, c2.getPackets());
    EXPECT_EQ(70000, c2.getBytes());

    // Increment values cont'd
    c2.increment(500, 50000);
    sleep(1);
    EXPECT_EQ(1200, c2.getPackets());
    EXPECT_EQ(120000, c2.getBytes());
    EXPECT_EQ(500, c2.getPacketRate());
    EXPECT_EQ(50000, c2.getByteRate());

    c2.description();

    // Stop the rate thread
    c2.disableRate();
}

#define RUN_TEST_COUNT  10
void *
incr_counter (void *context)
{
    uint32_t count = 0;
    while (count < RUN_TEST_COUNT) {
        sleep(1);
        ((Counter *)context)->increment(100, 100);
        count++;
    }
    return NULL;
}

TEST(Counter, dynamic_counter) {
    // Create counter
    Counter *c = new Counter("dynamic_counter");

    // Enable the rate thread
    c->enableRate();

    // Create a thread that updates dynamic counter
    pthread_t counter_test;
    pthread_create(&counter_test, NULL, incr_counter, (void *) c);

    // Sleep random secs and check rate
    sleep(5);
    EXPECT_EQ(100, c->getPacketRate());
    EXPECT_EQ(100, c->getByteRate());
    c->description();

    // Wait for the thread to finish
    pthread_join(counter_test, NULL);

    // Delete the counter
    delete c;
}
