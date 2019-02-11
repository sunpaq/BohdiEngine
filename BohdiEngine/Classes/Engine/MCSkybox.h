//
//  MCSkybox.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSkybox_h
#define MCSkybox_h

//#include "monkc_export.h"
#include "MC3DNode.h"
#include "BECubeTextureData.h"

structure(MCSkybox, MC3DNode)
    double boxCameraRatio;
    double boxCameraAngle;

    float skyboxVertices[24];
    float indexs[36];

    struct BECubeTextureData* cubedata;


    fundef(release, void));
    fundef(initWithCubeTexture, struct MCSkybox*), struct BECubeTextureData* cubetex);
    fundef(initWithFileNames, struct MCSkybox*), const char* namelist[]);
    fundef(initWithDefaultFiles, struct MCSkybox*));
    //override
    fundef(setRotationMat3, void), float mat3[9]);
    fundef(setRotationMat4, void), float mat4[16]);
    //property
    fundef(getViewMatrix, void), MCMatrix4* mat4);
    fundef(getProjectionMatrix, void), MCMatrix4* mat4);
};

constructor(MCSkybox));

alias(MCSkybox);

#endif /* MCSkybox_h */
