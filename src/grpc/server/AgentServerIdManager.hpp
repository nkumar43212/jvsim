//
//  AgentServerIdManager.hpp
//  Telemetry Agent
//
//  Created by NITIN KUMAR on 2/16/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef AgentServerIdManager_hpp
#define AgentServerIdManager_hpp

#include <iostream>
#include <mutex>
#include <bitset>
#include <unistd.h>

typedef uint32_t id_idx_t;

#define SUBSCRIPTION_ID_SPACE_SIZE                  64
#define INTERNAL_SUBSCRIPTION_ID_SPACE_SIZE       8196
#define INTERNAL_SUBSCRIPTION_ID_SPACE_MIN        1000

// Will assume user is deligent enough to supply type T as std::bitmap<size_t>
template <typename T>
class AgentServerIdManager {
    // Control access to the space
    std::mutex _id_mutex;
    
    // Track allocated indices using a bit string
    T          _id_space;
    
    // Define minimum and maximum space
    size_t     _MIN_ID_SPACE_SIZE;
    size_t     _MAX_ID_SPACE_SIZE;

public:
    // Initialize the ID space manager
    AgentServerIdManager (id_idx_t min_id = 1) :
    _id_space(0),
    _MIN_ID_SPACE_SIZE(min_id),
    _MAX_ID_SPACE_SIZE(_id_space.size())
    {
    }

    // The size of the space
    size_t getSize ()
    {
        return _MAX_ID_SPACE_SIZE - _MIN_ID_SPACE_SIZE;
    }

    // Bounds of the space
    void getBounds (size_t *start, size_t *end)
    {
        *start = _MIN_ID_SPACE_SIZE;
        *end   = _MAX_ID_SPACE_SIZE - 1;
    }

    // Is this ID present in the space
    bool present (id_idx_t pos)
    {
        return _id_space.test((size_t)pos);
    }

    // Number of IDs allocated
    size_t count ()
    {
        return _id_space.count();
    }

    // Number of IDs availble
    size_t freeCount ()
    {
        return getSize() - count();
    }

    // Get a free ID. A return value of zero indicates failure
    id_idx_t allocate (void)
    {
        size_t start, end;

        getBounds(&start, &end);
        std::lock_guard<std::mutex> guard(_id_mutex);
        for (size_t i = start; i <= end; i++) {
            if (_id_space.test(i) == 0) {
                _id_space.set(i);
                return (id_idx_t)i;
            }
        }

        return getNullIdentifier();
    }

    // return the ID back to the free pool
    void deallocate (id_idx_t id, bool *bad_id = NULL)
    {
        std::lock_guard<std::mutex> guard(_id_mutex);
        if (bad_id) {
            *bad_id = (_id_space.test(id) == 0) ? true : false;
        }
        _id_space.reset(id);
    }

    id_idx_t getNullIdentifier (void)
    {
        return 0;
    }

    // Useful stats about the ID allocator
    void description (void)
    {
        id_idx_t start, end;
        
        getBounds(&start, &end);
        for (id_idx_t i = start; i <= end; i++) {
            if (_id_space.test(i)) {
                std::cout << i << " " << std::endl;
            }
        }
    }
};

#endif /* AgentServerIdManager_hpp */