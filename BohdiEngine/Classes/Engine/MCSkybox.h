//
//  MCSkybox.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSkybox_h
#define MCSkybox_h

#include "beengine_import.h"
#include "MC3DNode.h"

class(MCSkybox, MC3DNode,
      double boxCameraRatio;
      double boxCameraAngle;

      float skyboxVertices[24];
      float indexs[36];

      BECubeTextureData* cubedata;
);

method(MCSkybox, void, bye, voida);
method(MCSkybox, MCSkybox*, initWithCubeTexture, BECubeTextureData* cubetex);
method(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[]);
method(MCSkybox, MCSkybox*, initWithDefaultFiles, voida);
//override
method(MCSkybox, void, setRotationMat3, float mat3[9]);
method(MCSkybox, void, setRotationMat4, float mat4[16]);
//property
method(MCSkybox, void, getViewMatrix, MCMatrix4* mat4);
method(MCSkybox, void, getProjectionMatrix, MCMatrix4* mat4);

#endif /* MCSkybox_h */
