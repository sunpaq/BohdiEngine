//
//  MCSort.c
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/5.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSort.h"

fun(bye, void)) as(MCSort)
    if (it->array && it->length > 0) {
        free(it->array);
    }
}

fun(swap, void), size_t a, size_t b) as(MCSort)
    if (a < b) {
        mc_generic t = it->array[a];
        it->array[a] = it->array[b];
        it->array[b] = t;
    }
}

fun(insertionSort, void)) {
    
}

fun(quicksort, void), const size_t l, const size_t r) as(MCSort)
    if (l >= r || l > it->length || r > it->length) {
        //debug_log("quicksort exit l=%ld r=%ld\n", l, r);
        return;
    }
    mc_generic pivot = it->array[l];
    size_t cur=l, idx;
    for (idx=l+1; idx<=r; idx++) {
        if (MCGenericCompare(it->array[idx], pivot) < 0)
            swap(it, ++cur, idx);
    }
    
    swap(it, l, cur);
    quicksort(it, l, cur-1);
    quicksort(it, cur+1, r);
}

fun(quickSort, void)) as(MCSort)
    quicksort(it, 0, it->length-1);
}

fun(printArray, void)) as(MCSort)
    size_t i;
    for (i=0; i<it->length; i++) {
        printf("element of array[%ld]=%.2f\n", i, it->array[i].f);
    }
}

constructor(MCSort), mc_generic* array, size_t length) {
    MCObject(any);
    as(MCSort)
        it->array = (mc_generic*)malloc(sizeof(mc_generic) * length);
        size_t i;
        for (i=0; i<length; i++) {
            it->array[i] = array[i];
        }
        it->length = length;
    }
    dynamic(MCSort)
        funbind(bye);
        funbind(insertionSort);
        funbind(quickSort);
        funbind(printArray);
    }
    return any;
}



