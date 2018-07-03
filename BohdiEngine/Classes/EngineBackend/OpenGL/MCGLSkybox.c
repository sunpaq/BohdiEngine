//
//  MCGLSkybox.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCGLSkybox.h"
#include "MCGLContext.h"
#include "MCCamera.h"

static const char* vsource = S(precision highp float;
                               precision mediump int;
                               
                               layout (location=0) in vec3 position;
                               
                               uniform mat4 boxViewMatrix;
                               uniform mat4 boxProjectionMatrix;
                               out vec3 TexCoords;
                               
                               void main() {
                                   gl_Position = boxProjectionMatrix * boxViewMatrix * vec4(position, 1.0);
                                   TexCoords = position;
                               }
);//end vsource

static const char* fsource = S(precision highp sampler3D;
                               precision highp float;
                               precision lowp int;
                               const float Epsilon = 0.0000001;
                               
                               in vec3 TexCoords;
                               uniform samplerCube cubeSampler;
                               
                               out vec4 color;
                               
                               void main() {
                                   color = texture(cubeSampler, TexCoords);
                               }
);//end fsource

static GLfloat skyboxVertices[] = {
    -1.0f, -1.0f, -1.0f, //000 0
    -1.0f, -1.0f,  1.0f, //001 1
    -1.0f,  1.0f, -1.0f, //010 2
    -1.0f,  1.0f,  1.0f, //011 3
    1.0f, -1.0f, -1.0f,  //100 4
    1.0f, -1.0f,  1.0f,  //101 5
    1.0f,  1.0f, -1.0f,  //110 6
    1.0f,  1.0f,  1.0f,  //111 7
};

static GLuint indexs[] = {
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
        var(vaoid) = 0;
        var(vboid) = 0;
        var(texid) = 0;
        
        var(ctx) = new(MCGLContext);
        
        var(boxProjectionMatrix) = MCMatrix4Identity;
        var(boxViewMatrix) = MCMatrix4Identity;
        var(boxCameraRatio)= 9.0/16.0;
        var(boxCameraAngle)= M_PI * 0.55;
        
        MCGLContext_setClearScreenColor((MCColorf){0.05, 0.25, 0.35, 1.0});
        MCGLContext_featureSwith(MCGLCullFace, true);
        MCGLContext_cullFace(MCGLBack);
        MCGLContext_setFrontCounterClockWise(true);//CCW
        return obj;
    }else{
        return null;
    }
}

method(MCGLSkybox, void, bye, voida)
{
    release(var(ctx));
    MC3DNode_bye(sobj, 0);
}

method(MCGLSkybox, MCGLSkybox*, initWithCubeTexture, BECubeTextureData* cubetex)
{
    //Shader
    MCGLShader_initWithShaderCode(var(ctx)->shader, vsource, fsource,
                                   (const char* []){
                                       "position"
                                   }, 1,
                                   (MCGLUniformType []){
                                       MCGLUniformMat4,
                                       MCGLUniformMat4,
                                       MCGLUniformScalar
                                   },
                                   (const char* []){
                                       "boxViewMatrix",
                                       "boxProjectionMatrix",
                                       "cubeSampler"
                                   }, 3);
    
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, var(eboid));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
    //VAttributes
    MCVertexAttribute attr = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE,
        sizeof(GLfloat) * 3, MCBUFFER_OFFSET(0)};
    MCVertexAttributeLoad(&attr);
    //Texture
    MCGLContext_activeTextureUnit(0);
    MCGLContext_bindCubeTexture(var(texid));
    for (int i=0; i<6; i++) {
        BE2DTextureData* face = cubetex->faces[i];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     GL_RGB, face->width, face->height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, face->raw);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Unbind
    glBindVertexArray(0);
    
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

function(MCMatrix4, boxViewMatrix, voida)
{
    as(MCGLSkybox);
    MCMatrix4 m = MCMatrix4MakeLookAt(0, 0, 0,
                                      0, 0,-1,
                                      0, 1, 0);
    MCMatrix4 imat4 = MCMatrix4Invert(obj->Super.transform, null);
    
    return MCMatrix4Multiply(m, imat4);
}

function(MCMatrix4, boxProjectionMatrix, voida)
{
    as(MCGLSkybox);
    return MCMatrix4MakePerspective(obj->boxCameraAngle,
                                    obj->boxCameraRatio,
                                    0.001,
                                    200.0);
}

method(MCGLSkybox, void, setRotationMat3, float mat3[9])
{
    MC3DNode_rotateMat3(sobj, mat3, false);
}

method(MCGLSkybox, void, setRotationMat4, float mat4[16])
{
    MC3DNode_rotateMat4(sobj, mat4, false);
}

method(MCGLSkybox, void, update, MCGLContext* ctx)
{
    obj->boxViewMatrix = boxViewMatrix(obj, 0);
    obj->boxProjectionMatrix = boxProjectionMatrix(obj, 0);
    
    MCGLShader_activateShaderProgram(var(ctx)->shader, 0);
    MCGLUniformData data;
    data.mat4 = obj->boxProjectionMatrix;
    MCGLShader_updateUniform(var(ctx)->shader, "boxProjectionMatrix", data);
}

method(MCGLSkybox, void, draw, MCGLContext* ctx)
{
    glDepthMask(GL_FALSE);
    MCGLShader_activateShaderProgram(var(ctx)->shader, 0);
    MCGLUniformData data;
    data.mat4 = obj->boxViewMatrix;
    MCGLShader_updateUniform(var(ctx)->shader, "boxViewMatrix", data);
    MCGLShader_setUniforms(var(ctx)->shader, 0);
    
    glBindVertexArray(obj->vaoid);
    MCGLContext_activeTextureUnit(0);
    //MCGLContext_bindCubeTexture(obj->texid);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

onload(MCGLSkybox)
{
    if (load(MC3DNode)) {
        binding(MCGLSkybox, void, bye, voida);
        binding(MCGLSkybox, MCGLSkybox*, initWithCubeTexture, BECubeTextureData* cubetex);
        binding(MCGLSkybox, MCGLSkybox*, initWithFileNames, const char* namelist[]);
        binding(MCGLSkybox, MCGLSkybox*, initWithDefaultFiles, voida);
        //binding(MCGLSkybox, void, resizeWithWidthHeight, unsigned width, unsigned height);
        //binding(MCGLSkybox, void, setWidthHeightRatio, double widthHeightRatio);
        binding(MCGLSkybox, void, setRotationMat3, float mat3[9]);
        binding(MCGLSkybox, void, setRotationMat4, float mat4[16]);
        binding(MCGLSkybox, void, update, MCGLContext* ctx);
        binding(MCGLSkybox, void, draw, MCGLContext* ctx);
        return cla;
    }else{
        return null;
    }
}
