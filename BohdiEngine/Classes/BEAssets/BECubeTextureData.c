//
//  BECubeTextureData.c
//  Sapindus
//
//  Created by YuliSun on 24/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#include "BECubeTextureData.h"
#include "BEAssetsManager.h"
#include "MCLog.h"

struct BECubeTextureData* BECubeTextureData_newWithFacePaths(const char* facepaths[6])
{
    struct BECubeTextureData* data = new(BECubeTextureData);
    int i;
    for (i=0; i<6; i++) {
        struct BE2DTextureData* aface = BE2DTextureData_newWithPathname(facepaths[i]);
        if (aface != null) {
            data->faces[i] = aface;
        }else{
            error_log("BECubeTextureData image %s data is null!\n", facepaths[i]);
            return null;
        }
    }
    return data;
}

struct BECubeTextureData* BECubeTextureData_newWithFaces(const char* faces[6])
{
    struct BECubeTextureData* data = new(BECubeTextureData);
    char pathbuff[PATH_MAX] = {0};
    int i;
    for (i=0; i<6; i++) {
        if(MCFileGetPath(faces[i], pathbuff)){
            return null;
        }
        pathbuff[PATH_MAX-1] = NUL;
        data->faces[i] = BE2DTextureData_newWithPathname(pathbuff);
    }
    return data;
}

fun(release, void)) as(BECubeTextureData)
    int i;
    for (i=0; i<6; i++) {
        struct BE2DTextureData* face = it->faces[i];
        if (face != null) {
            Release(face);
        }
    }
    cast(it, MCObject)->release(it);
}

constructor(BECubeTextureData)) {
    MCObject(any);
    dynamic(BECubeTextureData)
        funbind(release);
    }
    return any;
}

