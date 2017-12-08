//
// Created by zhangan on 17-11-26.
//

#include <thread>
#include "Log.h"
#include "../Transaction/lock/Abstract.h"
#include "../Transaction/Transaction.h"

AbstractLock::LockType stdOutmutex;//lock the cout so it dont go intersected

void debuglog(LogLevel logLevel, char const * format, ...){
    char msg[512];
    va_list args;
    //#ifdef DEBUGMODE
    va_start(args, format);
    vsprintf(msg, format, args);
    va_end(args);

    stdOutmutex.lock();
    if(logLevel == LogLevel::ERROR){
        std::cout<<"Log [ERROR] "<<"TX["<<BaseTransaction::currentTXID<<"] :"<<msg<<std::endl;
    }else if(logLevel == LogLevel::WARN){
        std::cout<<"Log[WARN]: "<<"TX["<<BaseTransaction::currentTXID<<"] :"<<msg<<std::endl;
    }else if(logLevel == LogLevel::INFO){
        std::cout<<"Log[INFO]: "<<"TX["<<BaseTransaction::currentTXID<<"] :"<<msg<<std::endl;
    }
    stdOutmutex.unlock();
    //#endif
}