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
#include "monkc.h"

class(MCSet, MCObject,
      MCGeneric* values;
      size_t maxcount;
      size_t count;);

fun(MCSet, void, bye, voida);
fun(MCSet, MCSet*, initWithMaxCount, size_t maxcount);
fun(MCSet, MCSet*, insertValue, MCGeneric value);

/*
 - Disjoint Sets
 - Merge-find Sets
 - Union-find Data Structure
   they all the same thing
 */

class(MCUnionFind, MCSet);

fun(MCUnionFind, void, find, voida);
fun(MCUnionFind, void, union, voida);


#endif /* MCSet_h */
