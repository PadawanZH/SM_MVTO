//
// Created by zhangan on 17-12-6.
//

#ifndef TRANSACTIONALSTOSCHEDULER_MVBASECONTAINER_H
#define TRANSACTIONALSTOSCHEDULER_MVBASECONTAINER_H


#include "../../Boosting/BaseContainer.h"
#include "WSetTransaction.h"

template<typename valType>
class MVBaseContainer : BaseContainer{
public:

    class WaitListItem{
    public:
        enum OWNERSHIP {OWNER, WAITER};

        long txID;
        valType* pOldValue; //oldPointer should never be null!!!
        valType NewValue;
        bool dirtyBit;

        DataItemLocator::OP_TYPE op;
        OWNERSHIP ownership;

        WaitListItem *next;

        WaitListItem(long txID, valType* Oldpointer, DataItemLocator::OP_TYPE op, OWNERSHIP ownership){
            this->txID = txID;

            pOldValue = Oldpointer;

            dirtyBit = true;
            this->ownership = ownership;
            this->op = op;
            this->next = nullptr;
        }
    };

    class DataItem{
    public:
        valType value;
        WaitListItem *waitListHeader;
    };

    //Init section
    explicit MVBaseContainer(char *name);
    void constructWriteOnDataItem(long txID, DataItemLocator dataItem);
protected:
    virtual DataItem* getOrCreateDataItem(long key) = 0;
};


#endif //TRANSACTIONALSTOSCHEDULER_MVBASECONTAINER_H
