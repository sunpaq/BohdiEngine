//
//  MC3DiOS.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/6.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DiOS_h
#define MC3DiOS_h

//App lifecycle
void onAppStart();

//User Settings
int getSkyboxOn();

//GL callback
void onRootViewLoad(void* rootview);
void onReceiveMemoryWarning();
void onSetupGL(int windowWidth, int windowHeight);
void onOpenFile(const char* filename);
void onTearDownGL();
void onUpdate(double roll, double yaw, double pitch);
int onDraw();

//File callback
void onOpenExternalFile(const char* filepath);
void onOpenFile(const char* filename);
void onOpenFileAsync(const char* filename);
void onOpenFileAndExitThread(const char* filename);

//Gesture callback
void onGestureSwip();
void onGesturePan(double x, double y);
void onGesturePinch(double scale);

//Orientation Handling
void onResizeScreen(int windowWidth, int windowHeight);
void onStartStopBtn(int startOrStop);

//MCGL Errors
void MCGLError(const char* errmsg);
void MCGLStartLoading();
void MCGLStopLoading();

typedef enum {
    MC3DiOS_CameraLookAt,
    MC3DiOS_CameraLookAtDelta,
    MC3DiOS_CameraRadius,
    MC3DiOS_CameraRadiusDelta,
    MC3DiOS_CameraRadiusPercent,
    MC3DiOS_CameraRadiusPDelta,
    MC3DiOS_CameraAngels,
    MC3DiOS_CameraAngelsDelta,
    MC3DiOS_GetCurrent,
    MC3DiOS_LockRotation
} MC3DiOS_CameraCmdType;

typedef struct {
    MC3DiOS_CameraCmdType type;
    double lookatX;
    double lookatY;
    double lookatZ;
    double radius;
    double rpercent;
    double tht;
    double fai;
    int    lockRotation;
} MC3DiOS_CameraCmd;

void cameraCommand(MC3DiOS_CameraCmd* cmd);

#endif /* MC3DiOS_h */
