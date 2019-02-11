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
#include "MCThread.h"
#include "MCLog.h"

fun(lightHandler, struct MCLight*)) as(MCDirector)
    if (it->lastScene != null && it->lastScene->light != null) {
        return it->lastScene->light;
    }
    return null;
}

fun(cameraHandler, struct MCCamera*)) as(MCDirector)
    if (it->lastScene != null && it->lastScene->mainCamera != null) {
        return it->lastScene->mainCamera;
    }
    return null;
}

fun(releaseScenes, void), struct MC3DScene* scene) as(MCDirector)
    if (scene!= null) {
        if (scene->prev != null) {
            it->releaseScenes(it, scene->prev);
        }else{
            Release(scene);
        }
    }
}

fun(release, void)) as(MCDirector)
    it->pause = true;
    if (it->lastScene != null) {
        releaseScenes(it, it->lastScene);
        it->lastScene = null;
    }
    Release(it->skybox);
    Release(it->skysph);
    Release(it->skyboxThread);
    Release(it->modelThread);
    Release(it->renderer);
    cast(it, MCObject)->release(it);
}

fun(updateAll, void)) as(MCDirector)
    if (it && it->lastScene != null && it->pause == false) {
        if (it->cameraHandler(it)->rotateMode == MCCameraRotateAroundModelByGyroscope
            || it->cameraHandler(it)->rotateMode == MCCameraRotateAroundModelByGyroscopeReverse) {
            it->lastScene->setRotationMat3(it->lastScene, it->deviceRotationMat3.m);
        }
        if (it->lightFollowCamera && it->lightHandler(it) && it->cameraHandler(it)) {
            cpt(lightHandler)->lightPosition = computed(cpt(cameraHandler), currentPosition);
            cpt(lightHandler)->dataChanged = true;
        }
        //dynamic update scene
        if (it->renderer) {
            ff(it->renderer, updateScene), it->lastScene);
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

fun(setupMainScene, void), unsigned width, unsigned height) as(MCDirector)
    struct MC3DScene* scene = MC3DScene(alloc(MC3DScene), width, height);
    if (scene) {
        releaseScenes(it, it->lastScene);
        it->pushScene(it, scene);
        Release(scene);
    }
}

fun(setBackgroudColor, void), float R, float G, float B, float A) as(MCDirector)
    if (it->lastScene) {
        it->lastScene->bgcolor = (MCColorf){R,G,B,A};
    }
}

fun(pushScene, void), struct MC3DScene* scene) as(MCDirector)
    if (it->lastScene == null) {
        scene->next = null;
        scene->prev = null;
        it->lastScene = scene;
        Retain(scene);
    }else{
        scene->next = null;
        scene->prev = it->lastScene;
        it->lastScene->next = scene;
        it->lastScene = scene;
        Retain(scene);
    }
}

fun(popScene, void)) as(MCDirector)
    if (it->lastScene) {
        struct MC3DScene* scene = it->lastScene;
        scene->lockCamera(scene, true);
        struct MC3DScene* current = it->lastScene;
        //first scene
        if (current->prev == null) {
            it->lastScene = null;
            Release(current);
        }
        else {
            it->lastScene = current->next;
            Release(current);
        }
        scene->lockCamera(scene, false);
    }
}

fun(resizeAllScene, void), int width, int height) as(MCDirector)
    if (it->currentWidth == width && it->currentHeight == height) {
        //no need to update
        return;
    }
    struct MC3DScene* iter = null;
    for (iter=it->lastScene; iter!=null; iter=iter->prev) {
        iter->resizeScene(iter, width, height);
    }
    it->currentWidth = width;
    it->currentHeight = height;
}

//fun(MCDirector, void, scissorAllScene, int x, int y, int width, int height)
//{
//    MCGLContext_setViewport(x, y, width, height);
//    MCGLContext_setScissor(x, y, width, height);
//    //call resize scene
//    MCDirector_resizeAllScene(obj, width, height);
//}

fun(addNode, void), struct MC3DNode* node) as(MCDirector)
    if(node && it->lastScene && it->lastScene->rootnode) {
        it->lastScene->rootnode->addChild(it->lastScene->rootnode, (MC3DNode_t*)node);
    }else{
        error_log("MCDirector add node(%p) failed [lastScene=%p]\n",
                  node, it->lastScene);
    }
}

fun(addModel, void), struct MC3DModel* model, float maxsize) as(MCDirector)
    it->addModelAtIndex(it, model, maxsize, -1);
}

fun(addModelAtIndex, void), struct MC3DModel* model, float maxsize, int index) as(MCDirector)
    if(model && it->lastScene && it->lastScene->rootnode) {
        model->resizeToFit(model, maxsize);
        it->lastScene->rootnode->addChildAtIndex(it->lastScene->rootnode, (MC3DNode_t*)model, index);
    }else{
        error_log("MCDirector add model(%p) failed [lastScene=%p]\n",
                  model, it->lastScene);
    }
}

fun(addModelPathed, struct MC3DModel*), const char* path, float maxsize) as(MCDirector)
    struct MC3DModel* model = MC3DModel(alloc(MC3DModel));
    model->initWithFilePath(model, path);
    if (model) {
        it->addModelAtIndex(it, model, maxsize, -1);
        return model;
    }
    return null;
}

fun(addModelNamed, struct MC3DModel*), const char* name, float maxsize) as(MCDirector)
    MC3DModelCache_t* c = MC3DModelCache_shared();
    struct MC3DModel* model = c->fetchModelNamed(c, name);
    if (model) {
        it->addModelAtIndex(it, model, maxsize, -1);
        return model;
    }
    return it->addModelNamedAtIndex(it, name, maxsize, -1);
}

fun(addModelNamedAtIndex, struct MC3DModel*), const char* name, float maxsize, int index) as(MCDirector)
    struct MC3DModel* model = MC3DModel(alloc(MC3DModel));
    model->initWithFileName(model, name);
    it->addModelAtIndex(it, model, maxsize, index);
    return model;
}

fun(removeCurrentModel, void)) as(MCDirector)
    if (it->lastScene) {
        it->pause = true;
        struct MCLinkedList* list = it->lastScene->rootnode->children;
        list->popItem(list);
        it->pause = false;
    }
}

fun(addSkyboxNamed, void), const char* names[6]) as(MCDirector)
    if (it->lastScene) {
        struct MCSkybox* box = MCSkybox(alloc(MCSkybox));
        box->initWithFileNames(box, names);
        if (box) {
            it->lastScene->addSkybox(it->lastScene, box);
            Release(box);
        }
    }
}

fun(addSkysphereNamed, void), const char* name) as(MCDirector)
    if (it->lastScene) {
        struct MCSkysphere* sph = MCSkysphere(alloc(MCSkysphere));
        sph->initWithFileName(sph, name);
        if (sph) {
            it->lastScene->addSkysph(it->lastScene, sph);
            Release(sph);
        }
    }
}

fun(removeCurrentSkybox, void)) as(MCDirector)
    if (it->lastScene) {
        it->lastScene->removeSkybox(it->lastScene);
    }
}

fun(removeCurrentSkysph, void)) as(MCDirector)
    if (it->lastScene) {
        it->lastScene->removeSkysph(it->lastScene);
    }
}

fun(cameraFocusOn, void), MCVector4 vertex) as(MCDirector)
    struct MCCamera* c = computed(it, cameraHandler);
    if (c != null) {
        c->lookat.x = vertex.x;
        c->lookat.y = vertex.y;
        c->lookat.z = vertex.z;
        c->R_value  = vertex.w;
        c->R_percent= 1.0;
    }
}

fun(cameraFocusOnModel, void), struct MC3DModel* model) as(MCDirector)
    cpt(cameraHandler)->lookat  = computed(model, center);
}

fun(cameraZoomToFitModel, void), struct MC3DModel* model) as(MCDirector)
    cpt(cameraHandler)->R_value = computed(model, maxlength) * 2.0f;
}

fun(moveModelToOrigin, void), struct MC3DModel* model) {
    model->translateToOrigin(model);
}

fun(setDeviceRotationMat3, void), float mat3[9]) as(MCDirector)
    if (mat3) {
        int i;
        for (i=0; i<9; i++) {
            it->deviceRotationMat3.m[i] = mat3[i];
        }
    }
}

fun(setCameraRotateMode, void), MCCameraRotateMode mode) as(MCDirector)
    if (cpt(cameraHandler)) {
        cpt(cameraHandler)->rotateMode = mode;
    }
}

fun(printDebugInfo, void)) as(MCDirector)
    debug_log("MCDirector currentWidth=%d currentHeight=%d\n", it->currentWidth, it->currentHeight);
    struct MCCamera* cam = cpt(cameraHandler);
    if (cam) {
        ff(cam, printDebugInfo));
    }
    if (it->lastScene) {
        ff(it->lastScene, printDebugInfo));
    }
}

constructor(MCDirector)) {
    MCObject(any);
    as(MCDirector)
        it->lastScene = null;
        it->currentWidth = 0;
        it->currentHeight = 0;
        it->pause = false;
        it->lightFollowCamera = true;
        it->deviceRotationMat3 = MCMatrix3Identity;
        it->skybox = null;
        it->skysph = null;
        it->renderer = null;
        it->skyboxThread = MCThread(alloc(MCThread));
        it->modelThread = MCThread(alloc(MCThread));
    
        funadd(lightHandler);
        funadd(cameraHandler);
    }
    dynamic(MCDirector)
        funbind(releaseScenes);
        funbind(updateAll);
        funbind(setupMainScene);
        funbind(setBackgroudColor);
        funbind(pushScene);
        funbind(popScene);
        funbind(resizeAllScene);
        funbind(addNode);
        funbind(addModel);
        funbind(addModelAtIndex);
        funbind(addModelPathed);
        funbind(addModelNamed);
        funbind(addModelNamedAtIndex);
        funbind(removeCurrentModel);
        //use default if names/name is null
        funbind(addSkyboxNamed);
        funbind(addSkysphereNamed);
        funbind(removeCurrentSkybox);
        funbind(removeCurrentSkysph);
        funbind(cameraFocusOn);
        funbind(cameraFocusOnModel);
        funbind(cameraZoomToFitModel);
        funbind(moveModelToOrigin);
        funbind(setDeviceRotationMat3);
        funbind(setCameraRotateMode);
        funbind(printDebugInfo);
        funbind(release);
    }
    return any;
}

