//
//  MC3DModelCache.h
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#ifndef MC3DModelCache_h
#define MC3DModelCache_h

#include "monkc_export.h"
#include "MC3DModel.h"

class(MC3DModelCache, MCObject,
      MCMap* map);

util(MC3DModelCache, MC3DModelCache*, shared, voida);

fun(MC3DModelCache, MC3DModel*, loadModelNamed, const char* name);
fun(MC3DModelCache, MC3DModel*, fetchModelNamed, const char* name);

#endif /* MC3DModelCache_h */
