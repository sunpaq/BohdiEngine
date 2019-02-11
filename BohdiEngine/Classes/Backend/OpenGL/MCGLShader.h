//
//  MCGLShader.h
//  BohdiEngine
//
//  Created by 孙御礼 on 4/27/30 H.
//

#ifndef MCGLShader_h
#define MCGLShader_h

#include "MCGLBase.h"
#include "MCGLDefaultShader.h"
#include "MCObject.h"

#define MAX_UNIFORM_NUM   100

static const char* view_view = "view_view";
static const char* view_projection = "view_projection";
static const char* view_position   = "view_position";

static const char* model_model     = "model_model";
static const char* model_normal    = "model_normal";

static const char* light_ambient   = "light_ambient";
static const char* light_diffuse   = "light_diffuse";
static const char* light_specular  = "light_specular";
static const char* light_color     = "light_color";
static const char* light_position  = "light_position";

static const char* material_ambient   = "material_ambient";
static const char* material_diffuse   = "material_diffuse";
static const char* material_specular  = "material_specular";
static const char* material_dissolve  = "material_dissolve";
static const char* material_shininess = "material_shininess";

structure(MCGLShader, MCObject)
    GLuint pid;

    MCGLUniform uniforms[MAX_UNIFORM_NUM];
    bool uniformsDirty[MAX_UNIFORM_NUM];
    unsigned uniformCount;

    fundef(release, void));
    //shader
    //please cache the location index when you first call the setters
    //then directly pass the location index and pass name null
    fundef(initWithShaderCode, struct MCGLShader*), const char* vcode, const char* fcode,
    const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
    fundef(initWithShaderName, struct MCGLShader*), const char* vname, const char* fname,
    const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);

    fundef(activateShaderProgram, void));
    fundef(getUniformLocation, int), const char* name);
    fundef(updateUniform, void), const char* name, MCGLUniformData udata);
    fundef(setUniforms, void));

    fundef(prepareShader, GLuint), GLuint Id, const char* vcode, const char* fcode, const char* version);
    fundef(shaderSetInt, void), const char* name, int value);
    fundef(shaderSetBool, void), const char* name, bool value);
    //pass bundlename = null to get main bundle
    fundef(prepareShaderName, int), GLuint Id, const char* bundlename, const char* vname, const char* fname, const char* version);
    fundef(tryUseShaderProgram, void));

    //for debug
    fundef(getUniformVector, int), const char* name, GLfloat* params);
    fundef(printUniforms, void));
};

constructor(MCGLShader));

alias(MCGLShader);

#endif /* MCGLShader_h */
