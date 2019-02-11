//
//  MCTextureCache.c
//  BohdiEngine
//
//  Created by 孙御礼 on 2/26/30 H.
//

#include "MCTextureCache.h"
#include "MCMap.h"
#include "MCLog.h"

static MCTextureCache_t* _instance = null;

util(MCTextureCache, shared, struct MCTextureCache*))
{
    if (_instance == null) {
        _instance = MCTextureCache(alloc(MCTextureCache));
    }
    return _instance;
}

util(MCTextureCache, cleanAndDestoryShared, void))
{
    if (_instance) {
        Release(_instance);
        _instance = null;
    }
}

fun(release, void)) as(MCTextureCache)
    Release(it->map);
    it->map = null;
    cast(it, MCObject)->release(it);
}

fun(findTextureNamed, struct MCTexture*), const char* name) as(MCTextureCache)
    mc_generic result;
    it->map->getValueForKey(it->map, &result, name);
    if (result.mcobject) {
        debug_log("MCTextureCache - find object for key: %s\n", name);
        return (struct MCTexture*)result.mcobject;
    } else {
        return null;
    }
}

fun(cacheTextureNamed, void), struct MCTexture* tex, const char* name) as(MCTextureCache)
    if (tex) {
        Retain(tex);
        mc_generic result;
        it->map->getValueForKey(it->map, &result, name);
        if (!result.mcobject) {
            it->map->setValueForKey(it->map, gen_p(tex), name);
        } else {
            debug_log("MCTextureCache - already have object for key: %s\n", name);
        }
    }
}

constructor(MCTextureCache)) {
    MCObject(any);
    as(MCTextureCache)
        it->map = MCMap(alloc(MCMap));
    }
    dynamic(MCTextureCache)
        funbind(release);
        funbind(findTextureNamed);
        funbind(cacheTextureNamed);
    }
    return any;
}
