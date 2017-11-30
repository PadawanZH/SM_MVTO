//
// Created by zhangan on 17-11-17.
//

#ifndef TRANSACTIONALSTOSCHEDULER_TRANSACTION_H
#define TRANSACTIONALSTOSCHEDULER_TRANSACTION_H

#include <string>
#include <vector>
#include <functional>
#include <set>
#include "../Boosting/BaseContainer.h"

class BaseTransaction {
public:
    enum TxState{ACTIVE, COMMITTED, READY, ABORT};
    typedef std::vector<std::function<bool()>>  LogType;
public:
    explicit BaseTransaction (long id);

    // thread_local不要声明指针，会导致static initialization的问题，
    // 可能要用“thread_local std::unique_ptr<SomeClass> someClass(new SomeClass(params));”的形式来声明指针
    // uniqu_ptr会在thread退出时释放内存
    static thread_local LogType undoLog;
    static thread_local long txID;

private:
    TxState state;
    std::set<BaseContainer*> *containers;
public:
    ~BaseTransaction ();
    long getTxID () const;
    void start();
    void abort();
    void commit();
    void addContainer(BaseContainer* container);
    TxState getState();
};

#endif //TRANSACTIONALSTOSCHEDULER_TRANSACTION_H
