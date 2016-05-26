//
//  agent_test_id_manager_ut.cpp
//
//  Created: 5/25/16.
//
//  Authors: ABBAS SAKARWALA
//           NITIN KUMAR
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "gtest/gtest.h"
#include "agent_test_id_manager_ut.hpp"
#include "AgentServerIdManager.hpp"

// Init test
template <typename AgentServerIdManagerType>
void
_init_test (AgentServerIdManagerType &mgr)
{
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        EXPECT_EQ(false, mgr.present((id_idx_t)i));
    }
    EXPECT_EQ(0, mgr.count());
    EXPECT_TRUE(mgr.freeCount() == mgr.getSize());
}

// Allocate
template <typename AgentServerIdManagerType>
void
_allocate (AgentServerIdManagerType &mgr,
           id_idx_t first)
{
    bool status;

    // allocate once
    id_idx_t i = mgr.allocate();
    EXPECT_EQ(first, i);
    EXPECT_EQ(1, mgr.count());

    // deallocate last allocation
    mgr.deallocate(i, &status);
    EXPECT_EQ(false, status);
    EXPECT_EQ(0, mgr.count());

    // allocate again - 1
    i = mgr.allocate();
    EXPECT_EQ(first, i);

    // allocate again - 2
    i = mgr.allocate();
    EXPECT_EQ(first+1, i);

    EXPECT_EQ(2, mgr.count());
}

// Allocate all
template <typename AgentServerIdManagerType>
void
_allocate_all (AgentServerIdManagerType &mgr)
{
    id_idx_t j = 0;
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        j = mgr.allocate();
    }
    EXPECT_EQ(end, j);
    EXPECT_TRUE(mgr.count() == ((end - start) + 1));
}

// Allocate and free
template <typename AgentServerIdManagerType>
void
_deallocate_all (AgentServerIdManagerType &mgr)
{
    id_idx_t j = 0;
    size_t start, end;

    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        bool was_corrupted;

        j = mgr.allocate();
        mgr.deallocate(j, &was_corrupted);
        EXPECT_TRUE(was_corrupted == false);
    }
}

// Allocate randomly
template <typename AgentServerIdManagerType>
void
_allocate_random (AgentServerIdManagerType &mgr)
{
    size_t start, end;
    mgr.getBounds(&start, &end);

    size_t size = end - start + 1;
    srand((unsigned)time(0));
    id_idx_t count = rand() % size;

    for (id_idx_t i = 0; i < count; i++) {
        mgr.allocate();
    }
    EXPECT_EQ(count, mgr.count());
    EXPECT_TRUE(mgr.freeCount() == (size - count));
}

// Exhaust
template <typename AgentServerIdManagerType>
void
_exhaust (AgentServerIdManagerType &mgr)
{
    size_t start, end;
    id_idx_t last = 0;

    // Allocate all
    mgr.getBounds(&start, &end);
    for (size_t i = start; i <= end; i++) {
        last = mgr.allocate();
    }

    // and then some
    id_idx_t id = mgr.allocate();
    EXPECT_TRUE(id == mgr.getNullIdentifier());

    // Free an ID
    mgr.deallocate(last);

    // Now allocation must pass
    id = mgr.allocate();
    EXPECT_TRUE(id != mgr.getNullIdentifier());
}

/* START TEST CASE */

// Initialize a space manager, and ensure that we don't have bogus values
TEST(id_manager, init) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    _init_test(mgr);
}

// Initialize a space manager, and ensure that we don't have bogus values
// With initial minimum id set to some value
TEST(id_manager, init_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                        mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    _init_test(mgr);
}

// Simple allocations
TEST(id_manager, allocate) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    _allocate(mgr, 1);
}

// Simple allocations
// With initial minimum id set to some value
TEST(id_manager, allocate_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                        mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);

    _allocate(mgr, INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
}

// Allocate all the IDs
TEST(id_manager, allocate_all) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    _allocate_all(mgr);
}

// Allocate all the IDs
// With initial minimum id set to some value
TEST(id_manager, allocate_all_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                        mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    _allocate_all(mgr);
}

// Allocate and Free
TEST(id_manager, deallocate_all) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    _deallocate_all(mgr);
}

// Allocate and Free
// With initial minimum id set to some value
TEST(id_manager, deallocate_all_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                        mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    _deallocate_all(mgr);
}

// Randomly allocate N Ids
TEST(id_manager, allocate_random) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    _allocate_random(mgr);
}

// Randomly allocate N Ids
// With initial minimum id set to some value
TEST(id_manager, allocate_random_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                        mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    _allocate_random(mgr);
}

// Exhaust the space and then check whether allocations fail
TEST(id_manager, exhaust) {
    AgentServerIdManager<std::bitset<SUBSCRIPTION_ID_SPACE_SIZE>> mgr;
    _exhaust(mgr);
}

// Exhaust the space and then check whether allocations fail
// With initial minimum id set to some value
TEST(id_manager, exhaust_with_min) {
    AgentServerIdManager<std::bitset<INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE>>
                                        mgr(INTERNAL_SUBSCRIPTION_ID_SPACE_MIN);
    _exhaust(mgr);
}
