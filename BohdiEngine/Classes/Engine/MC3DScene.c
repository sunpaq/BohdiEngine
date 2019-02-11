//
//  MC3DScene.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DScene.h"
#include "MCLog.h"
#include "MCClock.h"

fun(isDrawSky, bool)) as(MC3DScene)
    return (it->combineMode != MC3DSceneModelOnly);
}

fun(release, void)) as(MC3DScene)
    Release(it->skybox);
    Release(it->skysph);
    Release(it->rootnode);
    Release(it->mainCamera);
    Release(it->clock);
    Release(it->light);
    cast(it, MCObject)->release(it);
}

fun(resizeScene, void), unsigned width, unsigned height) as(MC3DScene)
    it->scenewidth  = width;
    it->sceneheight = height;
    if (it->skybox) {
        it->skybox->boxCameraRatio = MCRatioMake(width, height);
    }
    if (it->skysph) {
        it->skysph->sphCameraRatio = MCRatioMake(width, height);
    }
    if (it->mainCamera) {
        it->mainCamera->ratio = MCRatioMake(width, height);
    }
}

fun(addSkybox, void), struct MCSkybox* box) as(MC3DScene)
    if (it->skybox) {
        Release(it->skybox);
    }
    it->skybox = box;
    it->combineMode = MC3DSceneModelWithSkybox;
    Retain(box);
}

fun(addSkysph, void), struct MCSkysphere* sph) as(MC3DScene)
    if (it->skysph) {
        Release(it->skysph);
    }
    it->skysph = sph;
    it->combineMode = MC3DSceneModelWithSkysph;
    Retain(sph);
}

fun(removeSkybox, void)) as(MC3DScene)
    if (it->skybox) {
        Release(it->skybox);
        it->skybox = null;
        it->combineMode = MC3DSceneModelOnly;
    }
}

fun(removeSkysph, void)) as(MC3DScene)
    if (it->skysph) {
        Release(it->skysph);
        it->skysph = null;
        it->combineMode = MC3DSceneModelOnly;
    }
}

fun(lockCamera, void), bool lock) as(MC3DScene)
    it->cameraLock = lock;
    printf("lock=%d\n", lock);
}

fun(getCamera, struct MCCamera*)) as(MC3DScene)
    return it->mainCamera;
}

fun(moveCameraOneStep, void), float deltaFai, float deltaTht) as(MC3DScene)
    if (it->cameraLock == false) {
        //        if (var(skyboxRef) != null) {
        //            MCSkyboxCamera_move(var(skyboxRef)->camera, deltaFai.d / 5, deltaTht.d / 5);
        //        }
        
        it->mainCamera->move(it->mainCamera, deltaFai, deltaTht);
    }
}

fun(moveSkyboxCamera, void), float deltaFai, float deltaTht) as(MC3DScene)
    if (it->isDrawSky(it)) {
        //MCSkyboxCamera_move(var(skyboxRef)->camera, deltaFai, deltaTht);
    }
}

fun(setRotationMat3, void), float mat3[9]) as(MC3DScene)
    if (it->isDrawSky(it)) {
        if (it->skybox) {
            it->skybox->setRotationMat3(it->skybox, mat3);
        }
        if (it->skysph) {
            it->skysph->setRotationMat3(it->skysph, mat3);
        }
    }
    it->mainCamera->setRotationMat3(it->mainCamera, mat3);
}

fun(setRotationMat4, void), float mat4[16]) as(MC3DScene)
    if (it->isDrawSky) {
        if (it->skybox) {
            it->skybox->setRotationMat4(it->skybox, mat4);
        }
        if (it->skysph) {
            it->skysph->setRotationMat4(it->skysph, mat4);
        }
    }
    //TODO rotate camera
}

fun(printDebugInfo, void)) as(MC3DScene)
    ff(it->light, printDebugInfo));
}

constructor(MC3DScene), unsigned width, unsigned height) {
    MCObject(any);
    as(MC3DScene)
        debug_log("MC3DScene initWithWidthHeightDefaultShader %dx%d %s\n", width, height, "MCGLRenderer");
        it->scenewidth = width;
        it->sceneheight = height;
        it->mainCamera = MCCamera(alloc(MCCamera), width, height);
    
        it->bgcolor = (MCColorf){0.05, 0.25, 0.35, 1.0};
        it->skybox = null;
        it->skysph = null;
    
        it->rootnode = MC3DNode(alloc(MC3DNode));
        it->clock = MCClock(alloc(MCClock));
        it->light = MCLight(alloc(MCLight));
    
        it->next = null;
        it->prev = null;
        it->cameraLock = false;
    
        it->combineMode = MC3DSceneModelOnly;
        it->cameraAutoRotate = false;
    
        funadd(isDrawSky);
    }
    dynamic(MC3DScene)
        funbind(release);
        funbind(resizeScene);
        funbind(addSkybox);
        funbind(addSkysph);
        funbind(removeSkybox);
        funbind(removeSkysph);
        funbind(lockCamera);
        funbind(getCamera);
        funbind(moveCameraOneStep);
        funbind(moveSkyboxCamera);
        funbind(setRotationMat3);
        funbind(setRotationMat4);
        funbind(printDebugInfo);
    }
    return any;
}

