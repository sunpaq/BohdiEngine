//
//  MCHeap.c
//  Sapindus
//
//  Created by YuliSun on 17/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include "MCHeap.h"

static size_t leftChildIndex(size_t i) {
    return i*2;
}

static size_t rightChildIndex(size_t i) {
    return i*2 + 1;
}

static size_t parentIndex(size_t i) {
    return i / 2;
}

static void swapNode(struct MCHeap* heap, size_t i, size_t j) {
    if (i <= heap->count && j <= heap->count) {
        mc_generic temp = heap->values[i];
        heap->values[i] = heap->values[j];
        heap->values[j] = temp;
    }
}

static void shiftup(struct MCHeap* heap, size_t index)
{
    size_t i = index, p;
    while (1) {
        if (i == 1)
            break;
        p = parentIndex(i);
        if (MCGenericCompare(heap->values[p], heap->values[i]) <= 0)
            break;
        swapNode(heap, p, i);
        i = p;
    }
}

static void shiftdown(struct MCHeap* heap, size_t index)
{
    size_t i = index, l, r;
    while (1) {
        l = leftChildIndex(i);
        r = rightChildIndex(i);
        if (MCGenericCompare(heap->values[i], heap->values[l]) > 0) {
            swapNode(heap, i, l);
            i = l;
        }
        else if (MCGenericCompare(heap->values[i], heap->values[r]) > 0) {
            swapNode(heap, i, r);
            i = r;
        }else{
            break;
        }
    }
}

static mc_generic deleteRoot(struct MCHeap* heap)
{
    mc_generic root = heap->values[1];
    heap->values[1] = heap->values[heap->count];
    heap->count--;
    shiftdown(heap, 1);
    return root;
}

//static void printNode(MCHeap* heap, size_t index)
//{
//    int indent = (int)(heap->maxheight-(size_t)log2(index));
//    if (index == 1) {
//        printf("%*s%lf\n", (int)heap->maxheight, "", (float)heap->values[1].float);
//    }
//
//    size_t l = leftChildIndex(index);
//    size_t r = rightChildIndex(index);
//    
//    if (l <= heap->count && r <= heap->count) {
//        if (indent < 0) {
//            indent = 0;
//        }
//        printf("%*s%lf", indent, "", (float)heap->values[l].float);
//        printf("%*s%lf", indent+1, "", (float)heap->values[r].float);
//
//        size_t H = computed(heap, height) - 1;
//        
//        if (r == exp2(H) - 1) {
//            //edge
//            printf("\n");
//        }
//        
//        printNode(heap, l);
//        printNode(heap, r);
//    }
//    
//    //printf("%*s%d %d\n", indent, "", heap->values[L], heap->values[R]);
//}



fun(height, size_t)) as(MCHeap)
    //log2(x) = log10(x) / log10(2)
    //log2(x) = logE(x) / logE(2)
    if (it->count) {
        return (size_t)log2(it->count);
    }
    return 0;
}

fun(width, size_t)) as(MCHeap)
    size_t height = it->height(it);
    return (size_t)exp2(height-1);
}

fun(bye, void)) as(MCHeap)
    if (it->values) {
        free(it->values);
    }
}

fun(initWithCopy, struct MCHeap*), struct MCHeap* ref) as(MCHeap)
    MCHeap(it, ref->maxcount);
    memcpy(it->values, ref->values, sizeof(mc_generic) * ref->maxcount);
    it->count = ref->count;
    return it;
}

fun(insertValue, size_t), mc_generic newval) as(MCHeap)
    struct MCHeap* heap = it;
    heap->values[++heap->count] = newval;
    shiftup(heap, heap->count);
    return 0;
}

fun(copySortAscend, struct MCArray*)) as(MCHeap)
    struct MCHeap* hcopy = MCHeap(alloc(MCHeap), it->maxcount);
    struct MCArray* array = MCArray(alloc(MCArray), it->maxcount);
    while (hcopy->count > 0) {
        array->addItem(array, deleteRoot(hcopy));
    }
    hcopy->release(hcopy);
    return array;
}

fun(printAll, void)) as(MCHeap)
    int i;
    for (i=1; i<it->count; i++) {
        printf("%.2f ", it->values[i].f);
    }
    printf("\n");

    //printNode(heap, 1);
}

fun(release, void)) as(MCObject)
    it->release(it);
}

constructor(MCHeap), size_t maxcount) {
    MCObject(any);
    as(MCHeap)
        //index 0 is reserved
        it->count = 0;
        it->values = (mc_generic*)malloc(sizeof(mc_generic) * maxcount);
        it->maxcount = maxcount;
        it->maxheight = (size_t)log2(maxcount);
    }
    dynamic(MCHeap)
        funbind(height);
        funbind(width);
        funbind(bye);
        funbind(initWithCopy);
        funbind(insertValue);
        funbind(copySortAscend);
        funbind(printAll);
        funbind(release);
    }
    return any;
}
