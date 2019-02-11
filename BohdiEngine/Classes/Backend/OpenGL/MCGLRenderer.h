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

//#include "monkc_export.h"
#include "MCObject.h"
#include "MCGLContext.h"
#include "MC3DScene.h"
#include "MCMesh.h"

//including MCRenderer.h

structure(MCGLRenderer, MCObject)
    GLenum useage;
    GLenum drawMode;
    struct MCGLContext* skycontext;
    struct MCGLContext* context;

    //life cycle
    fundef(release, void));
    fundef(initWithShaderCodeString, struct MCGLRenderer*), const char* vcode, const char* fcode);
    fundef(initWithShaderFileName, struct MCGLRenderer*), const char* vshader, const char* fshader);
    fundef(initWithDefaultShader, struct MCGLRenderer*));

    fundef(updateScene, void), struct MC3DScene* scene);
    fundef(drawScene, void), struct MC3DScene* scene);

    fundef(scissorAllScene, void), int x, int y, int width, int height);
    fundef(setDrawMode, void), MCDrawMode mode);
    fundef(getDrawMode, MCDrawMode));

    fundef(drawSkysphere, void), struct MCSkysphere* sphere);
    fundef(drawSkybox, void), struct MCSkybox* skybox);
    fundef(updateLight, void), struct MCLight* light);
    fundef(updateCamera, void), struct MCCamera* cam);
    fundef(drawNode, void), struct MC3DNode* node);
    fundef(drawMesh, void), struct MCMesh* mesh, GLuint texid);

    fundef(initSkybox, void));
    fundef(initSkysphere, void));
};

constructor(MCGLRenderer));

alias(MCGLRenderer);

#endif /* MCGLRenderer_h */
