//
//  MC3DScene.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DScene.h"
#include "BEAssetsManager.h"

compute(MCBool, isDrawSky)
{
    as(MC3DScene);
    if (var(combineMode) != MC3DSceneModelOnly) {
        return true;
    }
    return false;
}

oninit(MC3DScene)
{
    if (init(MCObject)) {
        var(bgcolor) = (MCColorf){0.05, 0.25, 0.35, 1.0};

        //var(skyboxShow) = false;
        var(skybox)  = null;
        var(skysph)  = null;
        
        var(rootnode)   = new(MC3DNode);
        var(mainCamera) = new(MCCamera);
        var(clock)      = new(MCClock);
        var(light)      = new(MCLight);
        
        var(next) = null;
        var(prev) = null;
        var(scenewidth) = 0;
        var(sceneheight)= 0;
        
        var(cameraLock) = false;
        var(isDrawSky)  = isDrawSky;
        
        var(combineMode)  = MC3DSceneModelOnly;
        var(cameraAutoRotate) = false;
        return obj;
    }else{
        return null;
    }
}

method(MC3DScene, void, printDebugInfo, voida)
{
    ff(obj->light, printDebugInfo, 0);
}

method(MC3DScene, void, bye, voida)
{
    release(var(skybox));
    release(var(skysph));
    release(var(rootnode));
    release(var(mainCamera));
    release(var(clock));
    release(var(light));
    
    superbye(MCObject);
}

method(MC3DScene, MC3DScene*, initWithWidthHeight, unsigned width, unsigned height)
{
    debug_log("MC3DScene initWithWidthHeightDefaultShader %dx%d %s\n", width, height, "MCGLRenderer");
    var(scenewidth)  = width;
    var(sceneheight) = height;
    MCCamera_initWithWidthHeight(var(mainCamera), width, height);
    return obj;
}

method(MC3DScene, void, resizeScene, unsigned width, unsigned height)
{
    obj->scenewidth  = width;
    obj->sceneheight = height;
    if (obj->skybox) {
        obj->skybox->boxCameraRatio = MCRatioMake(width, height);
    }
    if (obj->skysph) {
        obj->skysph->sphCameraRatio = MCRatioMake(width, height);
    }
    if (obj->mainCamera) {
        obj->mainCamera->ratio = MCRatioMake(width, height);
    }
}

method(MC3DScene, void, addSkybox, MCGLSkybox* box)
{
    if (obj->skybox) {
        release(obj->skybox);
    }
    obj->skybox = box;
    obj->combineMode = MC3DSceneModelWithSkybox;
    retain(box);
}

method(MC3DScene, void, addSkysph, MCSkysphere* sph)
{
    if (obj->skysph) {
        release(obj->skysph);
    }
    obj->skysph = sph;
    obj->combineMode = MC3DSceneModelWithSkysph;
    retain(sph);
}

method(MC3DScene, void, removeSkybox, voida)
{
    if (obj->skybox) {
        release(obj->skybox);
        obj->skybox = null;
        obj->combineMode = MC3DSceneModelOnly;
    }
}

method(MC3DScene, void, removeSkysph, voida)
{
    if (obj->skysph) {
        release(obj->skysph);
        obj->skysph = null;
        obj->combineMode = MC3DSceneModelOnly;
    }
}

method(MC3DScene, void, lockCamera, MCBool lock)
{
    var(cameraLock) = lock;
    printf("lock=%d\n", lock);
}

method(MC3DScene, MCCamera*, getCamera, voida)
{
    return var(mainCamera);
}

method(MC3DScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht)
{
    if (var(cameraLock) == false) {
//        if (var(skyboxRef) != null) {
//            MCGLSkyboxCamera_move(var(skyboxRef)->camera, deltaFai.d / 5, deltaTht.d / 5);
//        }

        MCCamera_move(var(mainCamera), deltaFai, deltaTht);
    }
}

method(MC3DScene, void, moveSkyboxCamera, MCFloat deltaFai, MCFloat deltaTht)
{
    if (cpt(isDrawSky)) {
        //MCGLSkyboxCamera_move(var(skyboxRef)->camera, deltaFai, deltaTht);
    }
}

method(MC3DScene, void, setRotationMat3, float mat3[9])
{
    if (cpt(isDrawSky)) {
        if (var(skybox)) {
            MCGLSkybox_setRotationMat3(var(skybox), mat3);
        }
        if (var(skysph)) {
            MCSkysphere_setRotationMat3(var(skysph), mat3);
        }
    }
    MCCamera_setRotationMat3(var(mainCamera), mat3);
}

method(MC3DScene, void, setRotationMat4, float mat4[16])
{
    if (cpt(isDrawSky)) {
        if (var(skybox)) {
            MCGLSkybox_setRotationMat4(var(skybox), mat4);
        }
        if (var(skysph)) {
            MCSkysphere_setRotationMat4(var(skysph), mat4);
        }
    }
    //TODO rotate camera
}

onload(MC3DScene)
{
    if (load(MCObject)) {
        binding(MC3DScene, void, bye, voida);
        binding(MC3DScene, MC3DScene*, initWithWidthHeight, unsigned width, unsigned height);
        binding(MC3DScene, void, resizeScene, unsigned width, unsigned height);
        binding(MC3DScene, void, addSkybox, MCGLSkybox* box);
        binding(MC3DScene, void, addSkysph, MCSkysphere* sph);
        binding(MC3DScene, void, removeSkybox, voida);
        binding(MC3DScene, void, removeSkysph, voida);
        binding(MC3DScene, void, lockCamera, MCBool lock);
        binding(MC3DScene, MCCamera*, getCamera, voida);
        binding(MC3DScene, void, moveCameraOneStep, double deltaFai, double deltaTht);
        binding(MC3DScene, void, moveSkyboxCamera, MCDouble deltaFai, MCDouble deltaTht);
        binding(MC3DScene, void, setRotationMat3, float mat3[9]);
        binding(MC3DScene, void, setRotationMat4, float mat4[16]);
        binding(MC3DScene, void, printDebugInfo, voida);

        return cla;
    }else{
        return null;
    }
}
