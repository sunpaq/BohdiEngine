//
//  BECubeTextureData.h
//  Sapindus
//
//  Created by YuliSun on 24/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#ifndef BECubeTextureData_h
#define BECubeTextureData_h

#include "monkc_export.h"
#include "BE2DTextureData.h"

//usual face order: "right","left","top","bottom","back","front"
class(BECubeTextureData, MCObject,
      BE2DTextureData* faces[6]
);

util(BECubeTextureData, BECubeTextureData*, newWithFacePaths, const char* facepaths[6]);
util(BECubeTextureData, BECubeTextureData*, newWithFaces, const char* faces[6]);
fun(BECubeTextureData, void, bye, voida);

//BEAssetsManager is an utility class
//util(BEAssetsManager, BE2DTextureData*, load2DTexture, BE2DTextureData* texture);

#endif /* BECubeTextureData_h */
