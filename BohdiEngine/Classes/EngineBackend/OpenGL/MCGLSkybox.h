//
//  MCGLSkybox.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCGLSkybox_h
#define MCGLSkybox_h

#include "monkc.h"
#include "MC3DNode.h"
#include "BECubeTextureData.h"

class(MCGLSkybox, MC3DNode,
      MCGLContext* ctx;
      
      MCMatrix4 boxViewMatrix;
      MCMatrix4 boxProjectionMatrix;
      double boxCameraRatio;
      double boxCameraAngle;
      
      MCUInt vaoid;
      MCUInt vboid;
      MCUInt eboid;
      MCUInt texid);

method(MCGLSkybox, void, bye, voida);
method(MCGLSkybox, MCGLSkybox*, initWithCubeTexture, BECubeTextureData* cubetex);
method(MCGLSkybox, MCGLSkybox*, initWithFileNames, const char* namelist[]);
method(MCGLSkybox, MCGLSkybox*, initWithDefaultFiles, voida);
//override
method(MCGLSkybox, void, setRotationMat3, float mat3[9]);
method(MCGLSkybox, void, setRotationMat4, float mat4[16]);
method(MCGLSkybox, void, update, MCGLContext* ctx);
method(MCGLSkybox, void, draw, MCGLContext* ctx);

#endif /* MCGLSkybox_h */
