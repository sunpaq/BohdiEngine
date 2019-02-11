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
structure(MCHeap, MCObject)
      mc_generic* values;
      size_t count;
      size_t maxcount;
      size_t maxheight;

      fundef(height, size_t));
      fundef(width, size_t));

      fundef(bye, void));
      fundef(initWithCopy, struct MCHeap*), struct MCHeap* ref);
      fundef(insertValue, size_t), mc_generic newval);
      fundef(copySortAscend, struct MCArray*));
      fundef(printAll, void));
      fundef(release, void));
};

constructor(MCHeap), size_t maxcount);

alias(MCHeap);

#endif /* MCHeap_h */
