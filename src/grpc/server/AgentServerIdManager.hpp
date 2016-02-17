//
//  AgentServerIdManager.hpp
//  grpc
//
//  Created by NITIN KUMAR on 2/16/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentServerIdManager_hpp
#define AgentServerIdManager_hpp

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <bitset>

#define ID_SPACE_SIZE 64

class AgentServerIdManager {
    std::mutex _id_mutex;
    std::bitset<ID_SPACE_SIZE> _id_space;
    
public:
    // Initialize the ID space manager
    AgentServerIdManager () : _id_space(0)
    {
    }
    
    // Get a free ID. A return value of zero indicates failure
    uint32_t allocate()
    {
        std::lock_guard<std::mutex> guard(_id_mutex);
        for (int i = 1; i < ID_SPACE_SIZE; i++) {
            if (_id_space.test(i) == 0) {
                _id_space.set(i);
                return i;
            }
        }
        return 0;
    }
    
    // return the ID back to the free pool
    void deallocate (uint32_t id)
    {
        std::lock_guard<std::mutex> guard(_id_mutex);
        _id_space.reset(id);
    }
    
    uint32_t getNullIdentifier ()
    {
        return 0;
    }
    
    // Usefull stats about the ID allocator
    void     description()
    {
        for (int i = 1; i < ID_SPACE_SIZE; i++) {
            if (_id_space.test(i)) {
                std::cout << i << "\n";
            }
        }
    }
};

#endif /* AgentServerIdManager_hpp */
