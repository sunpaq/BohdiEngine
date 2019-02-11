//
//  MCSkybox.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSkybox.h"
#include "MCCamera.h"

static float skyboxVertices[] = {
    -1.0f, -1.0f, -1.0f, //000 0
    -1.0f, -1.0f,  1.0f, //001 1
    -1.0f,  1.0f, -1.0f, //010 2
    -1.0f,  1.0f,  1.0f, //011 3
    1.0f, -1.0f, -1.0f,  //100 4
    1.0f, -1.0f,  1.0f,  //101 5
    1.0f,  1.0f, -1.0f,  //110 6
    1.0f,  1.0f,  1.0f,  //111 7
};

static float indexs[] = {
    2,0,4,4,6,2,
    1,0,2,2,3,1,
    4,5,7,7,6,4,
    1,3,7,7,5,1,
    2,6,7,7,3,2,
    0,1,4,4,1,5
};

fun(release, void)) as(MC3DNode)
    it->release(it);
}

fun(initWithCubeTexture, struct MCSkybox*), struct BECubeTextureData* cubetex) as(MCSkybox)
    it->cubedata = cubetex;
    return it;
}

fun(initWithFileNames, struct MCSkybox*), const char* namelist[]) as(MCSkybox)
    struct BECubeTextureData* data = BECubeTextureData_newWithFaces(namelist);
    struct MCSkybox* skybox = it->initWithCubeTexture(it, data);
    Release(data);
    return skybox;
}

fun(initWithDefaultFiles, struct MCSkybox*)) as(MCSkybox)
    const char* names[6] = {"right.jpg","left.jpg","top.jpg","bottom.jpg","back.jpg","front.jpg"};
    return it->initWithFileNames(it, names);
}

fun(setRotationMat3, void), float mat3[9]) as(MC3DNode)
    it->rotateMat3(it, mat3, false);
}

fun(setRotationMat4, void), float mat4[16]) as(MC3DNode)
    it->rotateMat4(it, mat4, false);
}

//property
fun(getViewMatrix, void), MCMatrix4* mat4) as(MC3DNode)
    MCMatrix4 m = MCMatrix4MakeLookAt(0, 0, 0,
                                      0, 0,-1,
                                      0, 1, 0);
    MCMatrix4 imat4 = MCMatrix4Invert(it->transform, null);
    *mat4 = MCMatrix4Multiply(m, imat4);
}

fun(getProjectionMatrix, void), MCMatrix4* mat4) as(MCSkybox)
    *mat4 = MCMatrix4MakePerspective(it->boxCameraAngle,
                                    it->boxCameraRatio,
                                    0.001,
                                    200.0);
}

constructor(MCSkybox)) {
    MC3DNode(any);
    as(MCSkybox)
        it->boxCameraRatio = 9.0/16.0;
        it->boxCameraAngle = M_PI * 0.55;
        int i, j;
        for (i=0; i<24; i++) {
            it->skyboxVertices[i] = skyboxVertices[i];
        }
        for (j=0; j<36; j++) {
            it->indexs[i] = indexs[i];
        }
    }
    dynamic(MCSkybox)
        funbind(release);
        funbind(initWithCubeTexture);
        funbind(initWithFileNames);
        funbind(initWithDefaultFiles);
        //override
        funbind(setRotationMat3);
        funbind(setRotationMat4);
        //property
        funbind(getViewMatrix);
        funbind(getProjectionMatrix);
    }
    return any;
}
