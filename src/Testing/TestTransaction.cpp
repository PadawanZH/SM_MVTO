//
// Created by zhangan on 17-11-27.
//
#include "TestTransaction.h"

IntegerContainer::IntegerContainer (int initVal) : BaseContainer((char*)"Integer"){
    this->baseNum = initVal;
    this->lock = new LockKey();
}

int IntegerContainer::add (int x) {
    lock->lock(x);
    baseNum += x;

    std::function<bool()> undoAction = [this,x](){
        this->baseNum -= x;
        std::cout<<"Aborting action by decrease the number by "<<x<<std::endl;
        return true;
    };

    BaseTransaction::undoLog.push_back(undoAction);

    return baseNum;
}

void IntegerContainer::releaseLocks () {
    if(this->lock){
        lock->releaseLocks();
    }else{
        debuglog(LogLevel::WARN, "Lock is NULL in Container %s", name);
    }
}
IntegerContainer::~IntegerContainer () {
    baseNum = 0;
    delete lock;
}

int testMain(){
    IntegerContainer* tmInt = new IntegerContainer(10);
    BaseTransaction *tx1 = new BaseTransaction(0);

    tx1->start();
    //debuglog(LogLevel::INFO, "tx %ld start", tx1->getTxID());
    try{
        {
            tx1->addContainer(tmInt);
            tmInt->add(10);
            tx1->addContainer(tmInt);
            tmInt->add(100);
            tx1->addContainer(tmInt);
            tmInt->add(1000);
            throw TXAbortException("Testing abort");
        }
    }catch (TXAbortException &e){
        debuglog(LogLevel::WARN, "Tx %ld aborted successfully", tx1->getTxID());
        tx1->abort();
        delete tx1;
        return 0;
    }
    tx1->commit();
    debuglog(LogLevel::INFO, "tx %ld commit", tx1->getTxID());
    delete tx1;

    std::cout<<"Int's val is "<<tmInt->baseNum<<std::endl;

    return 0;
}