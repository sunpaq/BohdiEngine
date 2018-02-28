//
//  MCTextureCache.c
//  BohdiEngine
//
//  Created by 孙御礼 on 2/26/30 H.
//

#include "MCTextureCache.h"

static MCTextureCache* _instance = null;

utility(MCTextureCache, MCTextureCache*, shared, voida)
{
    if (_instance == null) {
        _instance = new(MCTextureCache);
    }
    return _instance;
}

utility(MCTextureCache, void, cleanAndDestoryShared, voida)
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

method(MCTextureCache, void, bye, voida)
{
    release(var(map));
    var(map) = null;
    MCObject_bye(sobj, 0);
}

method(MCTextureCache, MCTexture*, findTextureNamed, const char* name)
{
    MCGeneric result;
    MCMap_getValueForKey(var(map), &result, name);
    if (result.mcobject) {
        return (MCTexture*)result.mcobject;
    } else {
        return null;
    }
}

method(MCTextureCache, void, cacheTextureNamed, MCTexture* tex, const char* name)
{
    if (tex) {
        MCGeneric result;
        MCMap_getValueForKey(var(map), &result, name);
        if (!result.mcobject) {
            MCMap_setValueForKey(var(map), MCGenericO(cast(MCObject*, tex)), name);
            retain(tex);
        }
    }
}

onload(MCTextureCache) {
    if (load(MCObject)) {
        binding(MCTextureCache, void, bye, voida);
        binding(MCTextureCache, MCTexture*, findTextureNamed, const char* name);
        binding(MCTextureCache, void, cacheTextureNamed, MCTexture* tex, const char* name);
        return cla;
    } else {
        return null;
    }
}
