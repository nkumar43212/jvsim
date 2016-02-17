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
#include <unistd.h>

#define ID_SPACE_SIZE 64

class AgentServerIdManager {
    // Control access to the space
    std::mutex _id_mutex;
    
    // Track allocated indices using a bit string
    std::bitset<ID_SPACE_SIZE> _id_space;
    
public:
    // Initialize the ID space manager
    AgentServerIdManager () : _id_space(0)
    {
    }
    
    // The size of the space
    uint32_t getSize ()
    {
        return ID_SPACE_SIZE - 1;
    }
    
    // Bounds of the space
    void getBounds (uint32_t *start, uint32_t *end)
    {
        *start = 1;
        *end   = ID_SPACE_SIZE - 1;
    }
    
    // Is this ID present in the space
    bool present (uint32_t pos)
    {
        return _id_space.test(pos);
    }
    
    // Number of IDs allocated
    size_t count ()
    {
        return _id_space.count();
    }
    
    // Number of IDs availble
    uint32_t freeCount ()
    {
        return getSize() - (uint32_t)count();
    }
    
    // Get a free ID. A return value of zero indicates failure
    uint32_t allocate()
    {
        uint32_t start, end;
        
        getBounds(&start, &end);
        std::lock_guard<std::mutex> guard(_id_mutex);
        for (int i = start; i <= end; i++) {
            if (_id_space.test(i) == 0) {
                _id_space.set(i);
                return i;
            }
        }
        
        return getNullIdentifier();
    }
    
    // return the ID back to the free pool
    void deallocate (uint32_t id, bool *bad_id = NULL)
    {
        std::lock_guard<std::mutex> guard(_id_mutex);
        if (bad_id) {
            *bad_id = (_id_space.test(id) == 0) ? true : false;
        }
        _id_space.reset(id);
    }
    
    uint32_t getNullIdentifier ()
    {
        return 0;
    }
    
    // Usefull stats about the ID allocator
    void     description()
    {
        uint32_t start, end;
        
        getBounds(&start, &end);
        for (int i = start; i <= end; i++) {
            if (_id_space.test(i)) {
                std::cout << i << "\n";
            }
        }
    }
};

#endif /* AgentServerIdManager_hpp */
