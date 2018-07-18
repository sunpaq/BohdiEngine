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

#define MAX_UNIFORM_NUM   100

class(MCGLShader, MCObject,
    GLuint pid;

    MCGLUniform uniforms[MAX_UNIFORM_NUM];
    MCBool uniformsDirty[MAX_UNIFORM_NUM];
    MCUInt uniformCount;
);

method(MCGLShader, void, bye, voida);
//shader
//please cache the location index when you first call the setters
//then directly pass the location index and pass name null
method(MCGLShader, MCGLShader*, initWithShaderCode, const char* vcode, const char* fcode,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
method(MCGLShader, MCGLShader*, initWithShaderName, const char* vname, const char* fname,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);

method(MCGLShader, void, activateShaderProgram, voida);
method(MCGLShader, int,  getUniformLocation, const char* name);
method(MCGLShader, void, updateUniform, const char* name, MCGLUniformData udata);
method(MCGLShader, void, setUniforms, voida);

method(MCGLShader, GLuint, prepareShader, GLuint Id, const char* vcode, const char* fcode, const char* version);
method(MCGLShader, void, shaderSetUInt, const char* name, MCUInt value);
method(MCGLShader, void, shaderSetBool, const char* name, MCBool value);
//pass bundlename = null to get main bundle
method(MCGLShader, int, prepareShaderName, GLuint Id, const char* bundlename, const char* vname, const char* fname, const char* version);
method(MCGLShader, void, tryUseShaderProgram, voida);

//for debug
method(MCGLShader, int, getUniformVector, const char* name, GLfloat* params);
method(MCGLShader, void, printUniforms, voida);

#endif /* MCGLShader_h */
