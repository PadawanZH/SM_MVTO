//
// Created by zhangan on 17-11-17.
//

#include "Transaction.h"
#include <cassert>
#include "../Util/Log.h"

thread_local BaseTransaction::LogType BaseTransaction::undoLog;

thread_local long BaseTransaction::currentTXID = -1;

long BaseTransaction::getTxID () const {
    return txID;
}

BaseTransaction::BaseTransaction (long id){
    txID = id;
    currentTXID = txID;
    this->state = TxState::ACTIVE;
    if(!undoLog.empty()){
        undoLog.clear();
    }
    if(containers){
        containers->clear();
    }else{
        containers = new std::set<BaseContainer*>();
    }

}

BaseTransaction::~BaseTransaction () {
    containers->clear();
    delete containers;
}

BaseTransaction::TxState BaseTransaction::getState () {
    return this->state;
}

void BaseTransaction::start () {
    if(containers->empty()){
        if(!undoLog.empty()){
            undoLog.clear();
        }
    }else{
        debuglog(LogLevel::WARN, "There is a partially transaction found, auto abort it and restart");
        abort();//abort() will clear the undoLog
    }
}

void BaseTransaction::abort () {
    if(!undoLog.empty()){
        for(auto op = undoLog.rbegin(); op != undoLog.rend(); op++){
            //invoke callbacks in reverse order
            assert((*op)());
        }
        undoLog.clear();
    }

    //release every key hold by this tx
    if(!containers->empty()){
        debuglog(LogLevel::INFO,"start to release lock in container");
        for(auto containerPtr = containers->begin(); containerPtr != containers->end(); containerPtr++){
            (*containerPtr)->releaseLocks();
        }
        containers->clear();
    }

}

void BaseTransaction::commit () {
    if(!undoLog.empty()){
        undoLog.clear();
    }
    for(auto containerPtr = containers->begin(); containerPtr != containers->end(); containerPtr++){
        (*containerPtr)->releaseLocks();
    }
}

void BaseTransaction::addContainer (BaseContainer *container) {
    auto ret = containers->insert(container);
    if(ret.second){//container insert is successful
        debuglog(LogLevel::INFO, "Container %s is add to Transaction %ld", container->name, txID);
    }
}