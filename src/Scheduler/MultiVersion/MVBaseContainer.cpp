//
// Created by zhangan on 17-12-6.
//

#include "MVBaseContainer.h"

AbstractMVBaseContainer::AbstractMVBaseContainer(char *name) : BaseContainer(name) {}

template <typename valType>
MVBaseContainer<valType>::MVBaseContainer(char *name) : AbstractMVBaseContainer(name) {}

template <typename valType>
void MVBaseContainer<valType>::constructWriteOnDataItem(long txID, DataItemLocator dataItem) {
    MVBaseContainer::DataItem* dataHandle = getOrCreateDataItem(dataItem.key);
    if(!dataHandle->waitListHeader){
        //construct the header if not exist
        dataHandle->waitListHeader = new WaitListItem(txID, &dataHandle->value, dataItem.optype, WaitListItem::OWNERSHIP::OWNER);
    }else{
        //find the tail and append
        WaitListItem* tail = dataHandle->waitListHeader;
        while(tail->next){
            tail = tail->next;
        }
        dataHandle->waitListHeader = new WaitListItem(txID, &tail->NewValue, dataItem.optype, WaitListItem::OWNERSHIP::WAITER);
    }
}

