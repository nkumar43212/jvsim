//
//  AgentServerIdManager.hpp
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

typedef u_int32_t id_idx_t;

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
    id_idx_t getSize ()
    {
        return ID_SPACE_SIZE - 1;
    }

    // Bounds of the space
    void getBounds (id_idx_t *start, id_idx_t *end)
    {
        *start = 1;
        *end   = ID_SPACE_SIZE - 1;
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
    id_idx_t freeCount ()
    {
        return getSize() - (id_idx_t)count();
    }

    // Get a free ID. A return value of zero indicates failure
    id_idx_t allocate (void)
    {
        id_idx_t start, end;

        getBounds(&start, &end);
        std::lock_guard<std::mutex> guard(_id_mutex);
        for (id_idx_t i = start; i <= end; i++) {
            if (_id_space.test(i) == 0) {
                _id_space.set(i);
                return i;
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
                std::cout << i << "\n";
            }
        }
    }
};

#endif /* AgentServerIdManager_hpp */
