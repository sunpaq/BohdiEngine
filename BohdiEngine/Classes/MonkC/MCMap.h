//
//  MCMap.h
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#ifndef MCMap_h
#define MCMap_h

#include "monkc.h"

class(MCMap, MCObject,
      mc_hashtable* table;
      computing(MCHashTableSize, itemsCount));

fun(MCMap, void, bye, voida);
fun(MCMap, void, setValueForKey, MCGeneric value, const char* key);
fun(MCMap, void, getValueForKey, MCGeneric* result, const char* key);
fun(MCMap, void, getValueByIndex, MCGeneric* result, MCHashTableIndex index);

#endif /* MCMap_h */
