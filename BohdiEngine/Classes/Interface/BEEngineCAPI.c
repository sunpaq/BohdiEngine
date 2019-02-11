//
// Created by 孙御礼 on 7/28/2018 AD.
//

#include "BEEngineCAPI.h"

#include "MCObject.h"
#include "beengine_export.h"
#include "MCLog.h"
#include "MCThread.h"
#include "MCGLRenderer.h"

static struct MCDirector* director = null;
static struct MCObject* renderer = null;
static struct BECubeTextureData* cubtex = null;

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
    struct MC3DModel* model = new(MC3DModel);
    ff(model, initWithFilePathColor), filepath, (MCColorf){1.0, 1.0, 1.0, 1.0});
    ff(director->lastScene->rootnode, setAllVisible), false);
    ff(director, addModel), model);
}

//static void asyncReadSkybox()
//{
//    ff(director->lastScene, loadSkybox, 0);
//}

void openFile(const char* filename)
{
    //model
    struct MC3DModel* model = new(MC3DModel);
    ff(model, initWithFileNameColor), filename, (MCColorf){0.8, 0.8, 0.8, 1.0});
    if (model) {
        debug_log("Create MC3DModel success:%s\n", model->name);
        director->addModel(director, model, 10.0);
        director->cameraFocusOn(director, MCVector4Make(0, -10 * 0.5, 0, 10 * 2.0));

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

    struct MCString* filename = cast(arg, MCString);

    openFile(filename->buff);
    ff(director->lastScene->rootnode, setAllVisible), true);

    Release(filename);
    MCThread_exitWithStatus((void*)200);
}

void openFileAsync(const char* filename)
{
    struct MCString* str = MCString_newWithCString(filename);
    ff(director->modelThread, initWithFPointerArgument), openFileAndExitThread, (void*)str);
    ff(director->modelThread, start), 0);

    //MCThread_joinThread(director->modelThread->tid);
}

void onReceiveMemoryWarning()
{
    error_log("Receive Memory Warning\n");
    struct MC3DScene* mainScene = director->lastScene;
    if (mainScene != null && mainScene->rootnode != null) {
        ff(mainScene->rootnode, cleanUnvisibleChild), 0);
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
        struct MCGLRenderer* r = new(MCGLRenderer);
        r->initWithDefaultShader(r);
        renderer = (obj)r;
        director = new(MCDirector);
        director->renderer = renderer;
        director->currentWidth = windowWidth;
        director->currentHeight = windowHeight;
        debug_log("onSetupGL director created\n");

        //scene1
        struct MC3DScene* mainScene = MC3DScene(alloc(MC3DScene), director->currentWidth, director->currentHeight);
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

        //superof(mainScene)->nextResponder = (MCObject*)director;
        ff(director, pushScene), mainScene);

        //kick off
        director->updateAll(director);

        debug_log("onSetupGL main scene pushed into director\n");
    }
}

void onTearDownGL()
{
    Release(director);
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

        director->updateAll(director);
    }
}

int onDraw()
{
    int fps = -1;
    if (director && director->renderer && director->lastScene) {
        ff(director->renderer, drawScene), director->lastScene);
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
    struct MCCamera* camera = director->lastScene->mainCamera;

    if (director != null && director->lastScene != null && camera != null) {
        double sign = camera->isReverseMovement == true? -1.0f : 1.0f;
        camera->move(camera, x*sign, y*sign);

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
    struct MCCamera* camera = director->lastScene->mainCamera;
    if (director != null && director->lastScene != null && camera != null) {
        camera->distanceScale(camera, 1.0/limited_scale);
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
        ff(director, resizeAllScene), windowWidth, windowHeight);
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
    struct MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        MCVector3 eye = MCVector3Make(x, y, z);
        cam->R_value = MCVector3Length(eye);
        cam->eye = eye;
        MCVector3 v3 = {x, y, z};
        cast(cam, MC3DNode)->translateVec3(cam, &v3, incremental?true:false);
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
        ff(director->renderer, setDrawMode), MCLineStrip);
        //computed(director, contextHandler)->drawMode = MCLineStrip;
    } else {
        ff(director->renderer, setDrawMode), MCTriAngles);
        //computed(director, contextHandler)->drawMode = MCTriAngles;
    }
}

