#include <string.h>
#include "MCArray.h"

static void expand_array(MCArray* obj)
{
    if(obj->maxcount == 0){
        obj->maxcount = 10;
        MCArray_initWithMaxCount(obj, obj->maxcount);
    }else{
        obj->maxcount = (obj->maxcount) * 2;//double
        MCGeneric* newbuff = (MCGeneric*)malloc(sizeof(MCGeneric) * obj->maxcount);
        memcpy(newbuff, obj->buff, obj->maxcount * sizeof(MCGeneric));
        free(obj->buff);
        obj->buff = newbuff;
        //printf("[MCArray] expand to (%d)\n", obj->size);
    }
}

static void insert_item(MCArray* obj, MCGeneric item)
{
    obj->buff[obj->indexLast++] = item;
    obj->count++;
}

static void delete_item(MCArray* obj, size_t index)
{
    if(index > obj->indexLast) return;
    obj->buff[index] = (MCGeneric){.mcvoidptr=null};
    if(index==obj->indexLast)
        obj->indexLast--;
    obj->count--;
}

oninit(MCArray)
{
    if (init(MCObject)) {
        obj->count = 0;
        obj->maxcount = 0;
        obj->indexLast = 0;
        obj->buff = null;
        return obj;
    }else{
        return null;
    }
}

fun(MCArray, MCArray*, initWithMaxCount, size_t max)
{
	obj->count = 0;
	obj->maxcount = max;
	obj->indexLast = 0;
    obj->buff = (MCGeneric*)malloc(sizeof(MCGeneric) * max);
	return obj;
}

fun(MCArray, void, bye, voida)
{
    if (obj->buff != null) {
        free(obj->buff);
    }
    superbye(MCObject);
}

fun(MCArray, MCArray*, clear, voida)
{
    free(obj->buff);
    return MCArray_initWithMaxCount(obj, 10);
}

fun(MCArray, MCArray*, addItem, MCGeneric item)
{
	if(obj->indexLast >= obj->maxcount){
		expand_array(obj);
	}
	insert_item(obj, item);
	return obj;
}

fun(MCArray, MCArray*, addItemToIndex, MCGeneric item, size_t index)
{
	if(index >= obj->maxcount){
		expand_array(obj);
	}
	insert_item(obj, item);
	return obj;
}

fun(MCArray, MCArray*, removeLastItem, voida)
{
	delete_item(obj, obj->indexLast);
	return obj;
}

fun(MCArray, MCArray*, removeItem, MCGeneric* item)
{
    for (int i=0; i<obj->maxcount; i++) {
        if (&obj->buff[i] == item) {
            delete_item(obj, i);
        }
    }
	return obj;
}

fun(MCArray, MCArray*, removeItemByIndex, size_t index)
{
	delete_item(obj, index);
	return obj;
}

fun(MCArray, MCGeneric*, getItemByIndex, size_t index)
{
	return &obj->buff[index];
}

fun(MCArray, void, printAll, const char* delimiter)
{
    for (int i=0; i<obj->count; i++) {
        printf("%.2f%s", obj->buff[i].mcfloat, delimiter);
    }
    printf("\n");
}

onload(MCArray)
{
    if (load(MCObject)) {
        bid(MCArray, void, bye, voida);
        bid(MCArray, MCArray*, initWithMaxCount, size_t max);
        bid(MCArray, MCArray*, addItem, MCGeneric item);
        bid(MCArray, MCArray*, addItemToIndex, MCGeneric item, size_t index);
        bid(MCArray, MCArray*, removeLastItem, voida);
        bid(MCArray, MCArray*, removeItem, MCGeneric* item);
        bid(MCArray, MCArray*, removeItemByIndex, size_t index);
        bid(MCArray, MCArray*, clear, voida);
        bid(MCArray, MCGeneric*, getItemByIndex, size_t index);
        bid(MCArray, void, printAll, const char* delimiter);
        return cla;
    }else{
        return null;
    }
}
