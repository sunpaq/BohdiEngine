//
//  MCSet.h
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/12.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSet_h
#define MCSet_h

#include <stdio.h>
#include "MCObject.h"

structure(MCSet, MCObject)
    mc_generic* values;
    size_t maxcount;
    size_t count;

    fundef(bye, void));
    fundef(insertValue, struct MCSet*), mc_generic value);
};

constructor(MCSet), size_t maxcount);

alias(MCSet);

/*
 - Disjoint Sets
 - Merge-find Sets
 - Union-find Data Structure
   they all the same thing
 */

structure(MCUnionFind, MCSet)
    fundef(findAction, void));
    fundef(unionAction, void));
};

constructor(MCUnionFind));

alias(MCUnionFind);

#endif /* MCSet_h */
