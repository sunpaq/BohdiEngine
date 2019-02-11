//
//  MCHashTable.c
//  monkc3
//
//  Created by 孙御礼 on 11/17/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#include "MCHashTable.h"

//copy form << The C Programming language >>
//BKDR Hash Function
MCHash MCHashTable_hash(const char* s) {
    register MCHash hashval = 0;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    //keep the value positive
    return (hashval & 0x7fffffff);
}

static unsigned probe(MCHash nkey, unsigned slots, unsigned times) {
    //printf("probe %d times\n", times);
    return (nkey + times * times) % slots;
}

/*
 method table is initially set min one
 class  table is initially set max one
 
 only the max size table use chain to slove collision
 other tables will expand(rehash) until they reach the max
 */

static unsigned mc_hashtable_sizes[MCHashTableLevelCount] = {
    MIN_HASHTABLE_SIZE,
    3101,
    13001,
    31001,
    130001
};

unsigned get_tablesize(MCHashTableLevel level)
{
    if (level > MCHashTableLevelMax) {
        return mc_hashtable_sizes[MCHashTableLevelMax];
    }
    return mc_hashtable_sizes[level];
}

/* *
 * Configure hash table size:
 * have 5 levels of size
 * and it can auto expand to next level when some key collisioned
 *
 * Example of memory usage:
 * max memory useage for one class  table is: 4Byte x 10000 = 40KB
 * max memory useage for one method table is: 4Byte x 10000 = 40KB
 * max total memory useage is 4Byte x 10000 x 10000 = 400000KB = 400MB
 * */
/*
typedef enum {
    MCHashTableLevel1 = 0,
    MCHashTableLevel2,
    MCHashTableLevel3,
    MCHashTableLevel4,
    MCHashTableLevelMax,
    MCHashTableLevelCount
} MCHashTableLevel;

typedef MCUInt MCHashTableSize;
typedef MCUInt MCHashTableIndex;
*/

static void copykey(char* des, const char* key, size_t maxlen) {
    size_t len = strlen(key);
    if (len > maxlen) {
        len = maxlen;
    }
    strncpy(des, key, len);
    des[len] = '\0';
}

fun(release, void)) as(MCHashItem)
    if (it->doesAutoReleaseObject && it->value.mcobject) {
        it->value.mcobject->release(it->value.mcobject);
    }
}

constructor(MCHashItem), const char* key, mc_generic value) {
    MCObject(any);
    as(MCHashItem)
        it->next = null;
        it->value = value;
        it->tombstone = false;
        it->doesAutoReleaseObject = false;
        it->hash = MCHashTable_hash(key);
        copykey(it->key, key, MAX_KEY_CHARS);
        funadd(release);
    }
    return any;
}

//MCHashTable

fun(getItem, struct MCHashItem*), const char* key) as(MCHashTable)
    MCHash hashval = MCHashTable_hash(key);
    unsigned tsize = get_tablesize(0);

    unsigned t = 0;
    while(t < tsize) {
        unsigned i = probe(hashval, tsize, t++);
        //slot empty
        if (!it->items[i]) {
            continue;
        }
        //key not fit
        if (strcmp(key, it->items[i]->key) != 0) {
            continue;
        }
        //pass all the check
        return it->items[i];
    }

    //not find
    return null;
}

fun(putItem, struct MCHashItem*), struct MCHashItem* item) as(MCHashTable)
    MCHash hashval = item->hash;
    unsigned tsize = get_tablesize(0);

    unsigned t = 0;
    while(t < tsize) {
        unsigned i = probe(hashval, tsize, t++);
        //slot empty
        if (!it->items[i]) {
            it->items[i] = item;
            //printf("add item[%d] = %s\n", i, item->key);
            return null;
        } else {
            struct MCHashItem* old = it->items[i];
            //slot deleted
            if (old->tombstone) {
                free(old);
                it->items[i] = item;
                printf("add item[%d] = %s\n", i, item->key);
                return null;
            }
            //already have
            if (strcmp(item->key, old->key) == 0) {
                printf("skip add item[%d] = %s\n", i, item->key);
                return old;
            }
        }
    }

    //table is full
    return null;
}



fun(put, mc_generic), const char* key, mc_generic value)
{
    val item = MCHashItem(alloc(MCHashItem), key, value);
    struct MCHashItem* old = putItem(any, item);
    if (old) {
        return old->value;
    }
    return gen_p(null);
}

fun(get, mc_generic), const char* key)
{
    struct MCHashItem* item = getItem(any, key);
    if (item) {
        return item->value;
    }
    return gen_p(null);
}

constructor(MCHashTable))
{
    as(MCHashTable)
        it->lock = 0;
        it->cache_count = 0;
        it->count = MIN_HASHTABLE_SIZE;
        //set all the slot to nil
        int i;
        for(i=0; i<it->count; i++) {
            it->items[i] = null;
        }
        //add functions
        funadd(putItem);
        funadd(getItem);
        funadd(put);
        funadd(get);
    }
    return any;
}





