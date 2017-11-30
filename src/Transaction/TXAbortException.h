//
// Created by zhangan on 17-11-17.
//

#ifndef TRANSACTIONALSTOSCHEDULER_TXABORTEXCEPTION_H
#define TRANSACTIONALSTOSCHEDULER_TXABORTEXCEPTION_H

#include <string>
#include <stdexcept>

class TXAbortException : public std::runtime_error {
public:
    explicit TXAbortException(std::string msg);
};


#endif //TRANSACTIONALSTOSCHEDULER_TXABORTEXCEPTION_H
