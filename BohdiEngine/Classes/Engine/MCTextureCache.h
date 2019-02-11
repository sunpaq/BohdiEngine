//
//  MCTextureCache.h
//  BohdiEngine
//
//  Created by 孙御礼 on 2/26/30 H.
//

#ifndef MCTextureCache_h
#define MCTextureCache_h

#include "MCObject.h"
#include "MCTexture.h"

structure(MCTextureCache, MCObject)
    struct MCMap* map;

    fundef(release, void));
    fundef(findTextureNamed, struct MCTexture*), const char* name);
    fundef(cacheTextureNamed, void), struct MCTexture* tex, const char* name);
};

constructor(MCTextureCache));

alias(MCTextureCache);

util(MCTextureCache, shared, struct MCTextureCache*)void);
util(MCTextureCache, cleanAndDestoryShared, void)void);

#endif /* MCTextureCache_h */
