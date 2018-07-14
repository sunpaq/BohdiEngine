//
//  MCRenderer.h
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//

#ifndef MCRenderer_h
#define MCRenderer_h

#define USE_GL

#include "monkc.h"
#include "MC3DBase.h"

#ifdef USE_GL
#include "MCGLRenderer.h"
class(MCRenderer, MCObject,
    MCGLRenderer* render;
    //MCDrawMode drawMode;
);
#endif

//life cycle
method(MCRenderer, void, bye, voida);
method(MCRenderer, MCRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
method(MCRenderer, MCRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
method(MCRenderer, MCRenderer*, initWithDefaultShader, voida);
//draw
method(MCRenderer, void, updateScene, MC3DScene* scene);
method(MCRenderer, void, drawScene, MC3DScene* scene);
//property
method(MCRenderer, MCDrawMode, getDrawMode, voida);
method(MCRenderer, void, setDrawMode, MCDrawMode mode);

#endif /* MCRenderer_h */
