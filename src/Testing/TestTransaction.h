//
// Created by zhangan on 17-11-26.
//

#ifndef TRANSACTIONALSTOSCHEDULER_TESTTRANSACTION_H
#define TRANSACTIONALSTOSCHEDULER_TESTTRANSACTION_H

#include "../Boosting/BaseContainer.h"
#include "../Util/Log.h"
#include "../Transaction/TXAbortException.h"
#include "../Transaction/Transaction.h"

class IntegerContainer : public BaseContainer{
public:
    explicit IntegerContainer (int initVal);
    ~IntegerContainer ();

    int baseNum;

    int add(int x);

    void releaseLocks() override;
};

int testMain();

#endif //TRANSACTIONALSTOSCHEDULER_TESTTRANSACTION_H
