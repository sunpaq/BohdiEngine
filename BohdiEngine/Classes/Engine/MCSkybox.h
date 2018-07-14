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
      double boxCameraRatio;
      double boxCameraAngle;

      float skyboxVertices[24];
      float indexs[36];

      BECubeTextureData* cubedata;
);

method(MCGLSkybox, void, bye, voida);
method(MCGLSkybox, MCGLSkybox*, initWithCubeTexture, BECubeTextureData* cubetex);
method(MCGLSkybox, MCGLSkybox*, initWithFileNames, const char* namelist[]);
method(MCGLSkybox, MCGLSkybox*, initWithDefaultFiles, voida);
//override
method(MCGLSkybox, void, setRotationMat3, float mat3[9]);
method(MCGLSkybox, void, setRotationMat4, float mat4[16]);
//property
method(MCGLSkybox, void, getViewMatrix, MCMatrix4* mat4);
method(MCGLSkybox, void, getProjectionMatrix, MCMatrix4* mat4);

#endif /* MCGLSkybox_h */
