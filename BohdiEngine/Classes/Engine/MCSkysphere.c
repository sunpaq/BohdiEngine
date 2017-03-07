//
//  MCSkysphere.c
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#include "MCSkysphere.h"
#include "MCGLEngine.h"
#include "MC3DShapeBase.h"

oninit(MCSkysphere)
{
    if (init(MC3DNode)) {
        var(vaoid) = 0;
        var(vboid) = 0;
        var(eboid) = 0;
        var(texid) = 0;
        
        var(camera) = new(MCSkysphereCamera);
        var(ctx)    = new(MCGLContext);
        
        int nr = 72;
        int nc = 144;
        
        var(vertices_size) = sizeof(GLfloat) * nr * nc * 5;
        var(indices_size)  = sizeof(GLuint) * nr * nc * 2;
        
        var(vertices) = (GLfloat*)malloc(obj->vertices_size);
        var(indices)  = (GLuint*)malloc(obj->indices_size);
        var(ic) = MCGenerateSkysphere(nr, nc, 100.0, var(vertices), var(indices));
        
        MCGLEngine_setClearScreenColor((MCColorf){0.05, 0.25, 0.35, 1.0});
        MCGLEngine_featureSwith(MCGLCullFace, true);
        MCGLEngine_cullFace(MCGLBack);
        MCGLEngine_setFrontCounterClockWise(true);//CW
        return obj;
    }else{
        return null;
    }
}

method(MCSkysphere, void, bye, voida)
{
    release(var(camera));
    release(var(ctx));
    release(var(vertices));
    release(var(indices));
    
    MC3DNode_bye(0, sobj, 0);
}

method(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex, double widthHeightRatio)
{
    //Shader
    MCGLContext_initWithShaderName(0, var(ctx), "MCSkysphereShader.vsh", "MCSkysphereShader.fsh",
                                   (const char* []){
                                       "position",
                                       "texcoord"
                                   }, 2,
                                   (MCGLUniformType []){
                                       MCGLUniformMat4,
                                       MCGLUniformMat4,
                                       MCGLUniformScalar
                                   },
                                   (const char* []){
                                       "sphViewMatrix",
                                       "sphProjectionMatrix",
                                       "sampler"
                                   }, 3);
    //Camera
    MCSkysphereCamera_initWithWidthHeightRatio(0, var(camera), (MCFloat)widthHeightRatio);
    
    //Mesh & Texture
    MCUInt buffers[3];
    glGenVertexArrays(1, &var(vaoid));
    glGenBuffers(3, buffers);
    var(vboid) = buffers[0];
    var(eboid) = buffers[1];
    var(texid) = buffers[2];
    //VAO
    glBindVertexArray(var(vaoid));
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, var(vboid));
    glBufferData(GL_ARRAY_BUFFER, obj->vertices_size, obj->vertices, GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, var(eboid));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indices_size, obj->indices, GL_STATIC_DRAW);
    //VAttributes
    MCVertexAttribute attr1 = (MCVertexAttribute){0, 3, GL_FLOAT, GL_FALSE, 20, MCBUFFER_OFFSET(0)};
    MCVertexAttributeLoad(&attr1);
    MCVertexAttribute attr2 = (MCVertexAttribute){1, 2, GL_FLOAT, GL_FALSE, 20, MCBUFFER_OFFSET(12)};
    MCVertexAttributeLoad(&attr2);
    //Texture
    MCGLEngine_activeTextureUnit(0);
    MCGLEngine_bind2DTexture(var(texid));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->raw);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Unbind
    glBindVertexArray(0);
    
    return obj;
}

method(MCSkysphere, MCSkysphere*, initWithFileName, const char* name, double widthHeightRatio)
{
    BE2DTextureData* data = BE2DTextureData_newWithFilename(name);
    MCSkysphere* sph = MCSkysphere_initWithBE2DTexture(0, obj, data, widthHeightRatio);
    release(data);
    return sph;
}

method(MCSkysphere, MCSkysphere*, initWithDefaultFileRatio, double widthHeightRatio)
{
    return MCSkysphere_initWithFileName(0, obj, "skysphtex.jpg", widthHeightRatio);
}

method(MCSkysphere, MCSkysphere*, initWithDefaultFile, voida)
{
    return MCSkysphere_initWithFileName(0, obj, "skysphtex.jpg", MCRatioHDTV16x9);
}

method(MCSkysphere, void, resizeWithWidthHeight, unsigned width, unsigned height)
{
    //superof(var(camera))->ratio = MCRatioMake(width, height);
}

//override
method(MCSkysphere, void, update, MCGLContext* ctx)
{
    obj->sphViewMatrix = var(camera)->viewMatrix(var(camera));
    obj->sphProjectionMatrix = var(camera)->projectionMatrix(var(camera));
    
    if (obj->sphCameraRatio != superof(obj->camera)->ratio) {
        MCGLContext_activateShaderProgram(0, var(ctx), 0);
        
        MCGLUniformData data;
        data.mat4 = obj->sphProjectionMatrix;
        MCGLContext_updateUniform(0, var(ctx), "sphProjectionMatrix", data);
        obj->sphCameraRatio = superof(var(camera))->ratio;
    }
}

method(MCSkysphere, void, draw, MCGLContext* ctx)
{
    glDepthMask(GL_FALSE);
    MCGLContext_activateShaderProgram(0, var(ctx), 0);
    MCGLUniformData data;
    data.mat4 = obj->sphViewMatrix;
    MCGLContext_updateUniform(0, var(ctx), "sphViewMatrix", data);
    MCGLContext_setUniforms(0, var(ctx), 0);
    
    glBindVertexArray(obj->vaoid);
    MCGLEngine_activeTextureUnit(0);
    glDrawElements(GL_TRIANGLE_STRIP, var(ic), GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

method(MCSkysphere, void, setRotationMat3, float mat3[9])
{
    MCSkysphereCamera_setRotationMat3(0, obj->camera, mat3);
}

onload(MCSkysphere)
{
    if (load(MC3DNode)) {
        binding(MCSkysphere, void, bye, voida);
        binding(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithFileName, const char* name, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithDefaultFileRatio, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithDefaultFile, voida);
        binding(MCSkysphere, void, resizeWithWidthHeight, unsigned width, unsigned height);
        binding(MCSkysphere, void, setRotationMat3, float mat3[9]);
        //override
        binding(MCSkysphere, void, update, MCGLContext* ctx);
        binding(MCSkysphere, void, draw, MCGLContext* ctx);
        
        return cla;
    }else{
        return null;
    }
}
