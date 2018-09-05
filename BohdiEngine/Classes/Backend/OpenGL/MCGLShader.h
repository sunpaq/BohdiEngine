//
//  MCGLShader.h
//  BohdiEngine
//
//  Created by 孙御礼 on 4/27/30 H.
//

#ifndef MCGLShader_h
#define MCGLShader_h

#include "monkc_export.h"

#include "MCGLBase.h"
#include "MCGLDefaultShader.h"

#define MAX_UNIFORM_NUM   100

MCGlobalKey view_view       = "view_view";
MCGlobalKey view_projection = "view_projection";
MCGlobalKey view_position   = "view_position";

MCGlobalKey model_model     = "model_model";
MCGlobalKey model_normal    = "model_normal";

MCGlobalKey light_ambient   = "light_ambient";
MCGlobalKey light_diffuse   = "light_diffuse";
MCGlobalKey light_specular  = "light_specular";
MCGlobalKey light_color     = "light_color";
MCGlobalKey light_position  = "light_position";

MCGlobalKey material_ambient   = "material_ambient";
MCGlobalKey material_diffuse   = "material_diffuse";
MCGlobalKey material_specular  = "material_specular";
MCGlobalKey material_dissolve  = "material_dissolve";
MCGlobalKey material_shininess = "material_shininess";

class(MCGLShader, MCObject,
    GLuint pid;

    MCGLUniform uniforms[MAX_UNIFORM_NUM];
    MCBool uniformsDirty[MAX_UNIFORM_NUM];
    MCUInt uniformCount;
);

fun(MCGLShader, void, bye, voida);
//shader
//please cache the location index when you first call the setters
//then directly pass the location index and pass name null
fun(MCGLShader, MCGLShader*, initWithShaderCode, const char* vcode, const char* fcode,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
fun(MCGLShader, MCGLShader*, initWithShaderName, const char* vname, const char* fname,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);

fun(MCGLShader, void, activateShaderProgram, voida);
fun(MCGLShader, int,  getUniformLocation, const char* name);
fun(MCGLShader, void, updateUniform, const char* name, MCGLUniformData udata);
fun(MCGLShader, void, setUniforms, voida);

fun(MCGLShader, GLuint, prepareShader, GLuint Id, const char* vcode, const char* fcode, const char* version);
fun(MCGLShader, void, shaderSetInt, const char* name, MCInt value);
fun(MCGLShader, void, shaderSetBool, const char* name, MCBool value);
//pass bundlename = null to get main bundle
fun(MCGLShader, int, prepareShaderName, GLuint Id, const char* bundlename, const char* vname, const char* fname, const char* version);
fun(MCGLShader, void, tryUseShaderProgram, voida);

//for debug
fun(MCGLShader, int, getUniformVector, const char* name, GLfloat* params);
fun(MCGLShader, void, printUniforms, voida);

#endif /* MCGLShader_h */
