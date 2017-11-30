//
// Created by zhangan on 17-11-17.
//

#ifndef TRANSACTIONALSTOSCHEDULER_LOG_H
#define TRANSACTIONALSTOSCHEDULER_LOG_H


#include <iostream>
#include <cstdarg>

enum LogLevel {WARN, ERROR, INFO};

void debuglog(LogLevel logLevel, char const *format, ...);
/*
class Log {

public:
    enum LogLevel {WARN, ERROR, INFO};

    std::string logFileDir;
    std::string logFileName;

    void log(LogLevel logLevel, std::string msg){
        std::cout<<"Log: "<<msg<<endl;
//#ifdef DEBUGMODE
        std::string content;
        if(logLevel == LogLevel::ERROR){

        }else if(logLevel == LogLevel::WARN){

        }else if(logLevel == LogLevel::INFO){

        }else{
            return;
        }
//#endif
    }


private:
    Log (const std::string &logFileDir, const std::string &logFileName) : logFileDir(logFileDir),
                                                                          logFileName(logFileName) {
        this->logFileDir = logFileDir;
        this->logFileName = logFileName;
    }

public:
    static Log* getLog(){
        static Log* log = NULL;

        if(log == NULL){
            std::string timePostfix;
            time_t timep;
            time(&timep);
            timePostfix = asctime(gmtime(&timep));

            log = new Log("./LOG","running_Log_" + timePostfix);
        }else{
            return log;
        }
    }

    ~Log (){delete log;}
};

*/
#endif //TRANSACTIONALSTOSCHEDULER_LOG_H
