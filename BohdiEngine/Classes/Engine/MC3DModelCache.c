//
//  MC3DModelCache.c
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#include "MC3DModelCache.h"

static MC3DModelCache* instance = null;

oninit(MC3DModelCache)
{
    if (init(MCObject)) {
        instance->map = new(MCMap);
        return obj;
    } else {
        return null;
    }
}

utility(MC3DModelCache, MC3DModelCache*, shared, voida)
{
    if (!instance) {
        instance = new(MC3DModelCache);
    }
    return instance;
}

method(MC3DModelCache, MC3DModel*, loadModelNamed, const char* name)
{
    MC3DModel* model = MC3DModel_initWithFileName(0, new(MC3DModel), name);
    if (model) {
        MCMap_setValueForKey(0, obj->map, model, name);
    }
    return model;
}

method(MC3DModelCache, MC3DModel*, fetchModelNamed, const char* name)
{
    if (name) {
        MC3DModel* model = (MC3DModel*)MCMap_getValueForKey(0, obj->map, name);
        return model;
    }
    return null;
}

onload(MyClass)
{
    if (load(MCObject)) {
        binding(MC3DModelCache, MC3DModel*, loadModelNamed, const char* name);
        binding(MC3DModelCache, MC3DModel*, fetchModelNamed, const char* name);
        return cla;
    } else {
        return null;
    }
}
