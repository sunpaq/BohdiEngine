#include "MCCamera.h"
#include "MCLog.h"

fun(Radius, double)) as(MCCamera)
    return (it->R_value * it->R_percent);
}

fun(normal, MCMatrix3)) as(MCCamera)
    MCMatrix3 mat3 = MCMatrix4GetMatrix3(cpt(viewMatrix));
    MCMatrix3 nor = MCMatrix3InvertAndTranspose(mat3, null);
    return nor;
}

/*
 Initialize Status:
 
 view space coordinate orientation equal to device attitude (iphone on the table, x-right z-up)
 world space coordinate orientation equal monitor attitude (LCD on the desk, x-right -z forward)
 */

fun(viewMatrix, MCMatrix4)) as(MCCamera)
    double r = it->Radius(it);
    if (it->rotateMode == MCCameraRotateAroundModelManual) {
        it->transformSelfByEularAngle(it, it->lookat, r, it->fai, it->tht);
    }
    else if (it->rotateMode == MCCameraRotateAroundModelByGyroscope) {
        cast(it, MC3DNode)->transform = MCMatrix4MakeLookAt(0, 0, r, 0, 0, 0, 0, 1, 0);
    }
    else if (it->rotateMode == MCCameraRotateAroundModelByGyroscopeReverse) {
        cast(it, MC3DNode)->transform = MCMatrix4MakeLookAt(0, 0, r, 0, 0, 0, 0, 1, 0);
        cast(it, MC3DNode)->viewtrans = MCMatrix4Invert(cast(it, MC3DNode)->viewtrans, null);
    }
    else if (it->rotateMode == MCCameraFixedAtOrigin) {
        cast(it, MC3DNode)->transform = MCMatrix4Identity;
        cast(it, MC3DNode)->viewtrans = MCMatrix4Identity;
    }
    //default
    else {
        //do nothing
    }
    MCMatrix4 view = MCMatrix4Multiply(cast(it, MC3DNode)->transform, cast(it, MC3DNode)->viewtrans);
    return view;
}

fun(projectionMatrix, MCMatrix4)) as(MCCamera)
    double R = it->Radius(it);
    double DoF = it->depth_of_field;
    double far = R + DoF;
    double near = R - DoF;
    if (near <= 0) {
        near = MCLensIphone29mm;
    }
    return MCMatrix4MakePerspective(MCDegreesToRadians(it->field_of_view), it->ratio, near, far);
}

fun(currentPosition, MCVector3)) as(MCCamera)
    if (it->rotateMode != MCCameraRotateAroundModelManual) {
        double r = it->Radius(it);
        MCMatrix4 viewtrans = cast(it, MC3DNode)->viewtrans;
        MCMatrix4 transform = cast(it, MC3DNode)->transform;
        it->eye = MCGetEyeFromRotationMat4(viewtrans, r);
        it->R_value = MCVector3Length(MCGetTranslateFromCombinedMat4(transform));
        it->R_percent = 1.0;
    }
    return it->eye;
}

fun(rotationMat3, MCMatrix3)) as(MC3DNode)
    return MCMatrix4GetMatrix3(it->transform);
}

fun(printDebugInfo, void)) as(MCCamera)
    debug_log("MCCamera: lookat=%.2f/%.2f/%.2f R_value=%.2f R_percent=%.3f\n",
              it->lookat.x, it->lookat.y, it->lookat.z, it->R_value, it->R_percent);
}

fun(reset, void)) {
    //init(MCCamera);
}

fun(transformWorld, void), MCMatrix4* mat4) as(MC3DNode)
    it->viewtrans = *mat4;
}

fun(transformSelf, void), MCMatrix4* mat4) as(MC3DNode)
    it->transform = *mat4;
}

fun(transformSelfByEularAngle, void), MCVector3 lookat, double R, double fai, double tht) as(MCCamera)
    //double R = it->Radius(it);
    cast(it, MC3DNode)->transform = MCMatrix4MakeLookAtByEulerAngle_EyeUp(it->lookat, R,
                                                            it->fai, it->tht,
                                                            &it->eye, &it->up);
}

//override
fun(willDraw, void), MCMatrix4* projection, MCMatrix4* view, MCMatrix4* model) as(MCCamera)
    if (projection) {
        *projection = it->projectionMatrix(it);
    }
    
    if (view) {
        *view = it->viewMatrix(it);
    }

//    MCGLUniformData data;
//
//    data.mat4 = cpt(viewMatrix);
//    MCGLShader_updateUniform(ctx->shader, view_view, data);
//
//    data.mat4 = cpt(projectionMatrix);
//    MCGLShader_updateUniform(ctx->shader, view_projection, data);
}

fun(move, void), float deltaFai, float deltaTht) as(MCCamera)
    //angle with x+
    if (it->isReverseMovement) {
        it->fai += deltaFai;   //Left
        it->tht += deltaTht;   //Up
    }else{
        it->fai -= deltaFai;   //Left
        it->tht -= deltaTht;   //Up
    }
}

fun(fucus, void), float deltaX, float deltaY) as(MCCamera)
    //angle with x+
    if (it->isReverseMovement) {
        it->lookat.x += deltaX;
        it->lookat.y += deltaY;
    }else{
        it->lookat.x -= deltaX;
        it->lookat.y -= deltaY;
    }
}

fun(pull, void), float deltaR) as(MCCamera)
    it->R_value += deltaR;
}

fun(distanceScale, void), float scale) as(MCCamera)
    it->R_percent = scale;
}

fun(setRotationMat3, void), float mat3[9]) as(MCCamera)
    MCMatrix4 m4 = (MCMatrix4) {
        mat3[0], mat3[1], mat3[2], 0,
        mat3[3], mat3[4], mat3[5], 0,
        mat3[6], mat3[7], mat3[8], 0,
        0, 0, 0, 1
    };
    cast(it, MC3DNode)->viewtrans = m4;
}

fun(release, void)) as(MC3DNode)
    it->release(it);
}

constructor(MCCamera), unsigned width, unsigned height) {
    MC3DNode(any);
    as(MCCamera)
        it->ratio = MCRatioHDTV16x9;
        it->field_of_view = MCLensIphoneVideoViewAngle;
        it->depth_of_field = 100;

        //local spherical coordinate
        it->R_value = 100;
        it->R_percent = 1.0;
        it->tht = 90;
        it->fai = 0;
    
        it->lookat = MCVector3Make(0,0,0);
        it->eye = MCVector3Make(0.0,it->R_value,0.0);
        it->up  = MCVector3Make(0.0,0.0,-1.0);

        //transform
        it->isReverseMovement = false;
        it->rotateMode = MCCameraRotateAroundModelManual;
        //setting camera
        it->ratio = MCRatioMake(width, height);
        it->R_value = 5;
    }
    dynamic(MCCamera)
        funbind(Radius);
        funbind(normal);
        funbind(viewMatrix);
        funbind(projectionMatrix);
        funbind(currentPosition);
        funbind(rotationMat3);
        funbind(transformWorld);
        funbind(transformSelf);
        funbind(transformSelfByEularAngle);
        funbind(move);
        funbind(fucus);
        funbind(pull);
        funbind(reset);
        funbind(distanceScale);
        funbind(setRotationMat3);
        funbind(printDebugInfo);
        funbind(willDraw);
        funbind(release);
    }
    return any;
}

