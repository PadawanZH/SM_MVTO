//
// Created by zhangan on 17-12-6.
//

#ifndef TRANSACTIONALSTOSCHEDULER_MVBASECONTAINER_H
#define TRANSACTIONALSTOSCHEDULER_MVBASECONTAINER_H


#include "../../Boosting/BaseContainer.h"
#include "WSetTransaction.h"

class AbstractMVBaseContainer : public BaseContainer{
public:
    AbstractMVBaseContainer(char *name);
    virtual void constructWriteOnDataItem(long txID, DataItemLocator dataItem) = 0;
};


template<typename valType>
class MVBaseContainer : public AbstractMVBaseContainer{
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
        long key;
        valType value;
        WaitListItem *waitListHeader;

        DataItem(long key, valType initValue){
            this->key = key;
            value = initValue;
            waitListHeader = nullptr;
        }

        void FreeWaitList(){
            WaitListItem* tmp;
            WaitListItem* p = waitListHeader;
            while(p){
                tmp = p;
                p = p->next;
                delete tmp;
            }
        }

        ~DataItem(){
            FreeWaitList();
        }
    };

    //Init section
    explicit MVBaseContainer(char *name);
    void constructWriteOnDataItem(long txID, DataItemLocator dataItem);

    //do nothing in releaseLocks, because it not using the locks
    void releaseLocks() override{

    }
protected:
    virtual DataItem* getOrCreateDataItem(long key) = 0;
};


#endif //TRANSACTIONALSTOSCHEDULER_MVBASECONTAINER_H
