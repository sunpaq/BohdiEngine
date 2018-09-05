//
//  MCHeap.h
//  Sapindus
//
//  Created by YuliSun on 17/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#ifndef MCHeap_h
#define MCHeap_h

#include "monkc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "MCArray.h"

//min heap
class(MCHeap, MCObject,
      computing(size_t, height);
      computing(size_t, width);
      MCGeneric* values;
      size_t count;
      size_t maxcount;
      size_t maxheight;
);

fun(MCHeap, void, bye, voida);
fun(MCHeap, MCHeap*, initWithCopy, MCHeap* ref);
fun(MCHeap, MCHeap*, initWithMaxcount, size_t maxcount);
fun(MCHeap, size_t, insertValue, MCGeneric newval);
fun(MCHeap, MCArray*, copySortAscend, voida);
fun(MCHeap, void, printAll, voida);

#endif /* MCHeap_h */
