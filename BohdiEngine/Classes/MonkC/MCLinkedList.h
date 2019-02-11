//
//  MCLinkedList.h
//  Sapindus
//
//  Created by Sun YuLi on 16/6/1.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCLinkedList_h
#define MCLinkedList_h

//#include <stdio.h>
#include "MCObject.h"
#include "MCType.h"
//Item

structure(MCItem, MCObject)
    mc_generic value;
    obj object;
    struct MCItem* prevItem;
    struct MCItem* nextItem;

    fundef(linkNextItem, void), struct MCItem* next);
    fundef(linkPrevItem, void), struct MCItem* prev);
    fundef(releaseContent, void));
    fundef(release, void));
};

constructor(MCItem), obj content);

alias(MCItem);

struct MCItem* MCItem_itemWithObject(obj content);

//List

structure(MCLinkedList, MCObject)
    MCItem_t* headItem;
    MCItem_t* tailItem;

    bool countChanged;
    unsigned countCache;

    fundef(count, unsigned));
    fundef(cycle, MCItem_t*));
    fundef(addItem, void), MCItem_t* item);
    fundef(delItem, void), MCItem_t* item);
    fundef(addAndRetainObject, void), obj object);
    fundef(pushItem, void), MCItem_t* item);
    fundef(popItem, MCItem_t*));
    fundef(itemAtIndex, MCItem_t*), int index);
    fundef(addItemAtIndex, void), int index, MCItem_t* item);
    fundef(replaceItemAtIndex, void), int index, MCItem_t* withitem);
    fundef(insertAfterItem, void), MCItem_t* anchor, MCItem_t* item);
    fundef(insertBeforeItem, void), MCItem_t* anchor, MCItem_t* item);
    fundef(connectList, struct MCLinkedList*), struct MCLinkedList* otherlist);
    fundef(release, void));
};

constructor(MCLinkedList));

alias(MCLinkedList);

#define MCLinkedListForEach(list, code) \
{struct MCItem* item = list->headItem;         \
while (item != null) {                  \
    code                                \
    item = item->nextItem;              \
}}

#endif /* MCLinkedList_h */
