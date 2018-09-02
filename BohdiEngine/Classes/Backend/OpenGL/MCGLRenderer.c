//
//  MCGLShader.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLRenderer.h"
#include "MCGLDefaultShader.h"
#include "MCGLSkyboxShader.h"
#include "MCGLSkysphereShader.h"

#include "beengine_export.h"

oninit(MCGLRenderer)
{
    if(init(MCObject)){
        var(drawMode) = GL_TRIANGLES;
        var(useage) = GL_STATIC_DRAW;

        MCGLContext_featureSwith(MCGLDepthTest, true);
        MCGLContext_featureSwith(MCGLStencilTest, true);
        MCGLContext_featureSwith(MCGLCullFace, true);

        MCGLContext_cullFace(MCGLBack);
        MCGLContext_setFrontCounterClockWise(true);//CCW

        glDepthFunc(GL_LESS);
        
        // Enable blending
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        obj->context = new(MCGLContext);
        obj->skycontext = null;

        return obj;
    }else{
        return null;
    }
}

ifun(void, initSkybox, voida)
{
    as(MCGLRenderer);
    if (obj->skycontext == null) {
        obj->skycontext = new(MCGLContext);
        MCGLShader_initWithShaderCode(obj->skycontext->shader, MCSkybox_vsource, MCSkybox_fsource,
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
    }
}

ifun(void, initSkysphere, voida)
{
    as(MCGLRenderer);
    if (obj->skycontext == null) {
        obj->skycontext = new(MCGLContext);
        MCGLShader_initWithShaderCode(obj->skycontext->shader, MCSkysphere_vsource, MCSkysphere_fsource,
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
    }
}

fun(MCGLRenderer, void, bye, voida)
{
    release(obj->context);
    release(obj->skycontext);
    superbye(MCObject);
}

fun(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode)
{
    MCGLShader_initWithShaderCode(obj->context->shader, vcode, fcode,
        (const char* []){
            "position",
            "normal",
            "color",
            "texcoord"
        }, 4,
        (MCGLUniformType []){
            MCGLUniformMat4,
            MCGLUniformMat4,
            MCGLUniformVec3,
            
            MCGLUniformMat4,
            MCGLUniformMat3,
            
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,

            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec1,
            MCGLUniformVec1
        },
        (const char* []){
            view_view,
            view_projection,
            view_position,
            
            model_model,
            model_normal,
            
            light_ambient,
            light_diffuse,
            light_specular,
            light_color,
            light_position,
            
            material_ambient,
            material_diffuse,
            material_specular,
            material_dissolve,
            material_shininess
        }, 15);
    return obj;
}

fun(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader)
{
    char path[LINE_MAX];
    MCFileGetPath(vshader, path);
    const char* vcode = MCFileCopyContentWithPath(path);
    
    MCFileGetPath(fshader, path);
    const char* fcode = MCFileCopyContentWithPath(path);
    
    MCGLRenderer_initWithShaderCodeString(obj, vcode, fcode);
    
    free((void*)vcode);
    free((void*)fcode);
    return obj;
}

fun(MCGLRenderer, MCGLRenderer*, initWithDefaultShader, voida)
{
    return MCGLRenderer_initWithShaderCodeString(obj, MCGLDefault_vsource, MCGLDefault_fsource);
}

ifun(void, drawMesh, MCMesh* mesh, GLuint texid)
{
    as(MCGLRenderer);
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //VAO
    glBindVertexArray(VAO);
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertexDataSize, mesh->vertexDataPtr, obj->useage);
    //EBO
    if (mesh->vertexIndexes != null) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mesh->vertexCount, mesh->vertexIndexes, obj->useage);
    }
    //VAttributes
    MCVertexAttribute attr1 = {MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(0)};
    MCVertexAttribute attr2 = {MCVertexAttribNormal,   3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(12)};
    MCVertexAttribute attr3 = {MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(24)};
    MCVertexAttribute attr4 = {MCVertexAttribTexCoord0,2, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(36)};
    MCVertexAttributeLoad(&attr1);
    MCVertexAttributeLoad(&attr2);
    MCVertexAttributeLoad(&attr3);
    MCVertexAttributeLoad(&attr4);
    //Texture
    if (texid > 0) {
        //MCGLContext_bind2DTexture(texid);
    }
    //draw
    if (obj->drawMode != -1) {
        if (mesh->vertexIndexes != null) {
            glDrawElements(obj->drawMode, 100, GL_UNSIGNED_INT, (GLvoid*)0);
        }else{
            glDrawArrays(obj->drawMode, 0, mesh->vertexCount);
        }
    }
    //commit
    glFlush();
    //unbind
    MCGLContext_unbind2DTextures(0);
    //delete
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

ifun(void, drawNode, MC3DNode* node)
{
    as(MCGLRenderer);
    //callback
    if (node->receiveEvent) {
        ff(node, willDraw, 0);
    }
    
    if (node->overrideDraw) {
        ff(node, draw, 0);
    } else {
        //draw self
        MCGLContext* ctx = obj->context;
        MCGLShader* shader = ctx->shader;
        
        MCGLShader_activateShaderProgram(shader, 0);
        MCGLUniform f;
        
        //scale translate
        MCMatrix4 viewModel = MCMatrix4Multiply(node->viewtrans, node->transform);
        
        if (!MCMatrix4Equal(&MCMatrix4Identity, &viewModel)) {
            f.data.mat4 = viewModel;
            MCGLShader_updateUniform(shader, model_model, f.data);
        }
        
//        MCMatrix3 nor = MCMatrix3InvertAndTranspose(MCMatrix4GetMatrix3(node->transform), NULL);
//        f.data.mat3 = nor;
//        MCGLShader_updateUniform(ctx->shader, model_normal, f.data);
        
        //material
        if (node->material != null) {
            if (node->material->hidden == 1) {
                return;
            }
            node->material->dataChanged = true;
            MCGLContext_loadMaterial(ctx, node->material);
        }
        
        //batch setup
        MCGLShader_setUniforms(ctx->shader, 0);
        
        //texture
        MCGLShader_shaderSetInt(ctx->shader, "usetexture", 0);
        GLuint texid = 0;
        if (node->diffuseTexture) {
            texid = MCGLContext_loadTexture(ctx, node->diffuseTexture, "diffuse_sampler", 0);
            MCGLShader_shaderSetInt(ctx->shader, "usetexture", 1);
        }
        if (node->specularTexture) {
            texid = MCGLContext_loadTexture(ctx, node->specularTexture, "specular_sampler", 1);
            MCGLShader_shaderSetInt(ctx->shader, "usetexture", 2);
        }
        if (node->normalTexture) {
            texid = MCGLContext_loadTexture(ctx, node->normalTexture, "normal_sampler", 2);
            MCGLShader_shaderSetInt(ctx->shader, "usetexture", 3);
        }

        //draw self meshes
        MCLinkedListForEach(node->meshes,
                            MCMesh* mesh = (MCMesh*)item;
                            if (mesh != null) {
                                drawMesh(obj, mesh, texid);
                            })
    }
    
    //callback
    if (node->receiveEvent) {
        ff(node, didDraw, obj);
    }
    
    //draw children
    MCLinkedListForEach(node->children,
                        MC3DNode* child = (MC3DNode*)item;
                        if (child != null && child->visible != false) {
                            drawNode(obj, child);
                        })
    
    //ff(ctx, printUniforms, 0);
}

ifun(void, updateCamera, MCCamera* cam)
{
    as(MCGLRenderer);
    MCGLUniformData data;
    MCGLShader* shader = obj->context->shader;
    data.mat4 = computed(cam, viewMatrix);
    MCGLShader_updateUniform(shader, view_view, data);
    data.mat4 = computed(cam, projectionMatrix);
    MCGLShader_updateUniform(shader, view_projection, data);
}

ifun(void, updateLight, MCLight* light)
{
    as(MCGLRenderer);
    if (light->dataChanged == true) {
        MCGLShader* shader = obj->context->shader;

        MCGLShader_activateShaderProgram(shader, 0);
        MCGLUniformData data;

        data.vec3 = light->ambientLightStrength;
        MCGLShader_updateUniform(shader, light_ambient, data);

        data.vec3 = light->diffuseLightStrength;
        MCGLShader_updateUniform(shader, light_diffuse, data);

        data.vec3 = light->specularLightStrength;
        MCGLShader_updateUniform(shader, light_specular, data);

        data.vec3 = light->lightColor;
        MCGLShader_updateUniform(shader, light_color, data);

        data.vec3 = light->lightPosition;
        MCGLShader_updateUniform(shader, light_position, data);

        light->dataChanged = false;
    }
}

ifun(void, drawSkybox, MCSkybox* skybox)
{
    as(MCGLRenderer);
    //init
    MCGLShader* shader = obj->context->shader;
    MCUInt vaoid;
    MCUInt vboid;
    MCUInt eboid;
    MCUInt texid;
    //Mesh & Texture
    MCUInt buffers[3];
    glGenVertexArrays(1, &vaoid);
    glGenBuffers(3, buffers);
    vboid = buffers[0];
    eboid = buffers[1];
    texid = buffers[2];
    //VAO
    glBindVertexArray(vaoid);
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox->skyboxVertices), skybox->skyboxVertices, GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skybox->indexs), skybox->indexs, GL_STATIC_DRAW);
    //VAttributes
    MCVertexAttribute attr = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE,
        sizeof(GLfloat) * 3, MCBUFFER_OFFSET(0)};
    MCVertexAttributeLoad(&attr);
    //Texture
    MCGLContext_activeTextureUnit(0);
    MCGLContext_bindCubeTexture(texid);
    for (int i=0; i<6; i++) {
        BE2DTextureData* face = skybox->cubedata->faces[i];
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
    //update
    MCGLShader_activateShaderProgram(shader, 0);
    MCGLUniformData data;
    MCSkybox_getViewMatrix(skybox, &data.mat4);
    MCGLShader_updateUniform(shader, "boxProjectionMatrix", data);
    //draw
    glDepthMask(GL_FALSE);
    MCGLShader_activateShaderProgram(shader, 0);
    MCSkybox_getProjectionMatrix(skybox, &data.mat4);
    MCGLShader_updateUniform(shader, "boxViewMatrix", data);
    MCGLShader_setUniforms(shader, 0);
    
    glBindVertexArray(vaoid);
    MCGLContext_activeTextureUnit(0);
    //MCGLContext_bindCubeTexture(obj->texid);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    
    glDeleteVertexArrays(1, &vaoid);
    glDeleteBuffers(3, buffers);
}

ifun(void, drawSkysphere, MCSkysphere* sphere)
{
    as(MCGLRenderer);
    MCGLShader* shader = obj->context->shader;
    MCUInt vaoid;
    MCUInt vboid;
    MCUInt eboid;
    MCUInt texid;
    MCUInt ic;
    //Mesh & Texture
    MCUInt buffers[4];
    glGenVertexArrays(1, &vaoid);
    glGenBuffers(3, buffers);
    vboid = buffers[0];
    eboid = buffers[1];
    texid = buffers[2];
    ic = buffers[3];
    //VAO
    glBindVertexArray(vaoid);
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboid);
    glBufferData(GL_ARRAY_BUFFER, sphere->vertices_size, sphere->vertices, GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere->indices_size, sphere->indices, GL_STATIC_DRAW);
    //VAttributes
    MCVertexAttribute attr1 = (MCVertexAttribute){0, 3, GL_FLOAT, GL_FALSE, 20, MCBUFFER_OFFSET(0)};
    MCVertexAttributeLoad(&attr1);
    MCVertexAttribute attr2 = (MCVertexAttribute){1, 2, GL_FLOAT, GL_FALSE, 20, MCBUFFER_OFFSET(12)};
    MCVertexAttributeLoad(&attr2);
    //Texture
    MCGLContext_activeTextureUnit(0);
    MCGLContext_bind2DTexture(texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sphere->tex->width, sphere->tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, sphere->tex->raw);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Unbind
    glBindVertexArray(0);
    //Shader
    MCGLShader_activateShaderProgram(shader, 0);
    MCGLUniformData data;
    MCSkysphere_getProjectionMatrix(sphere, &data.mat4);
    MCGLShader_updateUniform(shader, "sphProjectionMatrix", data);
    
    glDepthMask(GL_FALSE);
    MCGLShader_activateShaderProgram(shader, 0);
    MCSkysphere_getViewMatrix(sphere, &data.mat4);
    MCGLShader_updateUniform(shader, "sphViewMatrix", data);
    MCGLShader_setUniforms(shader, 0);
    
    glBindVertexArray(vaoid);
    MCGLContext_activeTextureUnit(0);
    glDrawElements(GL_TRIANGLE_STRIP, ic, GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    
    glDeleteVertexArrays(1, &vaoid);
    glDeleteBuffers(4, buffers);
}

ifun(void, updateScene, MC3DScene* scene)
{
    if (scene->cameraAutoRotate) {
        MC3DScene_moveCameraOneStep(scene, MCFloatF(0.5), MCFloatF(0.0));
    }
}

ifun(void, drawScene, MC3DScene* scene)
{
    as(MCGLRenderer);
    MCGLContext_clearScreenWithColor(scene->bgcolor);
    //MCGLContext_clearScreen(0);
    if (scene->isDrawSky) {
        //no model
        if (scene->combineMode == MC3DSceneSkyboxOnly) {
            initSkybox(obj, scene->skybox);
            drawSkybox(obj, scene->skybox);
            //return MCGLContext_tickFPS(var(clock));
        }
        else if (scene->combineMode == MC3DSceneSkysphOnly) {
            initSkysphere(obj, scene->skysph);
            drawSkysphere(obj, scene->skysph);
            //return MCGLContext_tickFPS(var(clock));
        }
        //with model
        else if (scene->combineMode == MC3DSceneModelWithSkybox) {
            initSkybox(obj, scene->skybox);
            drawSkybox(obj, scene->skybox);
        }
        else if (scene->combineMode == MC3DSceneModelWithSkysph) {
            initSkysphere(obj, scene->skysph);
            drawSkysphere(obj, scene->skysph);
        }
    }
    
    //Camera MVP Matrix
    MCCamera* cam = scene->mainCamera;
    if (cam) {
        updateCamera(obj, cam);
    }
    
    //Light
    MCLight* light = scene->light;
    if (light) {
        updateLight(obj, light);
    }
    
    drawNode(obj, scene->rootnode);
}

ifun(MCDrawMode, getDrawMode, voida)
{
    as(MCGLRenderer);
    return (MCDrawMode)obj->drawMode;
}

ifun(void, setDrawMode, MCDrawMode mode)
{
    as(MCGLRenderer);
    obj->drawMode = mode;
}

ifun(void, scissorAllScene, int x, int y, int width, int height)
{
    MCGLContext_setViewport(x, y, width, height);
    MCGLContext_setScissor(x, y, width, height);
}

onload(MCGLRenderer)
{
    if (load(MCObject)) {
        #include "../../Engine/MCRenderer.p"
        //life cycle
        bid(MCGLRenderer, void, bye, voida);
        bid(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
        bid(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
        bid(MCGLRenderer, MCGLRenderer*, initWithDefaultShader, voida);
        return cla;
    }else{
        return null;
    }
}



