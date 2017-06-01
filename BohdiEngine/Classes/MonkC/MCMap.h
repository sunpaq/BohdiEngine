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
      mc_hashtable* table);

method(MCMap, void, setValueForKey, void* value, const char* key);
method(MCMap, void*, getValueForKey, const char* key);

#endif /* MCMap_h */
