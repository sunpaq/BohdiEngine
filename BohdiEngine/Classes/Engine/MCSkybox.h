//
//  MCSkybox.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSkybox_h
#define MCSkybox_h

#include "monkc_export.h"
#include "MC3DNode.h"

class(MCSkybox, MC3DNode,
      double boxCameraRatio;
      double boxCameraAngle;

      float skyboxVertices[24];
      float indexs[36];

      BECubeTextureData* cubedata;
);

fun(MCSkybox, void, bye, voida);
fun(MCSkybox, MCSkybox*, initWithCubeTexture, BECubeTextureData* cubetex);
fun(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[]);
fun(MCSkybox, MCSkybox*, initWithDefaultFiles, voida);
//override
fun(MCSkybox, void, setRotationMat3, float mat3[9]);
fun(MCSkybox, void, setRotationMat4, float mat4[16]);
//property
fun(MCSkybox, void, getViewMatrix, MCMatrix4* mat4);
fun(MCSkybox, void, getProjectionMatrix, MCMatrix4* mat4);

#endif /* MCSkybox_h */
