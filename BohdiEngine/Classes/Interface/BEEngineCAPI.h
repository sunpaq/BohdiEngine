//
// Created by 孙御礼 on 7/28/2018 AD.
//

#ifndef MONKC4ANDROID_BEENGINECAPI_H
#define MONKC4ANDROID_BEENGINECAPI_H

void onAppStart();
void onRootViewLoad(void* rootview);
void onOpenExternalFile(const char* filepath);
void onReceiveMemoryWarning();
void onOpenFile(const char* filename);
void onSetupGL(int windowWidth, int windowHeight);
void onUpdate(double roll, double yaw, double pitch, double w);
int onDraw();
void onTearDownGL();
void onGestureSwip();
void onGesturePan(double x, double y);
void onGesturePinch(double scale);
void onResizeScreen(int windowWidth, int windowHeight);
void onStartStopBtn(int startOrStop);

void openFile(const char* filename);
void openFileAndExitThread(void* arg);
void openFileAsync(const char* filename);
void cameraDistanceScale(double scale, double min, double max);
void cameraTranslate(double x, double y, double z, _Bool incremental);

void setCameraAutoRotation(_Bool autorotate);
void setDoesDrawWireFrame(_Bool isWiremode);

#endif //MONKC4ANDROID_BEENGINECAPI_H
