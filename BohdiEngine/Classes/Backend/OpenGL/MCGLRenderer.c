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
#include "MCGLShader.h"
#include "MCMesh.h"
#include "BEAssetsManager.h"
#include "MC3DNode.h"
#include "MCCamera.h"
#include "MCLight.h"
#include "MCSkybox.h"
#include "MCSkysphere.h"
#include "MCGLContext.h"
#include "MC3DScene.h"
#include "MCObject.h"

fun(initSkybox, void)) as(MCGLRenderer)
    if (it->skycontext == null) {
        it->skycontext = new(MCGLContext);
        it->skycontext->shader->initWithShaderCode(it->skycontext->shader, MCSkybox_vsource, MCSkybox_fsource,
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

fun(initSkysphere, void)) as(MCGLRenderer);
    if (it->skycontext == null) {
        it->skycontext = new(MCGLContext);
        it->skycontext->shader->initWithShaderCode(it->skycontext->shader, MCSkysphere_vsource, MCSkysphere_fsource,
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

fun(release, void)) as(MCGLRenderer)
    Release(it->context);
    Release(it->skycontext);
    cast(it, MCObject)->release(it);
    //superbye(MCObject);
}

fun(initWithShaderCodeString, struct MCGLRenderer*), const char* vcode, const char* fcode) as(MCGLRenderer)
    it->context->shader->initWithShaderCode(it->context->shader, vcode, fcode,
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
    return it;
}

fun(initWithShaderFileName, struct MCGLRenderer*), const char* vshader, const char* fshader) as(MCGLRenderer)
    char path[LINE_MAX];
    MCFileGetPath(vshader, path);
    const char* vcode = MCFileCopyContentWithPath(path);
    
    MCFileGetPath(fshader, path);
    const char* fcode = MCFileCopyContentWithPath(path);
    
    it->initWithShaderCodeString(it, vcode, fcode);
    
    free((void*)vcode);
    free((void*)fcode);
    return it;
}

fun(initWithDefaultShader, struct MCGLRenderer*)) as(MCGLRenderer)
    return it->initWithShaderCodeString(it, MCGLDefault_vsource, MCGLDefault_fsource);
}

fun(drawMesh, void), struct MCMesh* mesh, GLuint texid) as(MCGLRenderer)
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //VAO
    glBindVertexArray(VAO);
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertexDataSize, mesh->vertexDataPtr, it->useage);
    //EBO
    if (mesh->vertexIndexes != null) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mesh->vertexCount, mesh->vertexIndexes, it->useage);
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
    if (it->drawMode != -1) {
        if (mesh->vertexIndexes != null) {
            glDrawElements(it->drawMode, 100, GL_UNSIGNED_INT, (GLvoid*)0);
        }else{
            glDrawArrays(it->drawMode, 0, mesh->vertexCount);
        }
    }
    //commit
    glFlush();
    //unbind
    MCGLContext_unbind2DTextures();
    //delete
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

fun(drawNode, void), struct MC3DNode* node) as(MCGLRenderer)
    //callback
    if (node->receiveEvent) {
        ff(node, willDraw));
    }
    
    if (node->overrideDraw) {
        ff(node, draw));
    } else {
        //draw self
        struct MCGLContext* ctx = it->context;
        struct MCGLShader* shader = ctx->shader;
        
        shader->activateShaderProgram(shader);
        MCGLUniform f;
        
        //scale translate
        MCMatrix4 viewModel = MCMatrix4Multiply(node->viewtrans, node->transform);
        
        if (!MCMatrix4Equal(&MCMatrix4Identity, &viewModel)) {
            f.data.mat4 = viewModel;
            shader->updateUniform(shader, model_model, f.data);
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
            ctx->loadMaterial(ctx, node->material);
        }
        
        //batch setup
        ctx->shader->setUniforms(ctx->shader);
        
        //texture
        ctx->shader->shaderSetInt(ctx->shader, "usetexture", 0);
        GLuint texid = 0;
        if (node->diffuseTexture) {
            texid = ctx->loadTexture(ctx, node->diffuseTexture, "diffuse_sampler", 0);
            ctx->shader->shaderSetInt(ctx->shader, "usetexture", 1);
        }
        if (node->specularTexture) {
            texid = ctx->loadTexture(ctx, node->specularTexture, "specular_sampler", 1);
            ctx->shader->shaderSetInt(ctx->shader, "usetexture", 2);
        }
        if (node->normalTexture) {
            texid = ctx->loadTexture(ctx, node->normalTexture, "normal_sampler", 2);
            ctx->shader->shaderSetInt(ctx->shader, "usetexture", 3);
        }

        //draw self meshes
        MCLinkedListForEach(node->meshes,
                            MCMesh_t* mesh = (MCMesh_t*)item;
                            if (mesh != null) {
                                drawMesh(it, mesh, texid);
                            })
    }
    
    //callback
    if (node->receiveEvent) {
        ff(node, didDraw), it);
    }
    
    //draw children
    MCLinkedListForEach(node->children,
                        MC3DNode_t* child = (MC3DNode_t*)item;
                        if (child != null && child->visible != false) {
                            drawNode(it, child);
                        })
    
    //ff(ctx, printUniforms, 0);
}

fun(updateCamera, void), struct MCCamera* cam) as(MCGLRenderer)
    MCGLUniformData data;
    struct MCGLShader* shader = it->context->shader;
    data.mat4 = computed(cam, viewMatrix);
    shader->updateUniform(shader, view_view, data);
    data.mat4 = computed(cam, projectionMatrix);
    shader->updateUniform(shader, view_projection, data);
}

fun(updateLight, void), struct MCLight* light) as(MCGLRenderer)
    if (light->dataChanged == true) {
        struct MCGLShader* shader = it->context->shader;

        shader->activateShaderProgram(shader);
        MCGLUniformData data;

        data.vec3 = light->ambientLightStrength;
        shader->updateUniform(shader, light_ambient, data);

        data.vec3 = light->diffuseLightStrength;
        shader->updateUniform(shader, light_diffuse, data);

        data.vec3 = light->specularLightStrength;
        shader->updateUniform(shader, light_specular, data);

        data.vec3 = light->lightColor;
        shader->updateUniform(shader, light_color, data);

        data.vec3 = light->lightPosition;
        shader->updateUniform(shader, light_position, data);

        light->dataChanged = false;
    }
}

fun(drawSkybox, void), struct MCSkybox* skybox) as(MCGLRenderer)
    //init
    struct MCGLShader* shader = it->context->shader;
    unsigned vaoid;
    unsigned vboid;
    unsigned eboid;
    unsigned texid;
    //Mesh & Texture
    unsigned buffers[3];
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
    int i;
    for (i=0; i<6; i++) {
        struct BE2DTextureData* face = skybox->cubedata->faces[i];
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
    shader->activateShaderProgram(shader);
    MCGLUniformData data;
    skybox->getViewMatrix(skybox, &data.mat4);
    shader->updateUniform(shader, "boxProjectionMatrix", data);
    //draw
    glDepthMask(GL_FALSE);
    shader->activateShaderProgram(shader);
    skybox->getProjectionMatrix(skybox, &data.mat4);
    shader->updateUniform(shader, "boxViewMatrix", data);
    shader->setUniforms(shader);
    
    glBindVertexArray(vaoid);
    MCGLContext_activeTextureUnit(0);
    //MCGLContext_bindCubeTexture(obj->texid);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    
    glDeleteVertexArrays(1, &vaoid);
    glDeleteBuffers(3, buffers);
}

fun(drawSkysphere, void), struct MCSkysphere* sphere) as(MCGLRenderer)
    struct MCGLShader* shader = it->context->shader;
    unsigned vaoid;
    unsigned vboid;
    unsigned eboid;
    unsigned texid;
    unsigned ic;
    //Mesh & Texture
    unsigned buffers[4];
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
    shader->activateShaderProgram(shader);
    MCGLUniformData data;
    sphere->getProjectionMatrix(sphere, &data.mat4);
    shader->updateUniform(shader, "sphProjectionMatrix", data);

    glDepthMask(GL_FALSE);
    shader->activateShaderProgram(shader);
    sphere->getViewMatrix(sphere, &data.mat4);
    shader->updateUniform(shader, "sphViewMatrix", data);
    shader->setUniforms(shader);
    
    glBindVertexArray(vaoid);
    MCGLContext_activeTextureUnit(0);
    glDrawElements(GL_TRIANGLE_STRIP, ic, GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    
    glDeleteVertexArrays(1, &vaoid);
    glDeleteBuffers(4, buffers);
}

fun(updateScene, void), struct MC3DScene* scene)
{
    if (scene->cameraAutoRotate) {
        scene->moveCameraOneStep(scene, 0.5, 0.0);
    }
}

fun(drawScene, void), struct MC3DScene* scene) as(MCGLRenderer)
    MCGLContext_clearScreenWithColor(scene->bgcolor);
    //MCGLContext_clearScreen(0);
    if (scene->isDrawSky) {
        //no model
        if (scene->combineMode == MC3DSceneSkyboxOnly) {
            initSkybox(it);
            //drawSkybox(it);
            //return MCGLContext_tickFPS(var(clock));
        }
        else if (scene->combineMode == MC3DSceneSkysphOnly) {
            initSkysphere(it);
            //drawSkysphere(it);
            //return MCGLContext_tickFPS(var(clock));
        }
        //with model
        else if (scene->combineMode == MC3DSceneModelWithSkybox) {
            initSkybox(it);
            //drawSkybox(it);
        }
        else if (scene->combineMode == MC3DSceneModelWithSkysph) {
            initSkysphere(it);
            //drawSkysphere(it);
        }
    }
    
    //Camera MVP Matrix
    struct MCCamera* cam = scene->mainCamera;
    if (cam) {
        updateCamera(it, cam);
    }
    
    //Light
    struct MCLight* light = scene->light;
    if (light) {
        updateLight(it, light);
    }
    
    drawNode(it, scene->rootnode);
}

fun(getDrawMode, MCDrawMode)) as(MCGLRenderer)
    return (MCDrawMode)it->drawMode;
}

fun(setDrawMode, void), MCDrawMode mode) as(MCGLRenderer)
    it->drawMode = mode;
}

fun(scissorAllScene, void), int x, int y, int width, int height)
{
    MCGLContext_setViewport(x, y, width, height);
    MCGLContext_setScissor(x, y, width, height);
}

constructor(MCGLRenderer)) {
    MCObject(any);
    as(MCGLRenderer)
        it->drawMode = GL_TRIANGLES;
        it->useage = GL_STATIC_DRAW;

        MCGLContext_featureSwith(MCGLDepthTest, true);
        MCGLContext_featureSwith(MCGLStencilTest, true);
        MCGLContext_featureSwith(MCGLCullFace, true);

        MCGLContext_cullFace(MCGLBack);
        MCGLContext_setFrontCounterClockWise(true);//CCW

        glDepthFunc(GL_LESS);

        // Enable blending
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        it->context = new(MCGLContext);
        it->skycontext = null;
    }
    dynamic(MCGLRenderer)
        funbind(release);
        funbind(initWithShaderCodeString);
        funbind(initWithShaderFileName);
        funbind(initWithDefaultShader);
        funbind(updateScene);
        funbind(drawScene);
    }
    return any;
}




