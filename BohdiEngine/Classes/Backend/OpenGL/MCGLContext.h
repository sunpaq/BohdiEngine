//
//  MCGLContext.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCGLContext_h
#define MCGLContext_h

//#include "monkc_export.h"

#include "MCObject.h"
#include "MCTexture.h"
#include "MCMaterial.h"
#include "MC3DBase.h"
#include "MCClock.h"
#include "MCGLBase.h"
#include "MCGLShader.h"

#define MAX_VATTR_NUM     100
#define MAX_UNIFORM_NUM   100

structure(MCGLContext, MCObject)
    struct MCGLShader* shader;
    double cameraRatio;

    fundef(release, void));
    //texture
    fundef(loadTexture, GLuint), struct MCTexture* tex, const char* samplerName, GLint unit);
    fundef(loadMaterial, void), struct MCMaterial* mtl);
};

constructor(MCGLContext));

alias(MCGLContext);

//Global
bool MCGLContext_isFeatureOn(MCGLFeature feature);
void MCGLContext_featureSwith(MCGLFeature feature, bool onOrOff);
void MCGLContext_flushCommandAsync(void);
void MCGLContext_flushCommandBlock(void);
void MCGLContext_clearScreen(void);
void MCGLContext_clearScreenWithColor(MCColorf color);
void MCGLContext_clearDepthBuffer(void);
void MCGLContext_clearStencilBuffer(void);
void MCGLContext_setClearScreenColor(MCColorf color);
void MCGLContext_setPointSize(double pointsize);
void MCGLContext_setLineWidth(double linewidth);
void MCGLContext_setFrontCounterClockWise(bool isCCW);
void MCGLContext_cullFace(MCGLFace face);
void MCGLContext_cullBackFace(void);
//Texture
unsigned MCGLContext_getIdleTextureUnit(void);
unsigned MCGLContext_getMaxTextureUnits(void);
void MCGLContext_generateTextureId(GLuint* tid);
void MCGLContext_activeTextureUnit(GLuint index);
void MCGLContext_bindCubeTexture(GLint tid);
void MCGLContext_bind2DTexture(GLuint tid);
void MCGLContext_unbind2DTextures(void);
void MCGLContext_rawdataToTexbuffer(struct MCTexture* tex, GLenum textype);
void MCGLContext_setupTexParameter(struct MCTexture* tex, GLenum textype);

//Alpha Blend
void MCGLContext_enableTransparency(bool enable);
//Z-Fighting
void MCGLContext_enablePolygonOffset(bool enable);

//Frame Rate (FPS)
int MCGLContext_tickFPS(struct MCClock* clock);

//Shader
bool MCGLContext_compileShader(GLuint* shader, GLenum type, const GLchar *source, const GLchar *version);
int MCGLContext_linkProgram(GLuint prog);
int MCGLContext_validateProgram(GLuint prog);

//Viewport
void MCGLContext_setViewport(int x, int y, int width, int height);
void MCGLContext_setScissor(int x, int y, int width, int height);

#endif /* MCGLContext_h */
