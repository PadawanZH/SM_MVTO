//
// Created by zhangan on 17-12-8.
//

#include "testMVScheduler.h"
TestMVContainer_IntMap::TestMVContainer_IntMap() : MVBaseContainer<int>((char*)"IntMap"){}

typename MVBaseContainer<int>::DataItem* TestMVContainer_IntMap::getOrCreateDataItem(long key) {
    typename std::map<long, typename MVBaseContainer<int>::DataItem>::iterator it = map.find(key);
    if(it != map.end()){
        return &it->second;
    }else{
        DataItem* pitem = new DataItem(key ,0);
        map.insert(std::pair<long, typename MVBaseContainer<int>::DataItem>(key, *pitem));
        return pitem;
    };
}

void TestMVContainer_IntMap::print() {
    std::cout<<"this is int map speaking"<<std::endl;
}



TestMVContainer_StringMap::TestMVContainer_StringMap()  : MVBaseContainer<std::string>((char*)"StringMap"){}

typename MVBaseContainer<std::string>::DataItem* TestMVContainer_StringMap::getOrCreateDataItem(long key) {
    typename std::map<long, typename MVBaseContainer<std::string>::DataItem>::iterator it = map.find(key);
    if(it != map.end()){
        return &it->second;
    }else{
        DataItem* pitem = new DataItem(key ,0);
        map.insert(std::pair<long, typename MVBaseContainer<std::string>::DataItem>(key, *pitem));
        return pitem;
    };
}

void TestMVContainer_StringMap::print() {
    std::cout<<"this is string map speaking"<<std::endl;
}