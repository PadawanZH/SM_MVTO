//
// Created by zhangan on 17-11-17.
//

#include "TXAbortException.h"

TXAbortException::TXAbortException (std::string msg) : runtime_error(msg){}
