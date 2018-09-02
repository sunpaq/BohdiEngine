
//
//  MCMap.c
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#include "MCMap.h"

compute(MCHashTableSize, itemsCount)
{
    as(MCMap);
    MCHashTableSize size = get_tablesize(obj->table->level);
    return size;
}

oninit(MCMap)
{
    if (init(MCObject)) {
        obj->table = new_table(MCHashTableLevel1);
        obj->itemsCount = itemsCount;
        return obj;
    } else {
        return null;
    }
}

fun(MCMap, void, bye, voida)
{
    MCHashTableSize count = cpt(itemsCount);
    for (MCHashTableIndex i=0; i<count; i++) {
        mc_hashitem* item = obj->table->items[i];
        if (item) {
            MCGeneric v = item->value;
            //if there have a MCObject in item, we release it
            if (v.mcobject) {
                release(v.mcobject);
            }
        }
    }
}

fun(MCMap, void, setValueForKey, MCGeneric value, const char* key)
{
    mc_hashitem* item = new_item(key, value, hash(key));
    if (item) {
        set_item(&obj->table, item, false, "MCMap");
    }
}

fun(MCMap, void, getValueForKey, MCGeneric* result, const char* key)
{
    mc_hashitem* item = get_item_bykey(obj->table, key);
    if (item) {
        (*result) = item->value;
        return;
    }
    (*result).mcvoidptr = null;
}

fun(MCMap, void, getValueByIndex, MCGeneric* result, MCHashTableIndex index)
{
    MCHashTableSize count = cpt(itemsCount);
    if (index < count) {
        mc_hashitem* item = obj->table->items[index];
        if (item) {
            MCGeneric v = item->value;
            *result = v;
        }
    }
}

onload(MCMap)
{
    if (load(MCObject)) {
        bid(MCMap, void, bye, voida);
        bid(MCMap, void, setValueForKey, MCGeneric value, const char* key);
        bid(MCMap, void, getValueForKey, MCGeneric* result, const char* key);
        bid(MCMap, void, getValueByIndex, MCGeneric* result, MCHashTableIndex index);
        return cla;
    } else {
        return null;
    }
}
