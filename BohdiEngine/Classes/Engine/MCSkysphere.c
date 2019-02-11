//
//  MCSkysphere.c
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#include "MCSkysphere.h"
#include "MC3DShapeBase.h"

fun(release, void)) as(MCSkysphere)
    free(it->vertices);
    free(it->indices);
    Release(it->tex);

    struct MC3DNode* node = &(it->Super);
    node->release(node);
}

fun(initWithBE2DTexture, struct MCSkysphere*), struct BE2DTextureData* tex) as(MCSkysphere)
    Retain(tex);
    it->tex = tex;
    return it;
}

fun(initWithFileName, struct MCSkysphere*), const char* name) as(MCSkysphere)
    struct BE2DTextureData* data = BE2DTextureData_newWithFilename(name);
    if (data) {
        struct MCSkysphere* sph = initWithBE2DTexture(it, data);
        Release(data);
        return sph;
    }
    return null;
}

fun(initWithDefaultFile, struct MCSkysphere*)) as(MCSkysphere)
    return initWithFileName(it, "skysphtex.jpg");
}

fun(getViewMatrix, void), MCMatrix4* mat4) as(MCSkysphere)
    MCMatrix4 m = MCMatrix4MakeLookAt(0, 0, 0,
                                      0, 0,-1,
                                      0, 1, 0);
    MCMatrix4 imat4 = MCMatrix4Invert(it->Super.transform, null);
    *mat4 = MCMatrix4Multiply(m, imat4);
}

fun(getProjectionMatrix, void), MCMatrix4* mat4) as(MCSkysphere)
    *mat4 = MCMatrix4MakePerspective(it->sphCameraAngle,
                                    it->sphCameraRatio,
                                    0.001,
                                    200.0);
}

fun(setRotationMat3, void), float mat3[9]) as(MC3DNode)
    it->rotateMat3(it, mat3, false);
}

fun(setRotationMat4, void), float mat4[16]) as(MC3DNode)
    it->rotateMat4(it, mat4, false);
}

fun(transformSelfByEularAngle, void), double R, double fai, double tht)
{
    //MCMatrix4 lookat = MCMatrix4MakeLookAt(0, 0, 0, 0, 0,-1, 0, 1, 0);
    //double R = var(R_value) * var(R_percent);
    //sobj->transform = MCMatrix4MakeLookAtByEulerAngle_EyeUp(lookat, R, fai, tht, null, null);
}

constructor(MCSkysphere)) {
    MC3DNode(any);
    as(MCSkysphere)
        it->Super.visible = true;
        it->tex = null;
        it->sphViewMatrix = MCMatrix4Identity;
        it->sphProjectionMatrix = MCMatrix4Identity;
        it->sphCameraAngle = M_PI * 0.55;
        it->sphCameraRatio = 9.0/16.0;
    
        it->R_value = 200.0;
        it->R_percent = 1.0;
        it->tht = 0.0;
        it->fai = 0.0;
    
        int nr = 72;
        int nc = 144;
    
        it->vertices_size = sizeof(GLfloat) * nr * nc * 5;
        it->indices_size = sizeof(GLuint) * nr * nc * 2;
        it->vertices = (GLfloat*)malloc(it->vertices_size);
        it->indices = (GLuint*)malloc(it->indices_size);
        it->ic = MCGenerateSkysphere(nr, nc, 100.0, it->vertices, it->indices);
    };
    dynamic(MCSkysphere)
        funbind(release);
        funbind(initWithBE2DTexture);
        funbind(initWithFileName);
        funbind(initWithDefaultFile);
        funbind(setRotationMat3);
        funbind(setRotationMat4);
        funbind(getViewMatrix);
        funbind(getProjectionMatrix);
    };
    return any;
}



