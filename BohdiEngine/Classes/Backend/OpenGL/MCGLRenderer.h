//
//  MCGLShader.h
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

/*
 Notice:
 
 You must set the window system on both platform
 as below: (RGBA=8888 Depth=24bit)
 
 //iOS
 <GLKViewController>
 view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
 view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
 
 //Android
 <GLSurfaceView>
 setEGLConfigChooser(8, 8, 8, 8, 24, 0);
 setEGLContextClientVersion(3);
 */

#ifndef MCGLRenderer_h
#define MCGLRenderer_h

#include "monkc_export.h"
#include "MCGLContext.h"

//including MCRenderer.h

class(MCGLRenderer, MCObject,
    GLenum useage;
    GLenum drawMode;
    MCGLContext* skycontext;
    MCGLContext* context;
);
//life cycle
fun(MCGLRenderer, void, bye, voida);
fun(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
fun(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
fun(MCGLRenderer, MCGLRenderer*, initWithDefaultShader, voida);

#endif /* MCGLRenderer_h */
