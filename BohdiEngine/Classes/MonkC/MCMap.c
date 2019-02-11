#include "MCMap.h"

fun(itemsCount, size_t)) as(MCMap)
    return it->table->count;
}

fun(setValueForKey, void), mc_generic value, const char* key) as(MCMap)
    it->table->put(it->table, key, value);
}

fun(getValueForKey, void), mc_generic* result, const char* key) as(MCMap)
    struct MCHashItem* item = it->table->getItem(it->table, key);
    if (item) {
        (*result) = item->value;
        return;
    }
    (*result).p = null;
}

fun(getValueByIndex, void), mc_generic* result, size_t index) as(MCMap)
    size_t count = it->table->count;
    if (index < count) {
        struct MCHashItem* item = it->table->items[index];
        if (item) {
            mc_generic v = item->value;
            *result = v;
        }
    }
}

fun(release, void)) as(MCMap)
    size_t count = it->table->count;
    size_t i;
    for (i=0; i<count; i++) {
        struct MCHashItem* item = it->table->items[i];
        if (item) {
            mc_generic v = item->value;
            //if there have a MCObject in item, we release it
            if (item->doesAutoReleaseObject && v.mcobject != null) {
                v.mcobject->release(v.mcobject);
            }
        }
    }
}

constructor(MCMap)) {
    MCObject(any);
    as(MCMap)
        it->table = MCHashTable(alloc(MCHashTable));
    }
    dynamic(MCMap)
        funbind(itemsCount);
        funbind(setValueForKey);
        funbind(getValueForKey);
        funbind(getValueByIndex);
        funbind(release);
    }
    return any;
}

