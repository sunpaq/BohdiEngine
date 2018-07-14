//
//  MC3DScene.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DScene_h
#define MC3DScene_h

#include "monkc.h"
#include "MC3DNode.h"
#include "MCCamera.h"
#include "MCClock.h"
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

class(MC3DScene, MCObject,
      struct MC3DSceneStruct* next;
      struct MC3DSceneStruct* prev;
      
      MCColorf      bgcolor;
      unsigned      scenewidth;
      unsigned      sceneheight;
      
      MCSkybox*     skybox;
      MCSkysphere*  skysph;
      
      MC3DNode*     rootnode;
      MCCamera*     mainCamera;
      MCLight*      light;
      MCBool        cameraLock;
      MCClock*      clock;
      
      computing(MCBool, isDrawSky);
      MC3DSceneCombineMode combineMode;

      MCBool cameraAutoRotate;
);

method(MC3DScene, void, bye, voida);
method(MC3DScene, MC3DScene*, initWithWidthHeight, unsigned width, unsigned height);
method(MC3DScene, void, resizeScene, unsigned width, unsigned height);
method(MC3DScene, void, addSkybox, MCSkybox* box);
method(MC3DScene, void, addSkysph, MCSkysphere* sph);
method(MC3DScene, void, removeSkybox, voida);
method(MC3DScene, void, removeSkysph, voida);
method(MC3DScene, void, lockCamera, MCBool lock);
method(MC3DScene, MCCamera*, getCamera, voida);
method(MC3DScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
method(MC3DScene, void, moveSkyboxCamera, MCFloat deltaFai, MCFloat deltaTht);
method(MC3DScene, void, setRotationMat3, float mat3[9]);
method(MC3DScene, void, setRotationMat4, float mat4[16]);
method(MC3DScene, void, printDebugInfo, voida);

#endif /* MC3DScene_h */
