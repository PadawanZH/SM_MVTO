//
// Created by zhangan on 17-11-17.
//

#ifndef TRANSACTIONALSTOSCHEDULER_LOCKKEY_H
#define TRANSACTIONALSTOSCHEDULER_LOCKKEY_H

#include <mutex>
#include "tbb/concurrent_hash_map.h"
#include "Abstract.h"
#include <chrono>

class LockKey : public AbstractLock{
    typedef tbb::concurrent_hash_map<long, AbstractLock::LockType*> LockMap;

public:
    LockKey ();

    virtual ~LockKey ();

    void init() override ;
    void lock(long key) override;
    void releaseLocks() override;

    static thread_local AbstractLock::LockSet m_lockSet;
private:
    LockMap m_lockMap;

};

#endif //TRANSACTIONALSTOSCHEDULER_LOCKKEY_H
