//
//  MC3DScene.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DScene_h
#define MC3DScene_h

//#include "monkc_export.h"
#include "MC3DNode.h"
#include "MCCamera.h"
#include "MCLight.h"
#include "MCSkybox.h"
#include "MCSkysphere.h"

typedef enum {
    MC3DSceneModelOnly,
    MC3DSceneModelWithSkybox,
    MC3DSceneModelWithSkysph,
    MC3DSceneSkyboxOnly,
    MC3DSceneSkysphOnly
} MC3DSceneCombineMode;

structure(MC3DScene, MCObject)
    struct MC3DScene* next;
    struct MC3DScene* prev;

    MCColorf bgcolor;
    unsigned scenewidth;
    unsigned sceneheight;

    struct MCSkybox* skybox;
    struct MCSkysphere* skysph;

    struct MC3DNode* rootnode;
    struct MCCamera* mainCamera;
    struct MCLight* light;
    struct MCClock* clock;

    bool cameraLock;
    bool cameraAutoRotate;

    MC3DSceneCombineMode combineMode;

    fundef(isDrawSky, bool));
    fundef(release, void));
    fundef(resizeScene, void), unsigned width, unsigned height);
    fundef(addSkybox, void), struct MCSkybox* box);
    fundef(addSkysph, void), struct MCSkysphere* sph);
    fundef(removeSkybox, void));
    fundef(removeSkysph, void));
    fundef(lockCamera, void), bool lock);
    fundef(getCamera, struct MCCamera*));
    fundef(moveCameraOneStep, void), float deltaFai, float deltaTht);
    fundef(moveSkyboxCamera, void), float deltaFai, float deltaTht);
    fundef(setRotationMat3, void), float mat3[9]);
    fundef(setRotationMat4, void), float mat4[16]);
    fundef(printDebugInfo, void));
};

constructor(MC3DScene), unsigned width, unsigned height);

alias(MC3DScene);

#endif /* MC3DScene_h */
