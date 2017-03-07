//
//  MCSkysphereCamera.c
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#include "MCSkysphereCamera.h"

compute(MCMatrix4, sphViewMatrix);
compute(MCMatrix4, sphProjectionMatrix);

oninit(MCSkysphereCamera)
{
    if (init(MCCamera)) {
        //super
        sobj->ratio = MCRatioHDTV16x9;
        sobj->R_value = 10.0;
        sobj->R_percent = 1.0;
        sobj->tht = 0;
        sobj->fai = 90;
        
        //world coordinate
        sobj->lookat = MCVector3Make(0, -1, 0);
        sobj->eye    = MCVector3Make(0, 0, 0);
        sobj->up     = MCVector3Make(0, 0, 1);
        
        //uniforms
        obj->viewMatrix        = sphViewMatrix;
        obj->projectionMatrix  = sphProjectionMatrix;
        
        return obj;
    }else{
        return null;
    }
}

compute(MCMatrix4, sphViewMatrix)
{
    as(MCSkysphereCamera);
    MCMatrix4 m = MCMatrix4MakeLookAt(0, 0,0,
                                      0, 0,-1,
                                      0, 1,0);
    MCMatrix4 imat4 = MCMatrix4Invert(obj->Super.Super.transform, null);
    
    return MCMatrix4Multiply(m, imat4);
}

compute(MCMatrix4, sphProjectionMatrix)
{
    as(MCSkysphereCamera);
    return MCMatrix4MakePerspective(M_PI * 0.55,
                                    sobj->ratio,
                                    0.001,
                                    200.0);
}

function(MCGLUniform, viewUniform, voida)
{
    as(MCSkysphereCamera);
    MCGLUniform uniform;
    uniform.type = MCGLUniformMat4;
    uniform.data.mat4 = cpt(viewMatrix);
    return uniform;
}

function(MCGLUniform, projectionUniform, voida)
{
    as(MCSkysphereCamera);
    MCGLUniform uniform;
    uniform.type = MCGLUniformMat4;
    uniform.data.mat4 = cpt(projectionMatrix);
    return uniform;
}

method(MCSkysphereCamera, MCSkysphereCamera*, initWithWidthHeightRatio, MCFloat ratio)
{
    //setting camera
    sobj->ratio = ratio.f;
    return obj;
}

method(MCSkysphereCamera, void, move, MCFloat deltaFai, MCFloat deltaTht)
{
    MCCamera_move(0, sobj, deltaFai, deltaTht);
}

method(MCSkysphereCamera, void, update, MCGLContext* ctx)
{
    //change value
    MCGLContext_activateShaderProgram(0, ctx, 0);
    
    MCGLUniform uv = viewUniform(0, obj, 0);
    MCGLUniform up = projectionUniform(0, obj, 0);
    
    MCGLContext_updateUniform(0, ctx, "sphViewMatrix", uv.data);
    MCGLContext_updateUniform(0, ctx, "sphProjectionMatrix", up.data);
    
    MCGLContext_setUniforms(0, ctx, 0);
}

method(MCSkysphereCamera, void, setRotationMat3, float mat3[9])
{
    MCCamera_setRotationMat3(0, sobj, mat3);
}

onload(MCSkysphereCamera)
{
    if (load(MCCamera)) {
        binding(MCSkysphereCamera, MCSkysphereCamera*, initWithWidthHeightRatio, MCFloat ratio);
        binding(MCSkysphereCamera, void, move, MCFloat deltaFai, MCFloat deltaTht);
        binding(MCSkysphereCamera, void, update, MCGLContext* ctx);
        binding(MCSkysphereCamera, void, setRotationMat3, float mat3[9]);
        
        return cla;
    }else{
        return null;
    }
}
