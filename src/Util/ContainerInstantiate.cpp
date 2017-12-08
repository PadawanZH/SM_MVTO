//
// Created by zhangan on 17-12-8.
//

#include "ContainerInstantiate.h"

std::map<std::string, std::function<AbstractMVBaseContainer*()>> ContainerUtil::MVInstanceMap = {
        {std::string("IntMap"), [](){
            AbstractMVBaseContainer *p = new TestMVContainer_IntMap();
            return p;
        }},
        {std::string("StringMap"), [](){
            AbstractMVBaseContainer *p = new TestMVContainer_StringMap();
            return p;
        }}
};