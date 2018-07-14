//
//  MCGLShader.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLRenderer.h"
#include "MCGLDefaultShader.h"
#include "MCGLContext.h"
#include "MC3DBase.h"
#include "MCCamera.h"
#include "MCLight.h"

oninit(MCGLRenderer)
{
    if(init(MCObject)){
        var(drawMode) = MCDrawNone;

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

        return obj;
    }else{
        return null;
    }
}

method(MCGLRenderer, void, bye, voida)
{
    release(obj->context);
    superbye(MCObject);
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode)
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
            MCGLUniformVec1,
            
            MCGLUniformScalar,
            MCGLUniformScalar
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
            material_shininess,
            
            diffuse_sampler,
            specular_sampler
        }, 17);
    return obj;
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader)
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

method(MCGLRenderer, MCGLRenderer*, initWithDefaultShader, voida)
{
    return MCGLRenderer_initWithShaderCodeString(obj, VCODE, FCODE);
}

function(void, drawMesh, MCGLMesh* mesh)
{
    as(MCGLRenderer);
    glBindVertexArray(mesh->VAO);
    //override draw mode
    GLenum mode = mesh->mode;
    if (obj->drawMode != MCDrawNone) {
        mode = obj->drawMode;
    }
    //draw
    if (mode != MCDrawNone) {
        if (mesh->vertexIndexes != null) {
            glDrawElements(mode, 100, GL_UNSIGNED_INT, (GLvoid*)0);
        }else{
            glDrawArrays(mode, 0, mesh->vertexCount);
        }
    }
    //Unbind
    glBindVertexArray(0);
    MCGLContext_unbind2DTextures(0);
}

function(void, drawNode, MC3DNode* node)
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
        
        MCMatrix3 nor = MCMatrix3InvertAndTranspose(MCMatrix4GetMatrix3(node->transform), NULL);
        f.data.mat3 = nor;
        MCGLShader_updateUniform(ctx->shader, model_normal, f.data);
        
        //material
        if (node->material != null) {
            if (node->material->hidden == 1) {
                return;
            }
            node->material->dataChanged = true;
            MCGLContext_loadMaterial(ctx, node->material);
        }
        
        //draw self texture
        if (node->diffuseTexture != null) {
            MCGLShader_shaderSetBool(ctx->shader, "usetexture", true);
        } else {
            MCGLShader_shaderSetBool(ctx->shader, "usetexture", false);
        }
        
        //batch setup
        MCGLShader_setUniforms(ctx->shader, 0);
        
        //draw self meshes
        MCLinkedListForEach(node->meshes,
                            MCGLMesh* mesh = (MCGLMesh*)item;
                            if (mesh != null) {
                                //texture
                                if (node->diffuseTexture) {
                                    MCGLContext_loadTexture(ctx, node->diffuseTexture, "diffuse_sampler");
                                }
                                if (node->specularTexture) {
                                    MCGLContext_loadTexture(ctx, node->specularTexture, "specular_sampler");
                                }
                                MCGLContext_loadMesh(ctx, mesh);
                                drawMesh(obj, mesh);
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

function(void, updateCamera, MCCamera* cam)
{
    as(MCGLRenderer);
    MCGLUniformData data;
    MCGLShader* shader = obj->context->shader;
    data.mat4 = computed(cam, viewMatrix);
    MCGLShader_updateUniform(shader, view_view, data);
    data.mat4 = computed(cam, projectionMatrix);
    MCGLShader_updateUniform(shader, view_projection, data);
}

function(void, updateLight, MCLight* light)
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

method(MCGLRenderer, void, updateScene, MC3DScene* scene)
{
    if(computed(scene, isDrawSky)) {
        //no model
        if (scene->combineMode == MC3DSceneSkyboxOnly) {
            MCGLSkybox_update(scene->skybox, var(context));
            return;
        }
        else if (scene->combineMode == MC3DSceneSkysphOnly) {
            MCGLSkysphere_update(scene->skysph, var(context));
            return;
        }
        //with model
        else if (scene->combineMode == MC3DSceneModelWithSkybox) {
            MCGLSkybox_update(scene->skybox, var(context));
        }
        else if (scene->combineMode == MC3DSceneModelWithSkysph) {
            MCGLSkysphere_update(scene->skysph, var(context));
        }
    }
    if (scene->cameraAutoRotate) {
        MC3DScene_moveCameraOneStep(scene, MCFloatF(0.5), MCFloatF(0.0));
    }
    //MCCamera_update(scene->mainCamera, obj->context);
    //MCLight_update(scene->light, obj->context);
}

method(MCGLRenderer, void, drawScene, MC3DScene* scene)
{
    MCGLContext_clearScreenWithColor(scene->bgcolor);
    //MCGLContext_clearScreen(0);
    if (scene->isDrawSky) {
        //no model
        if (scene->combineMode == MC3DSceneSkyboxOnly) {
            MCGLSkybox_draw(scene->skybox, var(context));
            //return MCGLContext_tickFPS(var(clock));
        }
        else if (scene->combineMode == MC3DSceneSkysphOnly) {
            MCGLSkysphere_draw(scene->skysph, var(context));
            //return MCGLContext_tickFPS(var(clock));
        }
        //with model
        else if (scene->combineMode == MC3DSceneModelWithSkybox) {
            MCGLSkybox_draw(scene->skybox, var(context));
        }
        else if (scene->combineMode == MC3DSceneModelWithSkysph) {
            MCGLSkysphere_draw(scene->skysph, var(context));
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

onload(MCGLRenderer)
{
    if (load(MCObject)) {
        //prehash();
        binding(MCGLRenderer, void, bye, voida);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
        //binding(MCGLRenderer, void, updateNodes, MC3DNode* rootnode);
        //binding(MCGLRenderer, void, drawNodes, MC3DNode* rootnode);
        return cla;
    }else{
        return null;
    }
}



