//
// Created by zhangan on 17-11-21.
//

#ifndef TRANSACTIONALSTOSCHEDULER_BOOSTED_MAP_H
#define TRANSACTIONALSTOSCHEDULER_BOOSTED_MAP_H


#include "BaseContainer.h"
#include "tbb/concurrent_hash_map.h"

class Boosted_map : public BaseContainer{
public:
    Boosted_map ();
    ~Boosted_map();

    bool tm_insert(long key, void* data);
    std::pair<bool, std::pair<long, void*>> tm_remove(long key); //<标志位，成功删除的<K,V>对，仅在标志位=true时有效>
    bool tm_contains(long key);
    void* tm_find(long key);

    void releaseLocks() override;


    bool nonTMInsert(long key, void* data);
    void print();// only for test
public://public only for test
    tbb::concurrent_hash_map<long, void*> hash_map;
};


#endif //TRANSACTIONALSTOSCHEDULER_BOOSTED_MAP_H
