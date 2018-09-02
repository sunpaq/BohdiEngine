//
//  MCGLShader.c
//  BohdiEngine
//
//  Created by 孙御礼 on 4/27/30 H.
//

#include "MCGLShader.h"
#include "MCGLContext.h"

oninit(MCGLShader)
{
    if (init(MCObject)) {
        var(pid) = glCreateProgram();
        var(uniformCount) = 0;
        return obj;
    } else {
        return null;
    }
}

ifun(int, fillUniformLocation, MCGLUniform* uniform)
{
    as(MCGLShader);
    if (uniform->location == MC3DErrUniformNotFound) {
        uniform->location = glGetUniformLocation(var(pid), uniform->name);
    }
    return uniform->location;
}

fun(MCGLShader, void, bye, voida)
{
    glDeleteProgram(var(pid));
}

//shader
//please cache the location index when you first call the setters
//then directly pass the location index and pass name null
fun(MCGLShader, MCGLShader*, initWithShaderCode, const char* vcode, const char* fcode,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount)
{
    MCGLShader_tryUseShaderProgram(obj, 0);
    
    //attribute
    for (int i=0; i<acount; i++) {
        glBindAttribLocation(obj->pid, i, attribs[i]);
    }
    
#if TARGET_OS_OSX
    MCGLShader_prepareShader(obj, obj->pid, vcode, fcode, "#version 330 core\n");
#else
    MCGLShader_prepareShader(obj, obj->pid, vcode, fcode, "#version 300 es\n");
#endif
    
    //uniforms
    for (int i=0; i<ucount; i++) {
        MCGLUniform* f = &obj->uniforms[obj->uniformCount++];
        MCGLUniformSetName(f, uniforms[i]);
        f->type = types[i];
        f->location = glGetUniformLocation(var(pid), uniforms[i]);
        obj->uniformsDirty[i] = false;
    }
    
    return obj;
}

fun(MCGLShader, MCGLShader*, initWithShaderName, const char* vname, const char* fname,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount)
{
    char vpath[LINE_MAX] = {0};
    if(MCFileGetPath(vname, vpath))
    return null;
    const char* vcode = MCFileCopyContentWithPath(vpath);
    
    char fpath[LINE_MAX] = {0};
    if(MCFileGetPath(fname, fpath))
    return null;
    const char* fcode = MCFileCopyContentWithPath(fpath);
    
    MCGLShader_initWithShaderCode(obj, vcode, fcode, attribs, acount, types, uniforms, ucount);
    
    free((void*)vcode);
    free((void*)fcode);
    return obj;
}

fun(MCGLShader, void, activateShaderProgram, voida)
{
    MCGLShader_tryUseShaderProgram(obj, 0);
}

fun(MCGLShader, int,  getUniformLocation, const char* name)
{
    for (MCUInt i=0; i<obj->uniformCount; i++) {
        if (strcmp(name, obj->uniforms[i].name)==0) {
            return fillUniformLocation(obj, &obj->uniforms[i]);
        }
    }
    return MC3DErrUniformNotFound;
}

//MCGLUniformScalar,
//MCGLUniformVec2,
//MCGLUniformVec3,
//MCGLUniformVec4,
//MCGLUniformMat3,
//MCGLUniformMat4
ifun(int, setUniform, const char* name, int loc, MCGLUniform* uniform)
{
    as(MCGLShader);
    if (var(pid) == 0) {
        return -1;
    }
    if (name != null) {
        loc = glGetUniformLocation(var(pid), name);
    }
    if (loc != MC3DErrUniformNotFound) {
        switch (uniform->type) {
            case MCGLUniformScalar:
            glUniform1i(loc, uniform->data.scalar);
            break;
            case MCGLUniformVec1:
            glUniform1f(loc, uniform->data.vec1);
            break;
            case MCGLUniformVec2:
            glUniform2f(loc, uniform->data.vec2.x, uniform->data.vec2.y);
            break;
            case MCGLUniformVec3:
            glUniform3f(loc, uniform->data.vec3.x, uniform->data.vec3.y,
                        uniform->data.vec3.z);
            break;
            case MCGLUniformVec4:
            glUniform4f(loc, uniform->data.vec4.x, uniform->data.vec4.y,
                        uniform->data.vec4.z, uniform->data.vec4.w);
            break;
            case MCGLUniformMat3:
            glUniformMatrix3fv(loc, 1, 0, uniform->data.mat3.m);
            break;
            case MCGLUniformMat4:
            glUniformMatrix4fv(loc, 1, 0, uniform->data.mat4.m);
            break;
            default:
            break;
        }
    }
    return loc;
}

fun(MCGLShader, void, updateUniform, const char* name, MCGLUniformData udata)
{
    MCGLUniform* u = null;
    int f = -1;
    for (int i=0; i<var(uniformCount); i++) {
        u = &var(uniforms)[i];
        if (strcmp(name, u->name) == 0) {
            f = i;
            break;
        }
    }
    
    if (u != null && f != -1 && !MCGLUniformDataEqual(u->type, &u->data, &udata)) {
        var(uniformsDirty)[f] = true;
        var(uniforms)[f].data = udata;
    }
    else if (f != -1){
        var(uniformsDirty)[f] = false;
    }
}

fun(MCGLShader, void, setUniforms, voida)
{
    for (int i=0; i<var(uniformCount); i++) {
        if (var(uniformsDirty)[i] == true) {
            MCGLUniform* f = &var(uniforms)[i];
            setUniform(obj, null, f->location, f);
            var(uniformsDirty)[i] = false;
        }
    }
}

fun(MCGLShader, GLuint, prepareShader, GLuint Id, const char* vcode, const char* fcode, const char* version)
{
    GLuint vertShader=0, fragShader=0;
    MCGLContext_compileShader(&vertShader, GL_VERTEX_SHADER, vcode, version);
    glAttachShader(Id, vertShader);
    
    MCGLContext_compileShader(&fragShader, GL_FRAGMENT_SHADER, fcode, version);
    glAttachShader(Id, fragShader);
    
    // Create shader program.
    //Id = glCreateProgram();
    
    // Attach vertex shader to program.
    
    // Attach fragment shader to program.
    
    //    if (context != null) {
    //        MCGLContext_beforeLinkProgram(0, context, Id);
    //    }
    
    // Link program.
    if (MCGLContext_linkProgram(Id) == 0) {
        error_log("Failed to link program: %d", Id);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (Id) {
            glDeleteProgram(Id);
            Id = 0;
        }
    }
    
    //    if (context != null) {
    //        MCGLContext_afterLinkProgram(0, context, Id);
    //    }
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(Id, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(Id, fragShader);
        glDeleteShader(fragShader);
    }
    
    return Id;
}

fun(MCGLShader, void, shaderSetInt, const char* name, MCInt value)
{
    glUniform1i(glGetUniformLocation(obj->pid, name), value);
}

fun(MCGLShader, void, shaderSetBool, const char* name, MCBool value)
{
    glUniform1i(glGetUniformLocation(obj->pid, name), value);
}

//pass bundlename = null to get main bundle
fun(MCGLShader, int, prepareShaderName, GLuint Id, const char* bundlename, const char* vname, const char* fname, const char* version)
{
    char vpath[PATH_MAX] = {0};
    if(MCFileGetPathFromBundle(bundlename, vname, vpath)) return -1;
    char* vcode = (char*)MCFileCopyContentWithPath(vpath);
    
    char fpath[PATH_MAX] = {0};
    if(MCFileGetPathFromBundle(bundlename, fname, fpath)) return -1;
    char* fcode = (char*)MCFileCopyContentWithPath(fpath);
    
    MCGLShader_prepareShader(0, Id, vcode, fcode, version);
    if (vcode) {
        free((void*)vcode);
    }
    if (fcode) {
        free((void*)fcode);
    }
    return 0;
}

fun(MCGLShader, void, tryUseShaderProgram, voida)
{
    GLuint Id = obj->pid;
    GLint cid = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cid);
    if (Id != 0 && Id != (GLuint)cid && glIsProgram(Id) == GL_TRUE) {
        glUseProgram(Id);
    }
}

fun(MCGLShader, int, getUniformVector, const char* name, GLfloat* params)
{
    int loc = (int)ff(obj, getUniformLocation, name);
    glGetUniformfv(var(pid), loc, params);
    return loc;
}

fun(MCGLShader, void, printUniforms, voida)
{
    MCLogTypeSet(MC_DEBUG);

    GLfloat buff[16];
    for (int i=0; i<var(uniformCount); i++) {
        MCGLUniform* f = &var(uniforms)[i];
        if (f) {
            ff(obj, getUniformVector, f->name, buff);
            if (f->type == MCGLUniformMat4) {
                //mat4
                debug_log("mat4:%s\n [%f/%f/%f/%f]\n [%f/%f/%f/%f]\n [%f/%f/%f/%f]\n [%f/%f/%f/%f]\n",
                          f->name,
                          buff[0],  buff[1],  buff[2],  buff[3],
                          buff[4],  buff[5],  buff[6],  buff[7],
                          buff[8],  buff[9],  buff[10], buff[11],
                          buff[12], buff[13], buff[14], buff[15]);
            }
            if (f->type == MCGLUniformMat3) {
                //mat3
                debug_log("mat3:%s\n [%f/%f/%f]\n [%f/%f/%f]\n [%f/%f/%f]\n",
                          f->name,
                          buff[0], buff[1], buff[2],
                          buff[3], buff[4], buff[5],
                          buff[6], buff[7], buff[8]);
            }
            if (f->type == MCGLUniformVec4) {
                //vec4
                debug_log("vec4:%s [%f/%f/%f/%f]\n", f->name, buff[0], buff[1], buff[2], buff[3]);
            }
            if (f->type == MCGLUniformVec3) {
                //vec3
                debug_log("vec3:%s [%f/%f/%f]\n", f->name, buff[0], buff[1], buff[2]);
            }
            if (f->type == MCGLUniformVec2) {
                //vec2
                debug_log("vec2:%s [%f/%f/%f]\n", f->name, buff[0], buff[1]);
            }
            if (f->type == MCGLUniformVec1) {
                //vec1
                debug_log("vec1:%s [%f]\n", f->name, buff[0]);
            }
            if (f->type == MCGLUniformScalar) {
                //scalar
                int value;
                ff(obj, getUniformVector, f->name, &value);
                debug_log("scalar:%s [%d]\n", f->name, value);
            }
        }
    }
}

onload(MCGLShader)
{
    if (load(MCObject)) {
        bid(MCGLShader, void, bye, voida);
        bid(MCGLShader, MCGLShader*, initWithShaderCode, const char* vcode, const char* fcode,
               const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
        bid(MCGLShader, MCGLShader*, initWithShaderName, const char* vname, const char* fname,
               const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
        bid(MCGLShader, void, activateShaderProgram, voida);
        bid(MCGLShader, int,  getUniformLocation, const char* name);
        bid(MCGLShader, void, updateUniform, const char* name, MCGLUniformData udata);
        bid(MCGLShader, void, setUniforms, voida);
        
        bid(MCGLShader, int, getUniformVector, const char* name, GLfloat* params);
        bid(MCGLShader, void, printUniforms, voida);
        return cla;
    } else {
        return null;
    }
}
