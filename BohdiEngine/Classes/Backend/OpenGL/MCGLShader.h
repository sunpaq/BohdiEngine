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
fun(MCGLShader, void, shaderSetUInt, const char* name, MCUInt value);
fun(MCGLShader, void, shaderSetBool, const char* name, MCBool value);
//pass bundlename = null to get main bundle
fun(MCGLShader, int, prepareShaderName, GLuint Id, const char* bundlename, const char* vname, const char* fname, const char* version);
fun(MCGLShader, void, tryUseShaderProgram, voida);

//for debug
fun(MCGLShader, int, getUniformVector, const char* name, GLfloat* params);
fun(MCGLShader, void, printUniforms, voida);

#endif /* MCGLShader_h */
