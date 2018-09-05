//
// Created by 孙御礼 on 7/28/2018 AD.
//

#include "BEEngineCAPI.h"

#include "monkc_export.h"
#include "beengine_export.h"

static MCDirector* director = null;
static MCObject* renderer = null;
static BECubeTextureData* cubtex = null;

void onAppStart()
{
    if (cubtex == null) {
        const char* names[6] = {"right","left","top","bottom","back","front"};
        cubtex = BECubeTextureData_newWithFaces(names);
    }
}

void onRootViewLoad(void* rootview)
{
    //put the test code into Testbed.c
    //starttest();
}

void onOpenExternalFile(const char* filepath)
{
    MC3DModel* model = ff(new(MC3DModel), initWithFilePathColor, filepath,
                          (MCColorf){MCFloatF(1.0), MCFloatF(1.0), MCFloatF(1.0), MCFloatF(1.0)});
    ff(director->lastScene->rootnode, setAllVisible, false);
    ff(director, addModel, model);
}

//static void asyncReadSkybox()
//{
//    ff(director->lastScene, loadSkybox, 0);
//}

void openFile(const char* filename)
{
    //model
    MC3DModel* model = ff(new(MC3DModel), initWithFileNameColor, filename,
                          (MCColorf){MCFloatF(0.8), MCFloatF(0.8), MCFloatF(0.8), MCFloatF(1.0)});
    if (model) {
        debug_log("Create MC3DModel success:%s\n", model->name);

        MCDirector_addModel(director, model, MCFloatF(10.0));
        MCDirector_cameraFocusOn(director, MCVector4Make(0, -10 * 0.5, 0, 10 * 2.0));

    } else {
        error_log("Can not create MC3DModel:%s\n", filename);
        exit(-1);
        //throw(MC3DModel_ERROR);
    }
}

void openFileAndExitThread(void* arg)
{
//    try {
//        onOpenFile(filename);
//        ff(director->lastScene->rootnode, setAllVisible, true);
//    }catch(MC3DModel_ERROR) {
//        error_log("MC3DModel_ERROR occur exit the process!");
//    }finally{
//        exit(-1);
//    }

    MCString* filename = cast(MCString*, arg);

    openFile(filename->buff);
    ff(director->lastScene->rootnode, setAllVisible, true);

    release(filename);
    MCThread_exitWithStatus((void*)200);
}

void openFileAsync(const char* filename)
{
    MCString* str = MCString_newWithCString(filename);
    ff(director->modelThread, initWithFPointerArgument, openFileAndExitThread, (void*)str);
    ff(director->modelThread, start, 0);

    //MCThread_joinThread(director->modelThread->tid);
}

void onReceiveMemoryWarning()
{
    error_log("Receive Memory Warning\n");
    MC3DScene* mainScene = director->lastScene;
    if (mainScene != null && mainScene->rootnode != null) {
        ff(mainScene->rootnode, cleanUnvisibleChild, 0);
    }
}

void onOpenFile(const char* filename)
{
    if (director == null) {
        onSetupGL(640, 480);
    }

    if (filename != null) {
        //if (director->lastScene->skyboxRef != null) {
        //ff(director->skyboxThread, initWithFPointerArgument, asyncReadSkybox, null);
        //ff(director->skyboxThread, start, 0);
        //}

        //openFileAsync(filename);
        openFile(filename);
    }
}

void onSetupGL(int windowWidth, int windowHeight)
{
    debug_log("onSetupGL called: width=%d height=%d\n", windowWidth, windowHeight);
    //MCLogTypeSet(MC_SILENT);

    if (director == null) {
        debug_log("onSetupGL create director\n");
        renderer = MCGLRenderer_initWithDefaultShader(new(MCGLRenderer), 0);
        director = new(MCDirector);
        director->renderer = renderer;
        director->currentWidth = windowWidth;
        director->currentHeight = windowHeight;
        debug_log("onSetupGL director created\n");

        //scene1
        MC3DScene* mainScene = ff(new(MC3DScene), initWithWidthHeight, director->currentWidth, director->currentHeight);
        debug_log("onSetupGL main scene created current screen size: %dx%d\n", windowWidth, windowHeight);

//        mainScene->sky = getSkyboxOn();
//
//        if (cubtex != null && mainScene->skyboxShow) {
//            MCSkybox* skybox = MCSkybox_initWithCubeTexture(0, new(MCSkybox), cubtex, MCRatioMake(windowWidth, windowHeight));
//            mainScene->skyboxRef = skybox;
//        }

        mainScene->mainCamera->R_value = 20;
        mainScene->mainCamera->tht = 60;
        mainScene->mainCamera->fai = 45;

        superof(mainScene)->nextResponder = (MCObject*)director;

        ff(director, pushScene, mainScene);

        //kick off
        MCDirector_updateAll(director, 0);
        //MCDirector_drawAll(director, 0);

        debug_log("onSetupGL main scene pushed into director\n");
    }
}

void onTearDownGL()
{
    release(director);
    director = null;
}

void onUpdate(double roll, double yaw, double pitch, double w)
{
    //printf("sensor data: roll=%f yaw=%f pitch=%f\n", roll, yaw, pitch);
    //MCLogTypeSet(MC_SILENT);
    if (director != null) {

//        if (computed(director->lastScene, isDrawSky)) {
//            if (director->currentWidth < director->currentHeight) {
//                MCSkyboxCamera_setAttitude(0, director->lastScene->skyboxRef->camera,
//                                           MCFloatF(roll*360), MCFloatF((pitch-1)*45));
//            }else{
//                MCSkyboxCamera_setAttitude(0, director->lastScene->skyboxRef->camera,
//                                           MCFloatF(pitch*360), MCFloatF((roll-1)*45));
//            }
//        }

        MCDirector_updateAll(director, 0);
    }
}

int onDraw()
{
    int fps = -1;
    if (director && director->renderer && director->lastScene) {
        ff(director->renderer, drawScene, director->lastScene);
        //fps = MCDirector_drawAll(director, 0);
    }

    MCLogTypeSet(MC_DEBUG);
    return fps;
}

//gesture callback
void onGestureSwip()
{
    if (director != null && director->lastScene != null) {
        //MCDirector_popScene(0, director, 0);
    }
}

void onGesturePan(double x, double y)
{
    MCCamera* camera = director->lastScene->mainCamera;

    if (director != null && director->lastScene != null && camera != null) {
        double sign = camera->isReverseMovement == true? -1.0f : 1.0f;
        MCCamera_move(camera, MCFloatF(x*sign), MCFloatF(y*sign));

//        if (camera->isLockRotation == true) {
//            double factor = 0.01;
//            MCCamera_fucus(camera, MCFloatF(x*sign*factor), MCFloatF(y*sign*factor));
//        }else{
//            MCCamera_move(camera, MCFloatF(x*sign), MCFloatF(y*sign));
//        }
    }
}

void cameraDistanceScale(double scale, double min, double max)
{
    double limited_scale = MAX(min, MIN(scale, max));
    MCCamera* camera = director->lastScene->mainCamera;
    if (director != null && director->lastScene != null && camera != null) {
        MCCamera_distanceScale(camera, MCFloatF(1.0/limited_scale));
    }
}

static float pinch_scale = 1.0;
void onGesturePinch(double scale)
{
    pinch_scale *= scale;
    cameraDistanceScale(pinch_scale, 0.1, 100);
}

void onResizeScreen(int windowWidth, int windowHeight)
{
    if (director != null) {
        ff(director, resizeAllScene, windowWidth, windowHeight);
    }
}

void onStartStopBtn(int startOrStop)
{
    if (director && director->lastScene) {
        director->lastScene->cameraLock = !startOrStop;
    }
}

void cameraTranslate(double x, double y, double z, _Bool incremental)
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        MCVector3 eye = MCVector3Make(x, y, z);
        cam->R_value = MCVector3Length(eye);
        cam->eye = eye;
        MCVector3 v3 = {x, y, z};
        MC3DNode_translateVec3(&cam->Super, &v3, incremental?true:false);
    }
}

void setCameraAutoRotation(_Bool autorotate)
{
    if (autorotate == true) {
        director->lastScene->cameraAutoRotate = true;
    } else {
        director->lastScene->cameraAutoRotate = false;
    }
}

void setDoesDrawWireFrame(_Bool isWiremode)
{
    if (!director || !director->renderer) return;
    if (isWiremode == true) {
        ff(director->renderer, setDrawMode, MCLineStrip);
        //computed(director, contextHandler)->drawMode = MCLineStrip;
    } else {
        ff(director->renderer, setDrawMode, MCTriAngles);
        //computed(director, contextHandler)->drawMode = MCTriAngles;
    }
}

