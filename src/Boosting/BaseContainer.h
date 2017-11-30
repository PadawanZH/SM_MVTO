//
// Created by zhangan on 17-11-17.
//

#ifndef TRANSACTIONALSTOSCHEDULER_BASECONTAINER_H
#define TRANSACTIONALSTOSCHEDULER_BASECONTAINER_H


#include <tbb/concurrent_hash_map.h>
#include <vector>
#include "../Transaction/lock/LockKey.h"

class BaseContainer {
public:
    AbstractLock* lock;
    char* name = (char*)"no name";
    explicit BaseContainer(char* name);

    virtual ~BaseContainer ();

    virtual void releaseLocks() = 0;
};


#endif //TRANSACTIONALSTOSCHEDULER_BASECONTAINER_H
