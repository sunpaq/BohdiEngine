//
//  MCGLSkybox.c
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

oninit(MCGLSkybox)
{
    if (init(MC3DNode)) {
        var(boxCameraRatio)= 9.0/16.0;
        var(boxCameraAngle)= M_PI * 0.55;
        
        for (int i=0; i<24; i++) {
            var(skyboxVertices)[i] = skyboxVertices[i];
        }
        
        for (int i=0; i<36; i++) {
            var(indexs)[i] = indexs[i];
        }
        
        return obj;
    }else{
        return null;
    }
}

method(MCGLSkybox, void, bye, voida)
{
    MC3DNode_bye(sobj, 0);
}

method(MCGLSkybox, MCGLSkybox*, initWithCubeTexture, BECubeTextureData* cubetex)
{
    obj->cubedata = cubetex;
    return obj;
}

method(MCGLSkybox, MCGLSkybox*, initWithFileNames, const char* namelist[])
{
    BECubeTextureData* data = BECubeTextureData_newWithFaces(namelist);
    MCGLSkybox* skybox = MCGLSkybox_initWithCubeTexture(obj, data);
    release(data);
    return skybox;
}

method(MCGLSkybox, MCGLSkybox*, initWithDefaultFiles, voida)
{
    const char* names[6] = {"right.jpg","left.jpg","top.jpg","bottom.jpg","back.jpg","front.jpg"};
    return MCGLSkybox_initWithFileNames(obj, names);
}

method(MCGLSkybox, void, setRotationMat3, float mat3[9])
{
    MC3DNode_rotateMat3(sobj, mat3, false);
}

method(MCGLSkybox, void, setRotationMat4, float mat4[16])
{
    MC3DNode_rotateMat4(sobj, mat4, false);
}

//property
method(MCGLSkybox, void, getViewMatrix, MCMatrix4* mat4)
{
    MCMatrix4 m = MCMatrix4MakeLookAt(0, 0, 0,
                                      0, 0,-1,
                                      0, 1, 0);
    MCMatrix4 imat4 = MCMatrix4Invert(obj->Super.transform, null);
    *mat4 = MCMatrix4Multiply(m, imat4);
}

method(MCGLSkybox, void, getProjectionMatrix, MCMatrix4* mat4)
{
    *mat4 = MCMatrix4MakePerspective(obj->boxCameraAngle,
                                    obj->boxCameraRatio,
                                    0.001,
                                    200.0);
}

onload(MCGLSkybox)
{
    if (load(MC3DNode)) {
        binding(MCGLSkybox, void, bye, voida);
        binding(MCGLSkybox, MCGLSkybox*, initWithCubeTexture, BECubeTextureData* cubetex);
        binding(MCGLSkybox, MCGLSkybox*, initWithFileNames, const char* namelist[]);
        binding(MCGLSkybox, MCGLSkybox*, initWithDefaultFiles, voida);
        binding(MCGLSkybox, void, setRotationMat3, float mat3[9]);
        binding(MCGLSkybox, void, setRotationMat4, float mat4[16]);
        binding(MCGLSkybox, void, getViewMatrix, MCMatrix4* mat4);
        binding(MCGLSkybox, void, getProjectionMatrix, MCMatrix4* mat4);
        return cla;
    }else{
        return null;
    }
}
