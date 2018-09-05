//
//  MCLinkedList.h
//  Sapindus
//
//  Created by Sun YuLi on 16/6/1.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCLinkedList_h
#define MCLinkedList_h

#include <stdio.h>
#include "MCContext.h"

//Item

class(MCItem, MCObject,
      MCGeneric value;
      MCObject* object;
      struct MCItemStruct* prevItem;
      struct MCItemStruct* nextItem);

fun(MCItem, void, bye, voida);
fun(MCItem, MCItem*, initWithContentObject, MCObject* content);
fun(MCItem, void, linkNextItem, MCItem* next);
fun(MCItem, void, linkPrevItem, MCItem* prev);

util(MCItem, MCItem*, itemWithObject, MCObject* content);

//List

class(MCLinkedList, MCObject,
      MCItem* headItem;
      MCItem* tailItem;
      
      MCBool countChanged;
      unsigned countCache;
      computing(unsigned, count);
      computing(MCItem*, cycle));

fun(MCLinkedList, void, bye, voida);
fun(MCLinkedList, void, addItem, MCItem* item);
fun(MCLinkedList, void, delItem, MCItem* item);
fun(MCLinkedList, void, addAndRetainObject, MCObject* object);
fun(MCLinkedList, void, pushItem, MCItem* item);
fun(MCLinkedList, MCItem*, popItem, voida);
fun(MCLinkedList, MCItem*, itemAtIndex, int index);
fun(MCLinkedList, void, addItemAtIndex, int index, MCItem* item);
fun(MCLinkedList, void, replaceItemAtIndex, int index, MCItem* withitem);
fun(MCLinkedList, void, insertAfterItem, MCItem* anchor, MCItem* item);
fun(MCLinkedList, void, insertBeforeItem, MCItem* anchor, MCItem* item);
fun(MCLinkedList, MCLinkedList*, connectList, MCLinkedList* otherlist);
fun(MCLinkedList, void, forEach, mc_message callback, void* userdata);

#define MCLinkedListForEach(list, code) \
{MCItem* item = list->headItem;         \
while (item != null) {                  \
    code                                \
    item = item->nextItem;              \
}}

#endif /* MCLinkedList_h */
