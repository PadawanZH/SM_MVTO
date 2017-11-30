//
// Created by zhangan on 17-11-17.
//

#include <sstream>
#include <thread>
#include "LockKey.h"
#include "../TXAbortException.h"
#include "../../Util/Log.h"
#include "../Transaction.h"

thread_local AbstractLock::LockSet LockKey::m_lockSet;

LockKey::LockKey () {
    init ();
}

LockKey::~LockKey () {
    for(LockMap::iterator it = m_lockMap.begin(); it != m_lockMap.end(); it++){
        delete it->second;
    }
}

void LockKey::init () {
    if(!m_lockSet.empty()){
        m_lockSet.clear();
    }
}

void LockKey::lock (long key) {
    LockMap::accessor lockMapWriter;
    LockType* lock = nullptr;

    //create lock in lockMap if there is no lock in key yet
    if(m_lockMap.insert(lockMapWriter, key)){
        lock = new LockType();
        lockMapWriter->second = lock;
    }else{
        lock = lockMapWriter->second;
    }

    //try to acquire the lock
    if(m_lockSet.insert(lock).second){//lock is successfully inserted into lockset
        if(lock->try_lock_for(std::chrono::milliseconds(100))){
            debuglog(LogLevel::INFO, "thread succeeded to lock key: %ld", key);
        }else{
            debuglog(LogLevel::WARN, "failed to acquire the LockKey with key: %ld, transaction aborted", key);
            m_lockSet.erase(lock);
            throw TXAbortException("TxAbortException");  //relay on exception to perform a roll back
        }
    }
}

void LockKey::releaseLocks () {
    //lockset is thread-local, so it is thread-safe
    for(LockSet::iterator it = m_lockSet.begin(); it != m_lockSet.end(); it++){
        (*it)->unlock();
    }
    m_lockSet.clear();
}

