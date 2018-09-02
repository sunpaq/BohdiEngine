//
//  MCSort.h
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/5.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSort_h
#define MCSort_h

#include "monkc.h"

class(MCSort, MCObject,
      MCGeneric* array;
      size_t length);

fun(MCSort, void, bye, voida);
fun(MCSort, MCSort*, initWithArray, MCGeneric* array, size_t length);
fun(MCSort, void, insertionSort, voida);
fun(MCSort, void, quickSort, voida);
fun(MCSort, void, printArray, voida);

#endif /* MCSort_h */
