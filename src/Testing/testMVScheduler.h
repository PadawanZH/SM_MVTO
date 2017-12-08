//
// Created by zhangan on 17-12-8.
//

#ifndef TRANSACTIONALSTOSCHEDULER_TESTMVSCHEDULER_H
#define TRANSACTIONALSTOSCHEDULER_TESTMVSCHEDULER_H
#include "../Scheduler/MultiVersion/MVBaseContainer.h"
#include <map>
#include <iostream>

/*
 * test the instantiate of container
 */
class TestMVContainer_IntMap : public MVBaseContainer<int>{
public:

    std::map<long, typename MVBaseContainer<int>::DataItem> map;

    TestMVContainer_IntMap();

    typename MVBaseContainer<int>::DataItem* getOrCreateDataItem(long key) override;

    void print();
};

class TestMVContainer_StringMap : public MVBaseContainer<std::string>{
public:
    TestMVContainer_StringMap();

    std::map<long, typename MVBaseContainer<std::string>::DataItem> map;

    typename MVBaseContainer<std::string>::DataItem* getOrCreateDataItem(long key) override;

    void print();
};

#endif //TRANSACTIONALSTOSCHEDULER_TESTMVSCHEDULER_H
