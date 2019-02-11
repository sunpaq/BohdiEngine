//
//  MCSkysphere.h
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#ifndef MCSkysphere_h
#define MCSkysphere_h

//#include "monkc_export.h"
#include "MC3DNode.h"
#include "BE2DTextureData.h"
#include "MC3DNode.h"

structure(MCSkysphere, MC3DNode)
    struct BE2DTextureData* tex;

    MCMatrix4 sphViewMatrix;
    MCMatrix4 sphProjectionMatrix;
    double sphCameraRatio;
    double sphCameraAngle;

    //local spherical coordinate
    //R[0,unlimited) tht[0, 180.0), fai[0, 360.0)
    double R_value;
    double R_percent;
    double tht;
    double fai;

    size_t vertices_size;
    size_t indices_size;
    float* vertices;
    uint32_t* indices;
    uint32_t ic;


    fundef(release, void));
    fundef(initWithBE2DTexture, struct MCSkysphere*), struct BE2DTextureData* tex);
    fundef(initWithFileName, struct MCSkysphere*), const char* name);
    fundef(initWithDefaultFile, struct MCSkysphere*));

    fundef(setRotationMat3, void), float mat3[9]);
    fundef(setRotationMat4, void), float mat4[16]);

    //property
    fundef(getViewMatrix, void), MCMatrix4* mat4);
    fundef(getProjectionMatrix, void), MCMatrix4* mat4);
};

constructor(MCSkysphere));

alias(MCSkysphere);

#endif /* MCSkysphere_h */

