//
//  BE2DTextureData.h
//  Sapindus
//
//  Created by YuliSun on 24/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#ifndef BE2DTextureData_h
#define BE2DTextureData_h

#include "MCObject.h"
#include "BE2DTextureData.h"

//handle image, no GL related code
structure(BE2DTextureData, MCObject)
    //const types
    unsigned AUTO;
    unsigned L;
    unsigned LA;
    unsigned RGB;
    unsigned RGBA;

    //input
    //unsigned type;
    char* path;

    //output
    unsigned char* raw;
    int width;
    int height;
    int channels;

    fundef(release, void));
};

constructor(BE2DTextureData));

alias(BE2DTextureData);

struct BE2DTextureData* BE2DTextureData_newWithPathnameType(const char* path, unsigned type);
struct BE2DTextureData* BE2DTextureData_newWithPathname(const char* path);
struct BE2DTextureData* BE2DTextureData_newWithFilename(const char* file);

#endif /* BE2DTextureData_h */
