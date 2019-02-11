//
//  MCMap.h
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#ifndef MCMap_h
#define MCMap_h

#include "MCObject.h"
#include "MCHashTable.h"

structure(MCMap, MCObject)
      struct MCHashTable* table;

      fundef(itemsCount, size_t));
      fundef(setValueForKey, void), mc_generic value, const char* key);
      fundef(getValueForKey, void), mc_generic* result, const char* key);
      fundef(getValueByIndex, void), mc_generic* result, size_t index);
      fundef(release, void));
};

constructor(MCMap));

alias(MCMap);

#endif /* MCMap_h */
