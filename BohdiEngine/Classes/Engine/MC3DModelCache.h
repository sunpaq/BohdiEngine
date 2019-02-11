//
//  MC3DModelCache.h
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#ifndef MC3DModelCache_h
#define MC3DModelCache_h

#include "MCObject.h"
#include "MC3DModel.h"
#include "MCMap.h"

structure(MC3DModelCache, MCObject)
    struct MCMap* map;

    fundef(loadModelNamed, struct MC3DModel*), const char* name);
    fundef(fetchModelNamed, struct MC3DModel*), const char* name);
};

constructor(MC3DModelCache));

alias(MC3DModelCache);

struct MC3DModelCache* MC3DModelCache_shared(void);

#endif /* MC3DModelCache_h */
