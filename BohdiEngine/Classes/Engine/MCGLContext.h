//
//  MCGLContext.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCGLContext_h
#define MCGLContext_h

#include "monkc.h"
#include "MCMath.h"
#include "MCGLBase.h"
#include "MCTexture.h"
#include "MCMaterial.h"
#include "MCGLShader.h"
#include "MCClock.h"
#include "MCMesh.h"

#define MAX_VATTR_NUM     100
#define MAX_UNIFORM_NUM   100

class(MCGLContext, MCObject,
      MCGLShader* shader;
      double cameraRatio;
      MCDrawMode drawMode;
);

method(MCGLContext, void, bye, voida);

//texture
method(MCGLContext, void, loadTexture, MCTexture* tex, const char* samplerName);
method(MCGLContext, void, loadMaterial, MCMaterial* mtl);
method(MCGLContext, void, loadMesh, MCMesh* meth);
method(MCGLContext, void, drawMesh, MCMesh* meth);

//Global
utility(MCGLContext, MCBool, isFeatureOn, MCGLFeature feature);
utility(MCGLContext, void, featureSwith, MCGLFeature feature, MCBool onOrOff);
utility(MCGLContext, void, flushCommandAsync, voida);
utility(MCGLContext, void, flushCommandBlock, voida);
utility(MCGLContext, void, clearScreen, voida);
utility(MCGLContext, void, clearScreenWithColor, MCColorf color);
utility(MCGLContext, void, clearDepthBuffer, voida);
utility(MCGLContext, void, clearStencilBuffer, voida);
utility(MCGLContext, void, setClearScreenColor, MCColorf color);
utility(MCGLContext, void, setPointSize, double pointsize);
utility(MCGLContext, void, setLineWidth, double linewidth);
utility(MCGLContext, void, setFrontCounterClockWise, MCBool isCCW);
utility(MCGLContext, void, cullFace, MCGLFace face);
utility(MCGLContext, void, cullBackFace, voida);
//Texture
utility(MCGLContext, MCUInt, getIdleTextureUnit, voida);
utility(MCGLContext, MCUInt, getMaxTextureUnits, voida);
utility(MCGLContext, void, generateTextureId, MCUInt* tid);
utility(MCGLContext, void, activeTextureUnit, MCUInt index);
utility(MCGLContext, void, bindCubeTexture, MCUInt tid);
utility(MCGLContext, void, bind2DTexture, MCUInt tid);
utility(MCGLContext, void, unbind2DTextures, voida);
utility(MCGLContext, void, rawdataToTexbuffer, MCTexture* tex, GLenum textype);
utility(MCGLContext, void, setupTexParameter, MCTexture* tex, GLenum textype);

//Alpha Blend
utility(MCGLContext, void, enableTransparency, MCBool enable);
//Z-Fighting
utility(MCGLContext, void, enablePolygonOffset, MCBool enable);

//Frame Rate (FPS)
utility(MCGLContext, int, tickFPS, MCClock* clock);

//Shader
utility(MCGLContext, MCBool, compileShader, GLuint* shader, GLenum type, const GLchar *source, const GLchar *version);
utility(MCGLContext, int, linkProgram, GLuint prog);
utility(MCGLContext, int, validateProgram, GLuint prog);

//Viewport
utility(MCGLContext, void, setViewport, int x, int y, int width, int height);
utility(MCGLContext, void, setScissor, int x, int y, int width, int height);

#endif /* MCGLContext_h */
