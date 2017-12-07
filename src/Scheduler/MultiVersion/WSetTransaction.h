//
// Created by zhangan on 17-12-6.
//

#ifndef TRANSACTIONALSTOSCHEDULER_WSETTRANSACTION_H
#define TRANSACTIONALSTOSCHEDULER_WSETTRANSACTION_H


#include "../../Transaction/Transaction.h"

class WSetTransaction : BaseTransaction{
public:
    typedef std::set<DataItemLocator, DataItemLocatorComp> ReadSet, WriteSet;

    WriteSet m_writeSet;
    explicit WSetTransaction(long id);

    //主要逻辑实现位置
    std::function *pMainMethod;
    bool registMainMethod(std::function *method);


    virtual void initWriteSet() = 0;
};

class DataItemLocator{
public:
    enum OP_TYPE{ WRITE, UPDATE, DELETE};

    std::string entryName;//Unique Name of Container, used as ID
    std::string typeName;//Container type, used to get instance
    long key;//data key

    //OP_TYPE don't include when comparing two Locator
    OP_TYPE optype;


    DataItemLocator(std::string entry, std::string type, long key, OP_TYPE op){
        this->entryName = entry;
        this->key = key;
        this->typeName = type;
        this->optype = op;
    }
};

class DataItemLocatorComp{
public:
    bool operator()(const DataItemLocator &left, const DataItemLocator &right){
        return (left.entryName < right.entryName) || (left.typeName < right.typeName) || (left.key < right.key);
    }
};

class DataAction{
public:
    long txID;
    DataItemLocator dataItem;
};

#endif //TRANSACTIONALSTOSCHEDULER_WSETTRANSACTION_H
