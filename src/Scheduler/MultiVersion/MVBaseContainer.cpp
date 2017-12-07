//
// Created by zhangan on 17-12-6.
//

#include "MVBaseContainer.h"

MVBaseContainer::MVBaseContainer(char *name) : BaseContainer(name) {

}

void MVBaseContainer::constructWriteOnDataItem(long txID, DataItemLocator dataItem) {
    DataItem* dataHandle = getOrCreateDataItem(dataItem.key);
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