//
//  MC3DScene.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DScene_h
#define MC3DScene_h

#include "monkc_export.h"
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

fun(MC3DScene, void, bye, voida);
fun(MC3DScene, MC3DScene*, initWithWidthHeight, unsigned width, unsigned height);
fun(MC3DScene, void, resizeScene, unsigned width, unsigned height);
fun(MC3DScene, void, addSkybox, MCSkybox* box);
fun(MC3DScene, void, addSkysph, MCSkysphere* sph);
fun(MC3DScene, void, removeSkybox, voida);
fun(MC3DScene, void, removeSkysph, voida);
fun(MC3DScene, void, lockCamera, MCBool lock);
fun(MC3DScene, MCCamera*, getCamera, voida);
fun(MC3DScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
fun(MC3DScene, void, moveSkyboxCamera, MCFloat deltaFai, MCFloat deltaTht);
fun(MC3DScene, void, setRotationMat3, float mat3[9]);
fun(MC3DScene, void, setRotationMat4, float mat4[16]);
fun(MC3DScene, void, printDebugInfo, voida);

#endif /* MC3DScene_h */
