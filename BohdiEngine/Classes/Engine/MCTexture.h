//
//  MCTexture.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCTexture_h
#define MCTexture_h

#include "beengine_import.h"

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

method(MCTexture, void, bye, voida);
method(MCTexture, MCTexture*, initWithFileNameMode, const char* name, MCTextureDisplayMode mode);
method(MCTexture, MCTexture*, initWithFileName, const char* name);
method(MCTexture, MCTexture*, initWith2DTexture, BE2DTextureData* tex);
//method(MCTexture, void, loadToGLBuffer, GLuint pid, const char* uniformName);
//method(MCTexture, void, active, GLuint pid, const char* uniformName);

#endif /* MCTexture_h */
