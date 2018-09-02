//
//  MCSkysphere.c
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#include "MCSkysphere.h"
#include "MC3DShapeBase.h"

oninit(MCSkysphere)
{
    if (init(MC3DNode)) {
        obj->Super.visible = true;
        
        var(tex) = null;
        var(sphViewMatrix) = MCMatrix4Identity;
        var(sphProjectionMatrix) = MCMatrix4Identity;
        var(sphCameraAngle) = M_PI * 0.55;
        var(sphCameraRatio) = 9.0/16.0;
        
        var(R_value) = 200.0;
        var(R_percent) = 1.0;
        var(tht) = 0.0;
        var(fai) = 0.0;
        
        int nr = 72;
        int nc = 144;
        
        var(vertices_size) = sizeof(GLfloat) * nr * nc * 5;
        var(indices_size)  = sizeof(GLuint) * nr * nc * 2;
        
        var(vertices) = (GLfloat*)malloc(obj->vertices_size);
        var(indices)  = (GLuint*)malloc(obj->indices_size);
        var(ic) = MCGenerateSkysphere(nr, nc, 100.0, var(vertices), var(indices));
        
        return obj;
    }else{
        return null;
    }
}

fun(MCSkysphere, void, bye, voida)
{
    
    free(var(vertices));
    free(var(indices));
    
//    GLuint texids[1] = {obj->texid};
//    glDeleteTextures(1, texids);
//
//    GLuint buffs[3] = {obj->vaoid, obj->vboid, obj->eboid};
//    glDeleteBuffers(3, buffs);
    
    //release(var(ctx));
    release(var(tex));
    
    MC3DNode_bye(sobj, 0);
}

fun(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex)
{
    retain(tex);
    var(tex) = tex;
    return obj;
}

fun(MCSkysphere, MCSkysphere*, initWithFileName, const char* name)
{
    BE2DTextureData* data = BE2DTextureData_newWithFilename(name);
    if (data) {
        MCSkysphere* sph = MCSkysphere_initWithBE2DTexture(obj, data);
        release(data);
        return sph;
    }
    return null;
}

fun(MCSkysphere, MCSkysphere*, initWithDefaultFile, voida)
{
    return MCSkysphere_initWithFileName(obj, "skysphtex.jpg");
}

fun(MCSkysphere, void, getViewMatrix, MCMatrix4* mat4)
{
    MCMatrix4 m = MCMatrix4MakeLookAt(0, 0, 0,
                                      0, 0,-1,
                                      0, 1, 0);
    MCMatrix4 imat4 = MCMatrix4Invert(obj->Super.transform, null);
    *mat4 = MCMatrix4Multiply(m, imat4);
}

fun(MCSkysphere, void, getProjectionMatrix, MCMatrix4* mat4)
{
    *mat4 = MCMatrix4MakePerspective(obj->sphCameraAngle,
                                    obj->sphCameraRatio,
                                    0.001,
                                    200.0);
}

fun(MCSkysphere, void, setRotationMat3, float mat3[9])
{
    MC3DNode_rotateMat3(sobj, mat3, false);
}

fun(MCSkysphere, void, setRotationMat4, float mat4[16])
{
    MC3DNode_rotateMat4(sobj, mat4, false);
}

fun(MCSkysphere, void, transformSelfByEularAngle, double R, double fai, double tht)
{
    //MCMatrix4 lookat = MCMatrix4MakeLookAt(0, 0, 0, 0, 0,-1, 0, 1, 0);
    //double R = var(R_value) * var(R_percent);
    //sobj->transform = MCMatrix4MakeLookAtByEulerAngle_EyeUp(lookat, R, fai, tht, null, null);
}

onload(MCSkysphere)
{
    if (load(MC3DNode)) {
        bid(MCSkysphere, void, bye, voida);
        bid(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex);
        bid(MCSkysphere, MCSkysphere*, initWithFileName, const char* name);
        bid(MCSkysphere, MCSkysphere*, initWithDefaultFile, voida);
        bid(MCSkysphere, void, setRotationMat3, float mat3[9]);
        bid(MCSkysphere, void, setRotationMat4, float mat4[16]);
        bid(MCSkysphere, void, getViewMatrix, MCMatrix4* mat4);
        bid(MCSkysphere, void, getProjectionMatrix, MCMatrix4* mat4);
        return cla;
    }else{
        return null;
    }
}
