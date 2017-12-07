//
// Created by zhangan on 17-12-6.
//

#ifndef TRANSACTIONALSTOSCHEDULER_CONTAINERINSTANTIATE_H
#define TRANSACTIONALSTOSCHEDULER_CONTAINERINSTANTIATE_H

#include <map>
#include "../Boosting/BaseContainer.h"
#include "../Scheduler/MultiVersion/MVBaseContainer.h"

static std::map<std::string, std::function<MVBaseContainer*(std::string name)>> MVInstanceMap;

/// 需要查证
/// \param typeName
/// \return
MVBaseContainer* InstanceOfMVContainer(std::string typeName){
    return &(*(MVInstanceMap.find(typeName)->second)(typeName));
}

#endif //TRANSACTIONALSTOSCHEDULER_CONTAINERINSTANTIATE_H
