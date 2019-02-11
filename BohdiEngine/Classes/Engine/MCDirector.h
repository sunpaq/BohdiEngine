//
//  MCDirector.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/19.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCDirector_h
#define MCDirector_h

//#include "monkc_export.h"
#include "MC3DScene.h"
#include "MC3DModel.h"
#include "MCLight.h"
#include "MCSkybox.h"
#include "MCSkysphere.h"

structure(MCDirector, MCObject)
    struct MCObject* renderer;
    struct MC3DScene* lastScene;

    struct MCThread* skyboxThread;
    struct MCThread* modelThread;

    int currentWidth;
    int currentHeight;

    bool lightFollowCamera;
    bool gyroscopeOnOff;
    bool pause;

    MCMatrix3 deviceRotationMat3;

    //common skybox for many scenes
    struct MCSkybox* skybox;
    struct MCSkysphere* skysph;

    fundef(lightHandler, struct MCLight*));
    fundef(cameraHandler, struct MCCamera*));
    fundef(updateAll, void));
    fundef(setupMainScene, void), unsigned width, unsigned height);
    fundef(setBackgroudColor, void), float R, float G, float B, float A);
    fundef(pushScene, void), struct MC3DScene* scene);
    fundef(popScene, void));
    fundef(releaseScenes, void), struct MC3DScene* scene);
    fundef(resizeAllScene, void), int width, int height);
    fundef(addNode, void), struct MC3DNode* node);
    fundef(addModel, void), struct MC3DModel* model, float maxsize);
    fundef(addModelAtIndex, void), struct MC3DModel* model, float maxsize, int index);
    fundef(addModelPathed, struct MC3DModel*), const char* path, float maxsize);
    fundef(addModelNamed, struct MC3DModel*), const char* name, float maxsize);
    fundef(addModelNamedAtIndex, struct MC3DModel*), const char* name, float maxsize, int index);
    fundef(removeCurrentModel, void));
    //use default if names/name is null
    fundef(addSkyboxNamed, void), const char* names[6]);
    fundef(addSkysphereNamed, void), const char* name);
    fundef(removeCurrentSkybox, void));
    fundef(removeCurrentSkysph, void));
    fundef(cameraFocusOn, void), MCVector4 vertex);
    fundef(cameraFocusOnModel, void), struct MC3DModel* model);
    fundef(cameraZoomToFitModel, void), struct MC3DModel* model);
    fundef(moveModelToOrigin, void), struct MC3DModel* model);
    fundef(setDeviceRotationMat3, void), float mat3[9]);
    fundef(setCameraRotateMode, void), MCCameraRotateMode mode);
    fundef(printDebugInfo, void));
    fundef(release, void));
};

constructor(MCDirector));

alias(MCDirector);

//return -1 or positive fps number
//please update your view only when it is not -1
//fun(MCDirector, int, drawAll, voida);

//fun(MCDirector, void, setupRenderer, MCObject* renderer);

//iOS callbacks
//void onRootViewLoad(void* rootview);
//void onOpenExternalFile(const char* filepath);
//void onReceiveMemoryWarning();
//void onSetupGL(unsigned windowWidth, unsigned windowHeight, const char** filePathArray);
//void onTearDownGL();
//void onUpdate();
//void onDraw();

#endif /* MCDirector_h */
