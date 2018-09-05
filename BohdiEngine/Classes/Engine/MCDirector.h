//
//  MCDirector.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/19.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCDirector_h
#define MCDirector_h

#include "monkc_export.h"
#include "MC3DScene.h"
#include "MC3DModel.h"
#include "MCLight.h"
#include "MCSkybox.h"
#include "MCSkysphere.h"

class(MCDirector, MCObject,
      MCObject* renderer;
      MC3DScene* lastScene;

      MCThread* skyboxThread;
      MCThread* modelThread;
      
      int currentWidth;
      int currentHeight;
      
      MCBool lightFollowCamera;
      //MCBool gyroscopeOnOff;
      MCBool pause;
      
      MCMatrix3 deviceRotationMat3;
      
      //common skybox for many scenes
      MCSkybox* skybox;
      MCSkysphere* skysph;
      
      computing(MCLight*, lightHandler);
      computing(MCCamera*, cameraHandler);
);

fun(MCDirector, void, bye, voida);
fun(MCDirector, void, updateAll, voida);

//return -1 or positive fps number
//please update your view only when it is not -1
//fun(MCDirector, int, drawAll, voida);

//fun(MCDirector, void, setupRenderer, MCObject* renderer);
fun(MCDirector, void, setupMainScene, unsigned width, unsigned height);
fun(MCDirector, void, setBackgroudColor, float R, float G, float B, float A);

fun(MCDirector, void, pushScene, MC3DScene* scene);
fun(MCDirector, void, popScene, voida);
fun(MCDirector, void, resizeAllScene, int width, int height);

fun(MCDirector, void, addNode, MC3DNode* node);
fun(MCDirector, void, addModel, MC3DModel* model, MCFloat maxsize);
fun(MCDirector, void, addModelAtIndex, MC3DModel* model, MCFloat maxsize, int index);

fun(MCDirector, MC3DModel*, addModelPathed, const char* path, MCFloat maxsize);
fun(MCDirector, MC3DModel*, addModelNamed, const char* name, MCFloat maxsize);
fun(MCDirector, MC3DModel*, addModelNamedAtIndex, const char* name, MCFloat maxsize, int index);

fun(MCDirector, void, removeCurrentModel, voida);
//use default if names/name is null
fun(MCDirector, void, addSkyboxNamed, const char* names[6]);
fun(MCDirector, void, addSkysphereNamed, const char* name);
fun(MCDirector, void, removeCurrentSkybox, voida);
fun(MCDirector, void, removeCurrentSkysph, voida);

fun(MCDirector, void, cameraFocusOn, MCVector4 vertex);
fun(MCDirector, void, cameraFocusOnModel, MC3DModel* model);
fun(MCDirector, void, cameraZoomToFitModel, MC3DModel* model);
fun(MCDirector, void, moveModelToOrigin, MC3DModel* model);

fun(MCDirector, void, setDeviceRotationMat3, float mat3[9]);
fun(MCDirector, void, setCameraRotateMode, MCCameraRotateMode mode);
fun(MCDirector, void, printDebugInfo, voida);

//iOS callbacks
//void onRootViewLoad(void* rootview);
//void onOpenExternalFile(const char* filepath);
//void onReceiveMemoryWarning();
//void onSetupGL(unsigned windowWidth, unsigned windowHeight, const char** filePathArray);
//void onTearDownGL();
//void onUpdate();
//void onDraw();

#endif /* MCDirector_h */
