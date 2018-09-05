//
//  MCTextureCache.c
//  BohdiEngine
//
//  Created by 孙御礼 on 2/26/30 H.
//

#include "MCTextureCache.h"

static MCTextureCache* _instance = null;

util(MCTextureCache, MCTextureCache*, shared, voida)
{
    if (_instance == null) {
        _instance = new(MCTextureCache);
    }
    return _instance;
}

util(MCTextureCache, void, cleanAndDestoryShared, voida)
{
    if (_instance) {
        release(_instance);
        _instance = null;
    }
}

oninit(MCTextureCache) {
    if (init(MCObject)) {
        obj->map = new(MCMap);
        return obj;
    } else {
        return null;
    }
}

fun(MCTextureCache, void, bye, voida)
{
    release(var(map));
    var(map) = null;
    MCObject_bye(sobj, 0);
}

fun(MCTextureCache, MCTexture*, findTextureNamed, const char* name)
{
    MCGeneric result;
    MCMap_getValueForKey(var(map), &result, name);
    if (result.mcobject) {
        debug_log("MCTextureCache - find object for key: %s\n", name);
        return (MCTexture*)result.mcobject;
    } else {
        return null;
    }
}

fun(MCTextureCache, void, cacheTextureNamed, MCTexture* tex, const char* name)
{
    if (tex) {
        retain(tex);
        MCGeneric result;
        MCMap_getValueForKey(var(map), &result, name);
        if (!result.mcobject) {
            MCMap_setValueForKey(var(map), MCGenericO(cast(MCObject*, tex)), name);
        } else {
            debug_log("MCTextureCache - already have object for key: %s\n", name);
        }
    }
}

onload(MCTextureCache) {
    if (load(MCObject)) {
        bid(MCTextureCache, void, bye, voida);
        bid(MCTextureCache, MCTexture*, findTextureNamed, const char* name);
        bid(MCTextureCache, void, cacheTextureNamed, MCTexture* tex, const char* name);
        return cla;
    } else {
        return null;
    }
}
