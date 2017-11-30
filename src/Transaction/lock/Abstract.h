//
// Created by zhangan on 17-11-22.
//

#ifndef TRANSACTIONALSTOSCHEDULER_ABSTRACT_H
#define TRANSACTIONALSTOSCHEDULER_ABSTRACT_H

#include <stdint-gcc.h>
#include <unordered_set>
#include <mutex>

class AbstractLock{
public:
    //must use recursive_timed_mutex instead of time_mutex! if object already own the timed_mutex and want to lock it again,
    // then the behavior of timed_mutex is unknown
    typedef std::recursive_timed_mutex LockType;
    typedef std::unordered_set<LockType*> LockSet;

    virtual void init() = 0;
    virtual void lock(long key) = 0;
    virtual void releaseLocks()= 0;

    virtual ~AbstractLock ();
};


#endif //TRANSACTIONALSTOSCHEDULER_ABSTRACT_H
