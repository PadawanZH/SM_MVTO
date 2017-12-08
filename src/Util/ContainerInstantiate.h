//
// Created by zhangan on 17-12-6.
//

#ifndef TRANSACTIONALSTOSCHEDULER_CONTAINERINSTANTIATE_H
#define TRANSACTIONALSTOSCHEDULER_CONTAINERINSTANTIATE_H

#include <map>
#include "../Boosting/BaseContainer.h"
#include "../Scheduler/MultiVersion/MVBaseContainer.h"

#include "../Testing/testMVScheduler.h"

class ContainerUtil{
public:
    static std::map<std::string, std::function<AbstractMVBaseContainer*()>> MVInstanceMap;

/// 需要查证
/// \param typeName
/// \return
    static AbstractMVBaseContainer* InstanceOf(std::string typeName){
        return &(*(MVInstanceMap.find(typeName)->second)());
    }
};


#endif //TRANSACTIONALSTOSCHEDULER_CONTAINERINSTANTIATE_H
