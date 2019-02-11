#ifndef _MCCamera
#define _MCCamera

//#include "monkc_export.h"
#include "MC3DBase.h"
#include "MC3DNode.h"

typedef enum {
    MCCameraFixedAtOrigin = 0,
    MCCameraRotateAroundModelManual,
    MCCameraRotateAroundModelByGyroscope,
    MCCameraRotateAroundModelByGyroscopeReverse
} MCCameraRotateMode;

structure(MCCamera, MC3DNode)
    double ratio;
    double depth_of_field;
    double field_of_view;
    //double max_distance;
    MCVector3 lookat;
    MCVector3 eye;
    MCVector3 up;

    //local spherical coordinate
    //R[0,unlimited) tht[0, 180.0), fai[0, 360.0)
    double R_value;
    double R_percent;
    double tht;
    double fai;

    bool isReverseMovement;
    MCCameraRotateMode rotateMode;

    fundef(Radius, double));
    fundef(normal, MCMatrix3));
    fundef(viewMatrix, MCMatrix4));
    fundef(projectionMatrix, MCMatrix4));
    fundef(currentPosition, MCVector3));
    fundef(rotationMat3, MCMatrix3));
    fundef(transformWorld, void), MCMatrix4* mat4);
    fundef(transformSelf, void), MCMatrix4* mat4);
    fundef(transformSelfByEularAngle, void), MCVector3 lookat, double R, double fai, double tht);
    fundef(move, void), float deltaFai, float deltaTht);
    fundef(fucus, void), float deltaX, float deltaY);
    fundef(pull, void), float deltaR);
    fundef(reset, void));
    fundef(distanceScale, void), float scale);
    fundef(setRotationMat3, void), float mat3[9]);
    fundef(printDebugInfo, void));
    fundef(willDraw, void), MCMatrix4* projection, MCMatrix4* view, MCMatrix4* model);
    fundef(release, void));
};

constructor(MCCamera), unsigned width, unsigned height);

alias(MCCamera);

#define MCLensStandard50mm (0.050)
#define MCLensWide24mm     (0.024)
#define MCLensLong100mm    (0.100)
#define MCLensLong200mm    (0.200)
#define MCLensLong2000mm   (2.000)
#define MCLensIphone29mm   (0.029)
#define MCLensIphone28mm   (0.028)

#define MCRatioCameraFilm3x2    (3.0/2.0)
#define MCRatioOldTV4x3         (4.0/3.0)
#define MCRatioHDTV16x9         (16.0/9.0)
#define MCRatioCinema239x100    (2.39/1.0)
#define MCRatioMake(w, h)       ((double)w / (double)h)

#define MCLensStandard50mmViewAngle (45.0)
#define MCLensIphone29mmViewAngle (73.44)
#define MCLensIphone28mmViewAngle (75.38)
#define MCLensIphoneVideoViewAngle (58.632149)
#endif
