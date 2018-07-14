//
//  MCRenderer.c
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//

#include "MCRenderer.h"

oninit(MCRenderer)
{
    if (init(MCObject)) {
        obj->render = new(MCGLRenderer);
        //obj->drawMode = MCTriAngles;
        return obj;
    }
    return null;
}

//life cycle
method(MCRenderer, void, bye, voida)
{
    release(obj->render);
}

method(MCRenderer, MCRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode)
{
    MCGLRenderer_initWithShaderCodeString(obj->render, vcode, fcode);
    return obj;
}

method(MCRenderer, MCRenderer*, initWithShaderFileName, const char* vshader, const char* fshader)
{
    MCGLRenderer_initWithShaderFileName(obj->render, vshader, fshader);
    return obj;
}

method(MCRenderer, MCRenderer*, initWithDefaultShader, voida)
{
    MCGLRenderer_initWithDefaultShader(obj->render, 0);
    return obj;
}

//draw
method(MCRenderer, void, updateScene, MC3DScene* scene)
{
    MCGLRenderer_updateScene(obj->render, scene);
}

method(MCRenderer, void, drawScene, MC3DScene* scene)
{
    MCGLRenderer_drawScene(obj->render, scene);
}

//property
method(MCRenderer, MCDrawMode, getDrawMode, voida)
{
    return (MCDrawMode)obj->render->drawMode;
}

method(MCRenderer, void, setDrawMode, MCDrawMode mode)
{
    obj->render->drawMode = (GLenum)mode;
}

onload(MCRenderer)
{
    if (load(MCObject)) {
        //life cycle
        binding(MCRenderer, void, bye, voida);
        binding(MCRenderer, MCRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
        binding(MCRenderer, MCRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
        binding(MCRenderer, MCRenderer*, initWithDefaultShader, voida);
        //draw
        binding(MCRenderer, void, updateScene, MC3DScene* scene);
        binding(MCRenderer, void, drawScene, MC3DScene* scene);
        //property
        binding(MCRenderer, MCDrawMode, getDrawMode, voida);
        binding(MCRenderer, void, setDrawMode, MCDrawMode mode);
        return cla;
    }
    return null;
}
