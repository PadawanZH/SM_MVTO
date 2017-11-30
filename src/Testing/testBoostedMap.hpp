//
// Created by zhangan on 17-11-30.
//

#ifndef TRANSACTIONALSTOSCHEDULER_TESTBOOSTEDMAP_HPP
#define TRANSACTIONALSTOSCHEDULER_TESTBOOSTEDMAP_HPP

#include "../Boosting/Boosted_map.h"
#include "../Transaction/Transaction.h"
#include "../Transaction/TXAbortException.h"
#include "../Util/Log.h"
#include <thread>

using namespace std;

void thread_insert(Boosted_map *pMap, long txID,  long key, void* data, bool fAbort){
    BaseTransaction *tx1= new BaseTransaction(txID);
    debuglog(LogLevel::INFO, "Tx %ld start",tx1->getTxID());
    tx1->start();
    try{
        {
            tx1->addContainer(pMap);
            pMap->tm_insert(key, data);
            if(fAbort){
                throw TXAbortException("Testing abort");
            }
        }
    }catch (TXAbortException &e){
        debuglog(LogLevel::WARN, "Tx %ld aborted successfully",tx1->getTxID());
        tx1->abort();
        delete tx1;
        return;
    }
    debuglog(LogLevel::INFO, "tx %ld commit", tx1->getTxID());
    tx1->commit();
    delete tx1;
}

void thread_remove(Boosted_map *pMap, long txID,  long key, bool fAbort){
    BaseTransaction *tx1= new BaseTransaction(txID);
    debuglog(LogLevel::INFO, "Tx %ld start",tx1->getTxID());
    tx1->start();
    try{
        {
            tx1->addContainer(pMap);
            auto ret = pMap->tm_remove(key);
            debuglog(LogLevel::INFO, "deleted data is %s", (char*)ret.second.second);
            if(fAbort){
                throw TXAbortException("Testing abort");
            }
        }
    }catch (TXAbortException &e){
        debuglog(LogLevel::WARN, "Tx %ld aborted successfully",tx1->getTxID());
        tx1->abort();
        delete tx1;
        return;
    }
    debuglog(LogLevel::INFO, "tx %ld commit", tx1->getTxID());
    tx1->commit();
    delete tx1;
}

void thread_contain(Boosted_map *pMap, long txID,  long key, bool fAbort){
    BaseTransaction *tx1= new BaseTransaction(txID);
    debuglog(LogLevel::INFO, "Tx %ld start",tx1->getTxID());
    tx1->start();
    try{
        {
            tx1->addContainer(pMap);
            pMap->tm_contains(key);
            if(fAbort){
                throw TXAbortException("Testing abort");
            }
        }
    }catch (TXAbortException &e){
        debuglog(LogLevel::WARN, "Tx %ld aborted successfully",tx1->getTxID());
        tx1->abort();
        delete tx1;
        return;
    }
    debuglog(LogLevel::INFO, "tx %ld commit", tx1->getTxID());
    tx1->commit();
    delete tx1;
}

void thread_find(Boosted_map *pMap, long txID,  long key, bool fAbort){
    BaseTransaction *tx1= new BaseTransaction(txID);
    debuglog(LogLevel::INFO, "Tx %ld start",tx1->getTxID());
    tx1->start();
    try{
        {
            tx1->addContainer(pMap);

            char* data = (char*)pMap->tm_find(key);
            if(data){
                debuglog(LogLevel::INFO, "In map, key [%ld, has data %s]", key, data);
            }
            if(fAbort){
                throw TXAbortException("Testing abort");
            }
        }
    }catch (TXAbortException &e){
        debuglog(LogLevel::WARN, "Tx %ld aborted successfully",tx1->getTxID());
        tx1->abort();
        delete tx1;
        return;
    }
    debuglog(LogLevel::INFO, "tx %ld commit", tx1->getTxID());
    tx1->commit();
    delete tx1;
}


class TestBoostMap{
public:
    Boosted_map *pMap;

    TestBoostMap(){
        pMap = new Boosted_map();
    }

    ~TestBoostMap(){
        delete pMap;
    }

    void testUndoAction(){
        char *data1 = new char[5];for(int i=0;i<5;i++){data1[i] = '1';}
        char *data2 = new char[5];for(int i=0;i<5;i++){data2[i] = '2';}
        char *data3 = new char[5];for(int i=0;i<5;i++){data3[i] = '3';}
        char *data4 = new char[5];for(int i=0;i<5;i++){data4[i] = '4';}
        char *data5 = new char[5];for(int i=0;i<5;i++){data5[i] = '5';}
        char *data6 = new char[5];for(int i=0;i<5;i++){data6[i] = '6';}
        //pre insert
        pMap->nonTMInsert(1, (void*)data1);
        pMap->nonTMInsert(2, (void*)data2);
        pMap->nonTMInsert(3, (void*)data3);

        cout<<"After pre insert, map's size is "<<pMap->hash_map.size()<<"\n----------------------------\n";
        pMap->print();
        cout<<"\n-----------------------\n";
        cout<<"1.  test the normal function of map: \n----------------------------\n";
        thread thread1(thread_insert,   pMap, 1, 4, (void*)data4, false);
        thread thread2(thread_insert,   pMap, 2, 4, (void*)data5, false);
        thread thread3(thread_remove,   pMap, 3, 3, false);
        thread thread4(thread_contain,  pMap, 4, 1, false);
        thread thread5(thread_find,     pMap, 5, 3, false);
        thread1.join();
        thread2.join();
        thread3.join();
        thread4.join();
        thread5.join();

        cout<<"\n----------------------------\n";
        cout<<"After Exp, map's size is "<<pMap->hash_map.size()<<"\n----------------------------\n";
        pMap->print();
        cout<<endl<<endl;

        cout<<"2.  test transaction charactersitic through mutiple aborted insertion and one find: ";
        cout<<"\n----------------------------\n";
        thread thread6(thread_insert,       pMap, 6,    5,  (void*)data6, true);
        thread thread7(thread_insert,       pMap, 7,    5,  (void*)data6, true);
        thread thread11(thread_contain,     pMap, 11,   5,  false);
        thread thread8(thread_insert,       pMap, 8,    5,  (void*)data6, true);
        thread thread9(thread_insert,       pMap, 9,    5,  (void*)data6, true);
        thread thread10(thread_contain,     pMap, 10,   5,  false);

        thread6.join();
        thread7.join();
        thread8.join();
        thread9.join();
        thread10.join();
        thread11.join();
        cout<<"\n----------------------------\n";
        cout<<"After Exp, map's size is "<<pMap->hash_map.size()<<"\n----------------------------\n";
        pMap->print();
        cout<<endl<<endl;


        delete data1;
        delete data2;
        delete data3;
        delete data4;
        delete data5;
        delete data6;
    }


    void testMapMain(){
        testUndoAction();
    }
};



#endif //TRANSACTIONALSTOSCHEDULER_TESTBOOSTEDMAP_HPP
