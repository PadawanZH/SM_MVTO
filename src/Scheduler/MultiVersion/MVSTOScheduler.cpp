//
// Created by zhangan on 17-12-6.
//

#include "MVSTOScheduler.h"
#include "../../Util/Log.h"
#include "../../Util/ContainerInstantiate.h"

MVSTOScheduler::MVSTOScheduler(std::list<WSetTransaction> *txList) {
    if(txList != nullptr){
        init(txList);
    }else{
        debuglog(LogLevel::ERROR, "When create MVSTOScheduler, the given argument txList is NULL!");
    }
}

void MVSTOScheduler::init(std::list<WSetTransaction> *txList) {
    initTxList(txList);
    initDataStructure();
}

void MVSTOScheduler::initTxList(std::list<WSetTransaction> *txList) {
    this->m_txList = txList;
}

void MVSTOScheduler::registDataStructure(std::string entryName, std::string typeName ){
    MVBaseContainer* instance = InstanceOfMVContainer(typeName);

    if(instance != nullptr){
        DataStructureMap::const_accessor cacc;
        if(m_dataStructure.insert(cacc, entryName)){
            cacc->second = instance;
        }else{
            debuglog(LogLevel::ERROR, "Already exist datastructure named \"%s\" with typeNmae \"%s\", yet still trying to create new one in map", entryName.c_str(), typeName.c_str());
        }
    }else{
        debuglog(LogLevel::ERROR, "NULL pointer returned by InstanceOfContainer with typename=%s", typeName.c_str());
    }
}

/*
 * regist write action to multiversion structure, that is to add the tx to the chain of the target item
 */
void MVSTOScheduler::registWriteAction(long txID, DataItemLocator dataItem) {
    DataStructureMap::const_accessor cacc;
    if(this->m_dataStructure.find(cacc, dataItem.entryName)){
        MVBaseContainer* instance = cacc->second;
        instance->constructWriteOnDataItem(txID, dataItem);
    }else{
        debuglog(LogLevel::ERROR, "when registWriteAction, no entry: %s not found in map", dataItem.entryName.c_str());
    }
}

void MVSTOScheduler::initDataStructure() {
    std::set<std::string> dataAlreadyRegist;
    for(auto txIT = m_txList->begin(); txIT != m_txList->end(); txIT++){
        WSetTransaction::WriteSet ws = (*txIT).m_writeSet;
        for(auto wsIT = ws.begin(); wsIT != ws.end(); wsIT++){
            auto ret = dataAlreadyRegist.insert((*wsIT).entryName);
            if(ret.second){
                //new structure
                registDataStructure((*wsIT).entryName, (*wsIT).typeName);
            }else{
                registWriteAction((*txIT).txID, (*wsIT));
            }
        }
    }
}

