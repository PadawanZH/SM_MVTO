//
// Created by zhangan on 17-11-21.
//

#include "Boosted_map.h"
#include "../Util/Log.h"
#include "../Transaction/Transaction.h"
#include "cassert"

Boosted_map::Boosted_map () : BaseContainer((char*)"Boosted Map") {
    lock = new LockKey();
}

Boosted_map::~Boosted_map() {
    delete lock;
}

void Boosted_map::releaseLocks () {
    if(lock){
        lock->releaseLocks();
    }else{
        debuglog(LogLevel::ERROR, "Container [%s] has null pointer of member lock", name);
    }
}


/*
 * Function: transactional memory version of Insert
 * return: true if insert the <K,V> pair; false if already exist one <K,_>
 *
 * Method ------------------------- Inverse
 * 1. insert(k, v)/true  ---------- remove(k)/true
 * 2. insert(k, v)/false ---------- Nothing
 *
 */
bool Boosted_map::tm_insert (long key, void* data) {
    lock->lock(key);
    bool result = hash_map.insert(std::make_pair(key, data));
    debuglog(LogLevel::INFO, "Tx [%ld] INSERT with key %ld",BaseTransaction::txID, key);
    if(result){
        BaseTransaction::undoLog.push_back([this, key]{
            debuglog(LogLevel::WARN, "Tx [%ld] undo INSERT action by remove key %ld",BaseTransaction::txID, key);
            return this->hash_map.erase(key);
        });
    }

    return result;
}

/*
 * Function: transactional memory version of Remove/Erase
 * return: true if remove the <K,V> pair; false if not finding <K, V> inside
 *
 * Method ------------------------------- Inverse
 * 1. remove(k)/<true, <k,v>>  ---------- insert(k, v)/true
 * 2. remove(k)/<false,_>      ---------- Nothing
 */
std::pair<bool, std::pair<long, void*>> Boosted_map::tm_remove (long key) {
    lock->lock(key);
    tbb::concurrent_hash_map<long, void*>::const_accessor reader;
    bool result = hash_map.find(reader, key);

    if(result){//find it! delete it
        void* data = reader->second;
        reader.release(); // must release the reader first, or erase action will wait forever

        assert(hash_map.erase(key));//must be true!
        debuglog(LogLevel::INFO, "Tx [%ld] REMOVE with key %ld",BaseTransaction::txID, key);
        BaseTransaction::undoLog.push_back([this, key, data](){
            debuglog(LogLevel::WARN, "Tx [%ld] undo REMOVE action by insert key %ld",BaseTransaction::txID, key);
            return hash_map.insert(std::make_pair(key, data));
        });

        std::pair<long, void*> deletedEntry = std::make_pair(key, data);
        return std::make_pair(true, deletedEntry);
    }else{
        debuglog(LogLevel::INFO, "Tx [%ld] want to REMOVE with key %ld, but NOT FOUND IN MAP",BaseTransaction::txID, key);
        return std::make_pair(false, std::make_pair(0, nullptr));
    }
}

/*
 * Function: transactional memory version of Contains
 * return: true if contain the <K,_> pair; false if not finding <K,_> inside
 *
 * Method ------------------------------- Inverse
 * contain(k)/_ ------------------------- Nothing
 */
bool Boosted_map::tm_contains(long key) {
    lock->lock(key);
    tbb::concurrent_hash_map<long, void*>::const_accessor reader;
    bool result = hash_map.find(reader, key);
    debuglog(LogLevel::INFO, "Tx [%ld] check CONTAINS with key %ld, and result is %s",BaseTransaction::txID, key, result? (char*)"true" : (char*)"false");
    return result;

}

/*
 * Function: transactional memory version of Find
 * return: V in <k,V> if not nullptr;
 *
 * Method ------------------------------- Inverse
 * find(k)/_ ---------------------------- Nothing
 */
void* Boosted_map::tm_find(long key) {
    lock->lock(key);
    tbb::concurrent_hash_map<long, void*>::const_accessor reader;
    bool result = hash_map.find(reader, key);
    debuglog(LogLevel::INFO, "Tx [%ld] FIND with key %ld, and result is %s",BaseTransaction::txID, key, result? (char*)"true" : (char*)"false");
    if(result){
        return reader->second;
    }else{
        return nullptr;
    }
}

/*
 * For Pre insert data for testing
 */
bool Boosted_map::nonTMInsert(long key, void* data){
    return hash_map.insert(std::make_pair(key, data));
}

void Boosted_map::print() {
    std::cout<<"Content of whole MAP: \n------------------------\n";
    std::cout<<"key \t\t value\n";
    for(auto it = hash_map.begin(); it != hash_map.end(); it++){
        std::cout<<it->first<<" \t\t "<<(char*)it->second<<std::endl;
    }
}