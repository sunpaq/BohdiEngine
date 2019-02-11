//
//  BECubeTextureData.h
//  Sapindus
//
//  Created by YuliSun on 24/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#ifndef BECubeTextureData_h
#define BECubeTextureData_h

#include "MCObject.h"
#include "BE2DTextureData.h"

//usual face order: "right","left","top","bottom","back","front"
structure(BECubeTextureData, MCObject)
    struct BE2DTextureData* faces[6];
    fundef(release, void));
};

constructor(BECubeTextureData));

alias(BECubeTextureData);

struct BECubeTextureData* BECubeTextureData_newWithFacePaths(const char* facepaths[6]);
struct BECubeTextureData* BECubeTextureData_newWithFaces(const char* faces[6]);

//BEAssetsManager is an utility class
//util(BEAssetsManager, BE2DTextureData*, load2DTexture, BE2DTextureData* texture);

#endif /* BECubeTextureData_h */
