#include <iostream>
#include "src/Testing/testMVScheduler.h"
#include "src/Util/ContainerInstantiate.h"


using namespace std;
int main() {
    TestMVContainer_StringMap* psMap = (TestMVContainer_StringMap*)ContainerUtil::InstanceOf("StringMap");
    TestMVContainer_IntMap* piMap = (TestMVContainer_IntMap*)ContainerUtil::InstanceOf("IntMap");

    psMap->print();
    piMap->print();
    return 0;
}