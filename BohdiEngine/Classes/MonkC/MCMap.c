//
//  MCMap.c
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#include "MCMap.h"

//class(MCMap, MCObject);
oninit(MCMap)
{
    if (init(MCObject)) {
        obj->table = new_table(MCHashTableLevel1);
        return obj;
    } else {
        return null;
    }
}

method(MCMap, void, setValueForKey, void* value, const char* key)
{
    mc_hashitem* item = new_item(key, MCGenericVp(value), hash_content(key));
    set_item(&obj->table, item, false, key);
}

method(MCMap, void*, getValueForKey, const char* key)
{
    mc_hashitem* item = get_item_bykey(obj->table, key);
    return item->value.mcvoidptr;
}

onload(MCMap)
{
    if (load(MCObject)) {
        binding(MCMap, void, setValueForKey, void* value, const char* key);
        binding(MCMap, void*, getValueForKey, const char* key);
        return cla;
    } else {
        return null;
    }
}
