//
//  MCTexture.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCTexture_h
#define MCTexture_h

#include "monkc_export.h"
#include "beassets_export.h"

typedef enum {
    MCTextureRepeat,
    MCTextureClampToEdge
} MCTextureDisplayMode;

class(MCTexture, MCObject,
      unsigned Id;
      int width;
      int height;
      BE2DTextureData* data;
      MCTextureDisplayMode displayMode;
      MCBool loadedToGL;
);

fun(MCTexture, void, bye, voida);
fun(MCTexture, MCTexture*, initWithFileNameMode, const char* name, MCTextureDisplayMode mode);
fun(MCTexture, MCTexture*, initWithFileName, const char* name);
fun(MCTexture, MCTexture*, initWith2DTexture, BE2DTextureData* tex);
//fun(MCTexture, void, loadToGLBuffer, GLuint pid, const char* uniformName);
//fun(MCTexture, void, active, GLuint pid, const char* uniformName);

#endif /* MCTexture_h */
