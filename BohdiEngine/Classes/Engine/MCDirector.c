//
//  MCDirector.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/19.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCDirector.h"
#include "MCThread.h"

compute(MCLight*, lightHandler)
{
    as(MCDirector);
    if (var(lastScene) != null && var(lastScene)->light != null) {
        return var(lastScene)->light;
    }
    return null;
}

compute(MCCamera*, cameraHandler)
{
    as(MCDirector);
    if (var(lastScene) != null && var(lastScene)->mainCamera != null) {
        return var(lastScene)->mainCamera;
    }
    return null;
}

compute(MCGLContext*, contextHandler)
{
    as(MCDirector);
    return var(lastScene)->renderer->context;
}

//compute(MCSkyboxCamera*, skyboxCameraHandler)
//{
//    as(MCDirector);
//    if (obj->lastScene) {
//        if (obj->lastScene->skyboxRef) {
//            return obj->lastScene->skyboxRef->camera;
//        }
//    }
//    return null;
//}

oninit(MCDirector)
{
    if (init(MCObject)) {
        var(lastScene) = null;
        var(currentWidth) = 0;
        var(currentHeight) = 0;
        
        var(gyroscopeMode) = true;
        var(lightFollowCamera) = true;
        var(deviceRotationMat3) = MCMatrix3Identity;
        
        var(lightHandler) = lightHandler;
        var(cameraHandler) = cameraHandler;
        var(contextHandler) = contextHandler;
        //var(skyboxCameraHandler) = skyboxCameraHandler;
        
        var(skybox) = null;
        var(skysph) = null;
        
        var(skyboxThread) = new(MCThread);
        var(modelThread) = new(MCThread);
        return obj;
    }else{
        return null;
    }
}

function(void, releaseScenes, MC3DScene* scene)
{
    as(MCDirector);
    if (scene!= null) {
        if (scene->prev != null) {
            releaseScenes(0, obj, scene->prev);
        }else{
            release(scene);
        }
    }
}

method(MCDirector, void, bye, voida)
{
    if (obj->lastScene != null) {
        releaseScenes(0, obj, obj->lastScene);
    }
    release(var(skybox));
    release(var(skysph));
    release(var(skyboxThread));
    release(var(modelThread));

    superbye(MCObject);
}

method(MCDirector, void, updateAll, voida)
{
    if (var(lastScene) != null) {
        if (var(gyroscopeMode)) {
            MCCamera_setRotationMat3(0, cpt(cameraHandler), obj->deviceRotationMat3.m);
            MC3DScene_setRotationMat3(0, var(lastScene), obj->deviceRotationMat3.m);
        }
        if (var(lightFollowCamera) && cpt(lightHandler) && cpt(cameraHandler)) {
            cpt(lightHandler)->lightPosition = computed(cpt(cameraHandler), currentPosition);
            cpt(lightHandler)->dataChanged = true;
        }
        MC3DScene_updateScene(0, var(lastScene), 0);
    }
}

method(MCDirector, int, drawAll, voida)
{
    int fps = -1;
    if (var(lastScene) != null) {
        fps = MC3DScene_drawScene(0, var(lastScene), 0);
    }
    return fps;
}

method(MCDirector, void, setupMainScene, unsigned width, unsigned height)
{
    MC3DScene* scene = ff(new(MC3DScene), initWithWidthHeightDefaultShader, width, height);
    MCDirector_pushScene(0, obj, scene);
}

method(MCDirector, void, pushScene, MC3DScene* scene)
{
    if (var(lastScene) == null) {
        var(lastScene) = scene;
    }else{
        scene->prev = var(lastScene);
        var(lastScene)->next = scene;
        
        var(lastScene) = scene;
    }
}

method(MCDirector, void, popScene, voida)
{
    ff(var(lastScene), lockCamera, true);
    //first scene
    if (var(lastScene) != null && var(lastScene)->prev != null) {
        var(lastScene) = var(lastScene)->prev;
    }
    //last scene
    else if (var(lastScene) != null && var(lastScene)->next != null) {
        var(lastScene) = var(lastScene)->next;
    }
    ff(var(lastScene), lockCamera, false);
}

method(MCDirector, void, resizeAllScene, int width, int height)
{
    if (var(currentWidth) == width && var(currentHeight) == height) {
        //no need to update
        return;
    }
    MC3DScene* iter;
    for (iter=var(lastScene); iter!=null; iter=iter->prev) {
        if (iter->skyboxRef != null) {
            iter->skyboxRef->boxCameraRatio = MCRatioMake(width, height);
        }
        if (iter->mainCamera != null) {
            iter->mainCamera->ratio = MCRatioMake(width, height);
        }
    }
    var(currentWidth) = width;
    var(currentHeight) = height;
}

method(MCDirector, void, addNode, MC3DNode* node)
{
    if(node && obj->lastScene && obj->lastScene->rootnode) {
        MC3DNode_addChild(0, obj->lastScene->rootnode, (MC3DNode*)node);
    }else{
        error_log("MCDirector add node(%p) failed [lastScene=%p rootnode=%p]\n",
                  node, obj->lastScene, obj->lastScene->rootnode);
    }
}

method(MCDirector, void, addModel, MC3DModel* model)
{
    if(model && obj->lastScene && obj->lastScene->rootnode) {
        MC3DNode_addChild(0, obj->lastScene->rootnode, (MC3DNode*)model);
        double maxl  = computed(model, maxlength);
        double scale = 10 / maxl;
        MCVector3 scaleVec = MCVector3Make(scale, scale, scale);
        MC3DNode_scaleVec3(0, &model->Super, &scaleVec, false);
        debug_log("MCDirector - model maxlength=%lf scale=%lf\n", maxl, scale);
        
    }else{
        error_log("MCDirector add model(%p) failed [lastScene=%p rootnode=%p]\n",
                  model, obj->lastScene, obj->lastScene->rootnode);
    }
}

method(MCDirector, void, addModelNamed, const char* name)
{
    MC3DModel* model = new(MC3DModel);
    MC3DModel_initWithFileName(0, model, name);
    MCDirector_addModel(0, obj, model);
}

method(MCDirector, void, addSkyboxNamed, const char* names[6])
{
    if (obj->lastScene) {
        //first release the old one
        if (var(skybox)) {
            release(var(skybox));
        }
        if (names) {
            var(skybox) = ff(new(MCSkybox), initWithFileNames, names);
        } else {
            var(skybox) = ff(new(MCSkybox), initWithDefaultFiles, 0);
        }
        obj->lastScene->skyboxRef = var(skybox);
        obj->lastScene->combineMode = MC3DSceneModelWithSkybox;
    }
}

method(MCDirector, void, addSkysphereNamed, const char* name)
{
    if (obj->lastScene) {
        //first release the old one
        if (var(skysph)) {
            release(var(skysph));
        }
        if (name) {
            var(skysph) = ff(new(MCSkysphere), initWithFileName, name);
        } else {
            var(skysph) = ff(new(MCSkysphere), initWithDefaultFile, 0);
        }
        obj->lastScene->skysphRef = var(skysph);
        obj->lastScene->combineMode = MC3DSceneModelWithSkysph;
    }
}

method(MCDirector, void, cameraFocusOn, MCVector4 vertex)
{
    MCCamera* c = computed(obj, cameraHandler);
    if (c != null) {
        c->lookat.x = vertex.x;
        c->lookat.y = vertex.y;
        c->lookat.z = vertex.z;
        c->R_value  = vertex.w;
        c->R_percent= 1.0;
    }
}

method(MCDirector, void, cameraFocusOnModel, MC3DModel* model)
{
    MC3DFrame frame = model->lastSavedFrame;
    double mheight = frame.ymax - frame.ymin;
    double mwidth  = frame.xmax - frame.xmin;
    double mdepth  = frame.zmax - frame.zmin;
    
    double _max = (mheight > mwidth) ? mheight : mwidth;
    double max = (mdepth > _max) ? mdepth : _max;
    
    cpt(cameraHandler)->lookat.y = mheight / 2.0f;
    cpt(cameraHandler)->R_value = max * 2.0f;
}

method(MCDirector, void, moveModelToOrigin, MC3DModel* model)
{
    MC3DModel_translateToOrigin(0, model, 0);
}

method(MCDirector, void, setDeviceRotationMat3, float mat3[9])
{
    if (mat3) {
        for (int i=0; i<9; i++) {
            obj->deviceRotationMat3.m[i] = mat3[i];
        }
    }
}

method(MCDirector, void, setCameraRotateMode, MCCameraRotateMode mode)
{
    if (cpt(cameraHandler)) {
        cpt(cameraHandler)->rotateMode = mode;
    }
}

method(MCDirector, void, printDebugInfo, voida)
{
    debug_log("MCDirector currentWidth=%d currentHeight=%d\n", obj->currentWidth, obj->currentHeight);
    MCCamera* cam = cpt(cameraHandler);
    if (cam) {
        ff(cam, printDebugInfo, 0);
    }
    if (obj->lastScene) {
        ff(obj->lastScene, printDebugInfo, 0);
    }
}

onload(MCDirector)
{
    if (load(MCObject)) {
        mixing(void, releaseScenes, MC3DScene* scene);
        
        binding(MCDirector, void, bye, voida);
        binding(MCDirector, void, updateAll, voida);
        binding(MCDirector, void, drawAll, voida);
        binding(MCDirector, void, setupMainScene, unsigned width, unsigned height);
        binding(MCDirector, void, pushScene, MC3DScene* scene);
        binding(MCDirector, void, popScene, voida);
        binding(MCDirector, void, resizeAllScene, int width, int height);
        binding(MCDirector, void, addNode, MC3DNode* node);
        binding(MCDirector, void, addModel, MC3DModel* model);
        binding(MCDirector, void, addModelNamed, const char* name);
        binding(MCDirector, void, addSkyboxNamed, const char* names[6]);
        binding(MCDirector, void, addSkysphereNamed, const char* name);
        binding(MCDirector, void, cameraFocusOn, MCVector3 vertex);
        binding(MCDirector, void, cameraFocusOnModel, MC3DModel* model);
        binding(MCDirector, void, moveModelToOrigin, MC3DModel* model);
        binding(MCDirector, void, setDeviceRotationMat3, float mat3[9]);
        binding(MCDirector, void, setCameraRotateMode, MCCameraRotateMode mode);
        binding(MCDirector, void, printDebugInfo, voida);

        return cla;
    }else{
        return null;
    }
}
