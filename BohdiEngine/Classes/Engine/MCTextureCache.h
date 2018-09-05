//
//  MCTextureCache.h
//  BohdiEngine
//
//  Created by 孙御礼 on 2/26/30 H.
//

#ifndef MCTextureCache_h
#define MCTextureCache_h

#include "monkc_export.h"
#include "MCTexture.h"

class(MCTextureCache, MCObject,
      MCMap* map);

util(MCTextureCache, MCTextureCache*, shared, voida);
util(MCTextureCache, void, cleanAndDestoryShared, voida);

fun(MCTextureCache, void, bye, voida);
fun(MCTextureCache, MCTexture*, findTextureNamed, const char* name);
fun(MCTextureCache, void, cacheTextureNamed, MCTexture* tex, const char* name);

#endif /* MCTextureCache_h */
