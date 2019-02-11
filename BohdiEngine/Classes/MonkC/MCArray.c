//
// Created by 孙御礼 on 2018-12-04.
//

#include "MCArray.h"

static void expand_array(struct MCArray* obj)
{
    if(obj->maxcount == 0){
        obj->maxcount = 100;
        MCArray(obj, obj->maxcount);
    }else{
        obj->maxcount = (obj->maxcount) * 2;//double
        mc_generic* newbuff = (mc_generic*)malloc(sizeof(mc_generic) * obj->maxcount);
        memcpy(newbuff, obj->buff, obj->maxcount * sizeof(mc_generic));
        free(obj->buff);
        obj->buff = newbuff;
        //printf("[MCArray] expand to (%d)\n", obj->size);
    }
}

static void insert_item(struct MCArray* obj, mc_generic item)
{
    obj->buff[obj->indexLast++] = item;
    obj->count++;
}

static void delete_item(struct MCArray* obj, size_t index)
{
    if(index > obj->indexLast) return;
    obj->buff[index] = (mc_generic){.p=null};
    if(index==obj->indexLast)
        obj->indexLast--;
    obj->count--;
}

fun(addItem, void), mc_generic item) as(MCArray)
    if (it->indexLast >= it->maxcount) {
        expand_array(it);
    }
    insert_item(it, item);
}

fun(addTo, void), mc_generic item, size_t index) as(MCArray)
    if(index >= it->maxcount){
        expand_array(it);
    }
    insert_item(it, item);
}

fun(removeLast, void)) as(MCArray)
    delete_item(it, it->indexLast);
}

fun(removeItem, void), mc_generic* item) as(MCArray)
    size_t i;
    for (i=0; i<it->maxcount; i++) {
        if (&it->buff[i] == item) {
            delete_item(it, i);
        }
    }
}

fun(removeAt, void), size_t index) as(MCArray)
    delete_item(it, index);
}

fun(clear, void)) as(MCArray)
    it->release(it);
    it->buff = (mc_generic*)malloc(sizeof(mc_generic) * 100);
}

fun(itemAt, mc_generic*), size_t index) as(MCArray)
    return &it->buff[index];
}

fun(printAll, void), const char* delimiter) as(MCArray)
    size_t i;
    for (i=0; i<it->count; i++) {
        printf("%.2f%s", it->buff[i].f, delimiter);
    }
    printf("\n");
}

fun(release, void)) {
    as(MCObject)
        it->release(it);
    }
    as(MCArray)
        if (it->buff) {
            free(it->buff);
        }
    }
}

constructor(MCArray), size_t maxcount) {
    MCObject(any);
    as(MCArray)
        it->maxcount = maxcount;
        it->count = 0;
        it->indexLast = 0;
        it->buff = (mc_generic*)malloc(sizeof(mc_generic) * 100);
        funadd(release);
    }
    dynamic(MCArray)
        funbind(addItem);
        funbind(addTo);
        funbind(removeLast);
        funbind(removeItem);
        funbind(removeAt);
        funbind(clear);
        funbind(itemAt);
        funbind(printAll);
    }
    return any;
}
