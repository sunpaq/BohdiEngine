//
//  MC3DModelCache.c
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#include "MC3DModelCache.h"

static MC3DModelCache_t* instance = null;

struct MC3DModelCache* MC3DModelCache_shared()
{
    if (!instance) {
        instance = MC3DModelCache(alloc(MC3DModelCache));
    }
    return instance;
}

fun(loadModelNamed, struct MC3DModel*), const char* name) as(MC3DModelCache)
    //MC3DModel* model = MC3DModelCache_fetchModelNamed(obj, name);
    mc_generic result;
    it->map->getValueForKey(it->map, &result, name);
    if (result.mcobject) {
        return (struct MC3DModel*)result.mcobject;
    } else {
        struct MC3DModel* model = MC3DModel(alloc(MC3DModel));
        model->initWithFileName(model, name);
        struct MCObject* mobj = (struct MCObject*)model;
        it->map->setValueForKey(it->map, gen_o(mobj), name);
        return model;
    }
}

fun(fetchModelNamed, struct MC3DModel*), const char* name) as(MC3DModelCache)
    if (name) {
        mc_generic result;
        it->map->getValueForKey(it->map, &result, name);
        if (result.mcobject) {
            struct MC3DModel* model = (struct MC3DModel*)result.mcobject;
            return model;
        }
    }
    return null;
}

constructor(MC3DModelCache))
{
    MCObject(any);
    as(MC3DModelCache)
        it->map = MCMap(alloc(MCMap));
    }
    dynamic(MC3DModelCache)
        funbind(loadModelNamed);
        funbind(fetchModelNamed);
    }
    return any;
}

