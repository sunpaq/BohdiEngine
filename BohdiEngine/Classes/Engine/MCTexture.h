//
//  MCTexture.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCTexture_h
#define MCTexture_h

#include "MCObject.h"
//#include "beassets_export.h"

typedef enum {
    MCTextureRepeat,
    MCTextureClampToEdge
} MCTextureDisplayMode;

structure(MCTexture, MCObject)
    unsigned Id;
    int width;
    int height;
    struct BE2DTextureData* data;
    MCTextureDisplayMode displayMode;
    bool loadedToGL;

    fundef(release, void));
    fundef(loadImageRawdata, unsigned char*), const char* path);
    fundef(initWithFileNameMode, struct MCTexture*), const char* name, MCTextureDisplayMode mode);
    fundef(initWithFileName, struct MCTexture*), const char* name);
    fundef(initWith2DTexture, struct MCTexture*), struct BE2DTextureData* tex);
};

constructor(MCTexture));

alias(MCTexture);

#endif /* MCTexture_h */
