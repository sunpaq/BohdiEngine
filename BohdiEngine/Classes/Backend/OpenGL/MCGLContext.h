//
//  MCGLContext.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCGLContext_h
#define MCGLContext_h

#include "monkc_export.h"

#include "MCTexture.h"
#include "MCMaterial.h"
#include "MC3DBase.h"

#include "MCGLBase.h"
#include "MCGLShader.h"

#define MAX_VATTR_NUM     100
#define MAX_UNIFORM_NUM   100

class(MCGLContext, MCObject,
      MCGLShader* shader;
      double cameraRatio;
);

fun(MCGLContext, void, bye, voida);

//texture
fun(MCGLContext, GLuint, loadTexture, MCTexture* tex, const char* samplerName, GLint unit);
fun(MCGLContext, void, loadMaterial, MCMaterial* mtl);

//Global
util(MCGLContext, MCBool, isFeatureOn, MCGLFeature feature);
util(MCGLContext, void, featureSwith, MCGLFeature feature, MCBool onOrOff);
util(MCGLContext, void, flushCommandAsync, voida);
util(MCGLContext, void, flushCommandBlock, voida);
util(MCGLContext, void, clearScreen, voida);
util(MCGLContext, void, clearScreenWithColor, MCColorf color);
util(MCGLContext, void, clearDepthBuffer, voida);
util(MCGLContext, void, clearStencilBuffer, voida);
util(MCGLContext, void, setClearScreenColor, MCColorf color);
util(MCGLContext, void, setPointSize, double pointsize);
util(MCGLContext, void, setLineWidth, double linewidth);
util(MCGLContext, void, setFrontCounterClockWise, MCBool isCCW);
util(MCGLContext, void, cullFace, MCGLFace face);
util(MCGLContext, void, cullBackFace, voida);
//Texture
util(MCGLContext, MCUInt, getIdleTextureUnit, voida);
util(MCGLContext, MCUInt, getMaxTextureUnits, voida);
util(MCGLContext, void, generateTextureId, GLuint* tid);
util(MCGLContext, void, activeTextureUnit, GLuint index);
util(MCGLContext, void, bindCubeTexture, GLint tid);
util(MCGLContext, void, bind2DTexture, GLuint tid);
util(MCGLContext, void, unbind2DTextures, voida);
util(MCGLContext, void, rawdataToTexbuffer, MCTexture* tex, GLenum textype);
util(MCGLContext, void, setupTexParameter, MCTexture* tex, GLenum textype);

//Alpha Blend
util(MCGLContext, void, enableTransparency, MCBool enable);
//Z-Fighting
util(MCGLContext, void, enablePolygonOffset, MCBool enable);

//Frame Rate (FPS)
util(MCGLContext, int, tickFPS, MCClock* clock);

//Shader
util(MCGLContext, MCBool, compileShader, GLuint* shader, GLenum type, const GLchar *source, const GLchar *version);
util(MCGLContext, int, linkProgram, GLuint prog);
util(MCGLContext, int, validateProgram, GLuint prog);

//Viewport
util(MCGLContext, void, setViewport, int x, int y, int width, int height);
util(MCGLContext, void, setScissor, int x, int y, int width, int height);

#endif /* MCGLContext_h */
