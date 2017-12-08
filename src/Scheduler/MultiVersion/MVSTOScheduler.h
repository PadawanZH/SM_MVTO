//
// Created by zhangan on 17-12-6.pointer of template class
//

#ifndef TRANSACTIONALSTOSCHEDULER_MVSTOSCHEDULER_H
#define TRANSACTIONALSTOSCHEDULER_MVSTOSCHEDULER_H

#include <list>
#include "WSetTransaction.h"
#include "MVBaseContainer.h"

class MVSTOScheduler {
public:
    typedef tbb::concurrent_hash_map<std::string, AbstractMVBaseContainer*> DataStructureMap;

    std::list<WSetTransaction*> *m_txList;
    DataStructureMap m_dataStructure;

    explicit MVSTOScheduler(std::list<WSetTransaction*> *txList);
    //init method
    void init(std::list<WSetTransaction*> *txList);
    void initTxList(std::list<WSetTransaction*> *txList);
    void initDataStructure();
    void registDataStructure(std::string entryName, std::string typeName);
    void registWriteAction(long txID, DataItemLocator dataItem);


    void* readMapData(std::string entryName, long key);
    bool writeData(std::string entryName, long key, void* data);

    //main schedule algorithm
    void startSchedule();
};



#endif //TRANSACTIONALSTOSCHEDULER_MVSTOSCHEDULER_H
