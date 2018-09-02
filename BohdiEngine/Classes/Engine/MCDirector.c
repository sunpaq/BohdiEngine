//
//  MCDirector.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/19.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCDirector.h"
#include "MCCube.h"
#include "MC3DModelCache.h"

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

oninit(MCDirector)
{
    if (init(MCObject)) {
        var(lastScene) = null;
        var(currentWidth) = 0;
        var(currentHeight) = 0;
        
        var(pause) = false;
        var(lightFollowCamera) = true;
        var(deviceRotationMat3) = MCMatrix3Identity;
        
        var(lightHandler) = lightHandler;
        var(cameraHandler) = cameraHandler;
        
        var(skybox) = null;
        var(skysph) = null;
        
        var(renderer) = null;
        var(skyboxThread) = new(MCThread);
        var(modelThread) = new(MCThread);
        return obj;
    }else{
        return null;
    }
}

ifun(void, releaseScenes, MC3DScene* scene)
{
    as(MCDirector);
    if (scene!= null) {
        if (scene->prev != null) {
            releaseScenes(obj, scene->prev);
        }else{
            release(scene);
        }
    }
}

fun(MCDirector, void, bye, voida)
{
    obj->pause = true;
    if (obj->lastScene != null) {
        releaseScenes(obj, obj->lastScene);
        obj->lastScene = null;
    }
    release(var(skybox));
    release(var(skysph));
    release(var(skyboxThread));
    release(var(modelThread));
    release(var(renderer));
    superbye(MCObject);
}

fun(MCDirector, void, updateAll, voida)
{
    if (obj && var(lastScene) != null && var(pause) == false) {
        if (cpt(cameraHandler)->rotateMode == MCCameraRotateAroundModelByGyroscope
            || cpt(cameraHandler)->rotateMode == MCCameraRotateAroundModelByGyroscopeReverse) {
            MC3DScene_setRotationMat3(var(lastScene), obj->deviceRotationMat3.m);
        }
        if (var(lightFollowCamera) && cpt(lightHandler) && cpt(cameraHandler)) {
            cpt(lightHandler)->lightPosition = computed(cpt(cameraHandler), currentPosition);
            cpt(lightHandler)->dataChanged = true;
        }
        //MC3DScene_updateScene(var(lastScene), 0);
        if (var(renderer)) {
            ff(var(renderer), updateScene, var(lastScene));
        }
    }
}

//fun(MCDirector, int, drawAll, voida)
//{
//    int fps = -1;
//    if (obj && var(lastScene) && var(renderer) && var(pause) == false) {
//        ff(var(renderer), drawScene, var(lastScene));
//        //fps = MC3DScene_drawScene(var(lastScene), 0);
//    }
//    return fps;
//}
//
//fun(MCDirector, void, setupRenderer, MCObject* renderer)
//{
//    var(renderer) = renderer;
//    retain(renderer);
//}

fun(MCDirector, void, setupMainScene, unsigned width, unsigned height)
{
    MC3DScene* scene = ff(new(MC3DScene), initWithWidthHeight, width, height);
    if (scene) {
        releaseScenes(obj, obj->lastScene);
        MCDirector_pushScene(obj, scene);
        release(scene);
        //test
        //MCDirector_addNode(obj, new(MCCube));
    }
}

fun(MCDirector, void, setBackgroudColor, float R, float G, float B, float A)
{
    if (obj->lastScene) {
        obj->lastScene->bgcolor = (MCColorf){R,G,B,A};
    }
}

fun(MCDirector, void, pushScene, MC3DScene* scene)
{
    if (var(lastScene) == null) {
        scene->next = null;
        scene->prev = null;
        var(lastScene) = scene;
        retain(scene);
    }else{
        scene->next = null;
        scene->prev = var(lastScene);
        var(lastScene)->next = scene;
        var(lastScene) = scene;
        retain(scene);
    }
}

fun(MCDirector, void, popScene, voida)
{
    if (var(lastScene)) {
        ff(var(lastScene), lockCamera, true);
        MC3DScene* current = var(lastScene);
        //first scene
        if (current->prev == null) {
            var(lastScene) = null;
            release(current);
        }
        else {
            var(lastScene) = current->next;
            release(current);
        }
        ff(var(lastScene), lockCamera, false);
    }
}

fun(MCDirector, void, resizeAllScene, int width, int height)
{
    if (var(currentWidth) == width && var(currentHeight) == height) {
        //no need to update
        return;
    }
    MC3DScene* iter = null;
    for (iter=var(lastScene); iter!=null; iter=iter->prev) {
        MC3DScene_resizeScene(iter, width, height);
    }
    var(currentWidth) = width;
    var(currentHeight) = height;
}

//fun(MCDirector, void, scissorAllScene, int x, int y, int width, int height)
//{
//    MCGLContext_setViewport(x, y, width, height);
//    MCGLContext_setScissor(x, y, width, height);
//    //call resize scene
//    MCDirector_resizeAllScene(obj, width, height);
//}

fun(MCDirector, void, addNode, MC3DNode* node)
{
    if(node && obj->lastScene && obj->lastScene->rootnode) {
        MC3DNode_addChild(obj->lastScene->rootnode, (MC3DNode*)node);
    }else{
        error_log("MCDirector add node(%p) failed [lastScene=%p]\n",
                  node, obj->lastScene);
    }
}

fun(MCDirector, void, addModel, MC3DModel* model, MCFloat maxsize)
{
    MCDirector_addModelAtIndex(obj, model, maxsize, -1);
}

fun(MCDirector, void, addModelAtIndex, MC3DModel* model, MCFloat maxsize, int index)
{
    if(model && obj->lastScene && obj->lastScene->rootnode) {
        MC3DModel_resizeToFit(model, maxsize.f);
        MC3DNode_addChildAtIndex(obj->lastScene->rootnode, (MC3DNode*)model, index);
    }else{
        error_log("MCDirector add model(%p) failed [lastScene=%p]\n",
                  model, obj->lastScene);
    }
}

fun(MCDirector, MC3DModel*, addModelPathed, const char* path, MCFloat maxsize)
{
    MC3DModel* model = MC3DModel_initWithFilePath(new(MC3DModel), path);
    if (model) {
        MCDirector_addModelAtIndex(obj, model, maxsize, -1);
        return model;
    }
    return null;
}

fun(MCDirector, MC3DModel*, addModelNamed, const char* name, MCFloat maxsize)
{
    MC3DModel* model = MC3DModelCache_fetchModelNamed(MC3DModelCache_shared(0), name);
    if (model) {
        MCDirector_addModelAtIndex(obj, model, maxsize, -1);
        return model;
    }
    return MCDirector_addModelNamedAtIndex(obj, name, maxsize, -1);
}

fun(MCDirector, MC3DModel*, addModelNamedAtIndex, const char* name, MCFloat maxsize, int index)
{
    MC3DModel* model = new(MC3DModel);
    MC3DModel_initWithFileName(model, name);
    MCDirector_addModelAtIndex(obj, model, maxsize, index);
    return model;
}

fun(MCDirector, void, removeCurrentModel, voida)
{
    if (obj->lastScene) {
        obj->pause = true;
        MCLinkedList* list = obj->lastScene->rootnode->children;
        MCLinkedList_popItem(list, 0);
        obj->pause = false;
    }
}

fun(MCDirector, void, addSkyboxNamed, const char* names[6])
{
    if (obj->lastScene) {
        MCSkybox* box = ff(new(MCSkybox), initWithFileNames, names);
        if (box) {
            MC3DScene_addSkybox(obj->lastScene, box);
            release(box);
        }
    }
}

fun(MCDirector, void, addSkysphereNamed, const char* name)
{
    if (obj->lastScene) {
        MCSkysphere* sph = ff(new(MCSkysphere), initWithFileName, name);
        if (sph) {
            MC3DScene_addSkysph(obj->lastScene, sph);
            release(sph);
        }
    }
}

fun(MCDirector, void, removeCurrentSkybox, voida)
{
    if (obj->lastScene) {
        MC3DScene_removeSkybox(obj->lastScene, 0);
    }
}

fun(MCDirector, void, removeCurrentSkysph, voida)
{
    if (obj->lastScene) {
        MC3DScene_removeSkysph(obj->lastScene, 0);
    }
}

fun(MCDirector, void, cameraFocusOn, MCVector4 vertex)
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

fun(MCDirector, void, cameraFocusOnModel, MC3DModel* model)
{
    cpt(cameraHandler)->lookat  = computed(model, center);
}

fun(MCDirector, void, cameraZoomToFitModel, MC3DModel* model)
{
    cpt(cameraHandler)->R_value = computed(model, maxlength) * 2.0f;
}

fun(MCDirector, void, moveModelToOrigin, MC3DModel* model)
{
    MC3DModel_translateToOrigin(model, 0);
}

fun(MCDirector, void, setDeviceRotationMat3, float mat3[9])
{
    if (mat3) {
        for (int i=0; i<9; i++) {
            obj->deviceRotationMat3.m[i] = mat3[i];
        }
    }
}

fun(MCDirector, void, setCameraRotateMode, MCCameraRotateMode mode)
{
    if (cpt(cameraHandler)) {
        cpt(cameraHandler)->rotateMode = mode;
    }
}

fun(MCDirector, void, printDebugInfo, voida)
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
        mix(void, releaseScenes, MC3DScene* scene);
        
        bid(MCDirector, void, bye, voida);
        bid(MCDirector, void, updateAll, voida);
        bid(MCDirector, void, setupMainScene, unsigned width, unsigned height);
        bid(MCDirector, void, setBackgroudColor, float R, float G, float B, float A);
        bid(MCDirector, void, pushScene, MC3DScene* scene);
        bid(MCDirector, void, popScene, voida);
        bid(MCDirector, void, resizeAllScene, int width, int height);
        bid(MCDirector, void, addNode, MC3DNode* node);
        bid(MCDirector, void, addModel, MC3DModel* model, int maxsize);
        bid(MCDirector, void, addModelAtIndex, MC3DModel* model, MCFloat maxsize, int index);
        bid(MCDirector, MC3DModel*, addModelPathed, const char* path, MCFloat maxsize);
        bid(MCDirector, MC3DModel*, addModelNamed, const char* name, MCFloat maxsize);
        bid(MCDirector, MC3DModel*, addModelNamedAtIndex, const char* name, MCFloat maxsize, int index);
        bid(MCDirector, void, removeCurrentModel, voida);
        bid(MCDirector, void, addSkyboxNamed, const char* names[6]);
        bid(MCDirector, void, addSkysphereNamed, const char* name);
        bid(MCDirector, void, removeCurrentSkybox, voida);
        bid(MCDirector, void, removeCurrentSkysph, voida);
        bid(MCDirector, void, cameraFocusOn, MCVector3 vertex);
        bid(MCDirector, void, cameraFocusOnModel, MC3DModel* model);
        bid(MCDirector, void, cameraZoomToFitModel, MC3DModel* model);
        bid(MCDirector, void, moveModelToOrigin, MC3DModel* model);
        bid(MCDirector, void, setDeviceRotationMat3, float mat3[9]);
        bid(MCDirector, void, setCameraRotateMode, MCCameraRotateMode mode);
        bid(MCDirector, void, printDebugInfo, voida);

        return cla;
    }else{
        return null;
    }
}
