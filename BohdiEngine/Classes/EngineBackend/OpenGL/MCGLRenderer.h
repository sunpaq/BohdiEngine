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

#include "monkc.h"

#include "MCGLBase.h"
#include "MCGLContext.h"
#include "MCMesh.h"

#include "MC3DScene.h"
#include "MC3DNode.h"
#include "MCCamera.h"
#include "MCMaterial.h"
#include "MCTexture.h"

class(MCGLRenderer, MCObject,
    MCGLContext* skycontext;
    MCGLContext* context;
    MCDrawMode drawMode;
    GLenum     useage;
);
//life cycle
method(MCGLRenderer, void, bye, voida);
method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
method(MCGLRenderer, MCGLRenderer*, initWithDefaultShader, voida);
//draw
method(MCGLRenderer, void, updateScene, MC3DScene* scene);
method(MCGLRenderer, void, drawScene, MC3DScene* scene);

#endif /* MCGLRenderer_h */
