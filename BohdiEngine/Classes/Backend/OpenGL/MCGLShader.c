//
//  MCGLShader.c
//  BohdiEngine
//
//  Created by 孙御礼 on 4/27/30 H.
//

#include "MCGLShader.h"
#include "MCGLContext.h"
#include "BEAssetsManager.h"
#include "MCLog.h"

fun(fillUniformLocation, int), MCGLUniform* uniform) as(MCGLShader)
    if (uniform->location == MC3DErrUniformNotFound) {
        uniform->location = glGetUniformLocation(it->pid, uniform->name);
    }
    return uniform->location;
}

fun(release, void)) as(MCGLShader)
    glDeleteProgram(it->pid);
}

//shader
//please cache the location index when you first call the setters
//then directly pass the location index and pass name null
fun(initWithShaderCode, struct MCGLShader*), const char* vcode, const char* fcode,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount) as(MCGLShader)
    it->tryUseShaderProgram(it);

    //attribute
    int i;
    for (i=0; i<acount; i++) {
        glBindAttribLocation(it->pid, i, attribs[i]);
    }
    
#if TARGET_OS_OSX
    it->prepareShader(it, it->pid, vcode, fcode, "#version 330 core\n");
#else
    it->prepareShader(it, it->pid, vcode, fcode, "#version 300 es\n");
#endif
    
    //uniforms
    for (i=0; i<ucount; i++) {
        MCGLUniform* f = &it->uniforms[it->uniformCount++];
        MCGLUniformSetName(f, uniforms[i]);
        f->type = types[i];
        f->location = glGetUniformLocation(it->pid, uniforms[i]);
        it->uniformsDirty[i] = false;
    }
    
    return it;
}

fun(initWithShaderName, struct MCGLShader*), const char* vname, const char* fname,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount) as(MCGLShader)
    char vpath[LINE_MAX] = {0};
    if(MCFileGetPath(vname, vpath))
    return null;
    const char* vcode = MCFileCopyContentWithPath(vpath);
    
    char fpath[LINE_MAX] = {0};
    if(MCFileGetPath(fname, fpath))
    return null;
    const char* fcode = MCFileCopyContentWithPath(fpath);
    
    it->initWithShaderCode(it, vcode, fcode, attribs, acount, types, uniforms, ucount);
    
    free((void*)vcode);
    free((void*)fcode);
    return it;
}

fun(activateShaderProgram, void)) as(MCGLShader)
    it->tryUseShaderProgram(it);
}

fun(getUniformLocation, int), const char* name) as(MCGLShader)
    for (unsigned i=0; i<it->uniformCount; i++) {
        if (strcmp(name, it->uniforms[i].name)==0) {
            return fillUniformLocation(it, &it->uniforms[i]);
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
fun(setUniform, int), const char* name, int loc, MCGLUniform* uniform) as(MCGLShader)
    if (it->pid == 0) {
        return -1;
    }
    if (name != null) {
        loc = glGetUniformLocation(it->pid, name);
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

fun(updateUniform, void), const char* name, MCGLUniformData udata) as(MCGLShader)
    MCGLUniform* u = null;
    int f = -1, i = 0;
    for (i=0; i<it->uniformCount; i++) {
        u = &it->uniforms[i];
        if (strcmp(name, u->name) == 0) {
            f = i;
            break;
        }
    }
    
    if (u != null && f != -1 && !MCGLUniformDataEqual(u->type, &u->data, &udata)) {
        it->uniformsDirty[f] = true;
        it->uniforms[f].data = udata;
    }
    else if (f != -1){
        it->uniformsDirty[f] = false;
    }
}

fun(setUniforms, void)) as(MCGLShader)
    for (int i=0; i<it->uniformCount; i++) {
        if (it->uniformsDirty[i] == true) {
            MCGLUniform* f = &it->uniforms[i];
            setUniform(it, null, f->location, f);
            it->uniformsDirty[i] = false;
        }
    }
}

fun(prepareShader, GLuint), GLuint Id, const char* vcode, const char* fcode, const char* version) {
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

fun(shaderSetInt, void), const char* name, int value) as(MCGLShader)
    glUniform1i(glGetUniformLocation(it->pid, name), value);
}

fun(shaderSetBool, void), const char* name, bool value) as(MCGLShader)
    glUniform1i(glGetUniformLocation(it->pid, name), value);
}

//pass bundlename = null to get main bundle
fun(prepareShaderName, int), GLuint Id, const char* bundlename, const char* vname, const char* fname, const char* version) as(MCGLShader)
    char vpath[PATH_MAX] = {0};
    if(MCFileGetPathFromBundle(bundlename, vname, vpath)) return -1;
    char* vcode = (char*)MCFileCopyContentWithPath(vpath);
    
    char fpath[PATH_MAX] = {0};
    if(MCFileGetPathFromBundle(bundlename, fname, fpath)) return -1;
    char* fcode = (char*)MCFileCopyContentWithPath(fpath);

    it->prepareShader(it, Id, vcode, fcode, version);
    if (vcode) {
        free((void*)vcode);
    }
    if (fcode) {
        free((void*)fcode);
    }
    return 0;
}

fun(tryUseShaderProgram, void)) as(MCGLShader)
    GLuint Id = it->pid;
    GLint cid = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cid);
    if (Id != 0 && Id != (GLuint)cid && glIsProgram(Id) == GL_TRUE) {
        glUseProgram(Id);
    }
}

fun(getUniformVector, int), const char* name, GLfloat* params) as(MCGLShader)
    int loc = (int)ff(it, getUniformLocation), name);
    glGetUniformfv(it->pid, loc, params);
    return loc;
}

fun(printUniforms, void)) as(MCGLShader)
    MCLogTypeSet(MC_DEBUG);

    GLfloat buff[16];
    for (int i=0; i<it->uniformCount; i++) {
        MCGLUniform* f = &it->uniforms[i];
        if (f) {
            ff(it, getUniformVector), f->name, buff);
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
                ff(it, getUniformVector), f->name, &value);
                debug_log("scalar:%s [%d]\n", f->name, value);
            }
        }
    }
}

constructor(MCGLShader)) {
    MCObject(any);
    as(MCGLShader)
        it->pid = glCreateProgram();
        it->uniformCount = 0;
    }
    dynamic(MCGLShader)
        funbind(release);
        funbind(initWithShaderCode);
        funbind(initWithShaderName);
        funbind(activateShaderProgram);
        funbind(getUniformLocation);
        funbind(updateUniform);
        funbind(setUniforms);
        funbind(prepareShader);
        funbind(shaderSetInt);
        funbind(shaderSetBool);
        funbind(prepareShaderName);
        funbind(tryUseShaderProgram);
        funbind(getUniformVector);
        funbind(printUniforms);
    }
    return any;
}



