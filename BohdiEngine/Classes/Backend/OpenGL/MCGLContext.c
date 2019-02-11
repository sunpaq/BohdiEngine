//
//  MCGLContext.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCGLContext.h"
#include "BE2DTextureData.h"

fun(release, void)) as(MCGLContext)
    Release(it->shader);
    it->shader = null;
    cast(it, MCObject)->release(it);
}

fun(loadTexture, GLuint), struct MCTexture* tex, const char* samplerName, GLint unit) as(MCGLContext)
    if (tex) {
        if (tex->loadedToGL == false) {
            tex->loadedToGL = true;
            MCGLContext_generateTextureId(&tex->Id);
            MCGLContext_activeTextureUnit(unit);
            MCGLContext_bind2DTexture(tex->Id);
            MCGLContext_rawdataToTexbuffer(tex, GL_TEXTURE_2D);
            MCGLContext_setupTexParameter(tex, GL_TEXTURE_2D);
        } else {
            MCGLContext_activeTextureUnit(unit);
            MCGLContext_bind2DTexture(tex->Id);
        }
        return tex->Id;
    }
    it->shader->shaderSetInt(it->shader, samplerName, unit);
    return 0;
}

fun(loadMaterial, void), struct MCMaterial* mtl) as(MCGLContext)
    //set up once part
    if (mtl->dataChanged == true) {
        it->shader->activateShaderProgram(it->shader);
        
        MCGLUniform f;
        f.type = MCGLUniformVec3;
        f.data.vec3 = mtl->ambientLightColor;
        it->shader->updateUniform(it->shader, material_ambient, f.data);
        
        f.data.vec3 = mtl->diffuseLightColor;
        f.type = MCGLUniformVec3;
        it->shader->updateUniform(it->shader, material_diffuse, f.data);
        
        f.type = MCGLUniformVec3;
        f.data.vec3 = mtl->specularLightColor;
        it->shader->updateUniform(it->shader, material_specular, f.data);
        
        f.data.vec1 = mtl->specularLightPower;
        f.type = MCGLUniformVec1;
        it->shader->updateUniform(it->shader, material_shininess, f.data);
        
        f.data.vec1 = mtl->dissolve;
        f.type = MCGLUniformVec1;
        it->shader->updateUniform(it->shader, material_dissolve, f.data);
        
        mtl->dataChanged = false;
    }
    //set each time
    it->shader->shaderSetInt(it->shader, "illum", mtl->illum);
}

//Global
bool MCGLContext_isFeatureOn(MCGLFeature feature)
{
    return (bool)glIsEnabled(feature);
}

void MCGLContext_featureSwith(MCGLFeature feature, bool onOrOff)
{
    bool isOn = (bool)glIsEnabled(feature);
    if (onOrOff) {
        if (!isOn) glEnable(feature);
    }else{
        if (isOn) glDisable(feature);
    }
}

void MCGLContext_flushCommandAsync()
{
    glFlush();
}

void MCGLContext_flushCommandBlock()
{
    glFinish();
}

void MCGLContext_clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MCGLContext_clearScreenWithColor(MCColorf color)
{
    glClearColor(color.R, color.G, color.B, color.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MCGLContext_clearDepthBuffer()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void MCGLContext_clearStencilBuffer()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

void MCGLContext_setClearScreenColor(MCColorf color)
{
    glClearColor(color.R, color.G, color.B, color.A);
}

void MCGLContext_setPointSize(double pointsize)
{
    //glPointSize is replaced by the gl_PointSize variable in the vertex shader.
    //glPointSize((GLfloat)pointsize);
}

void MCGLContext_setLineWidth(double linewidth)
{
    glLineWidth((GLfloat)linewidth);
}

void MCGLContext_setFrontCounterClockWise(bool isCCW)
{
    if (isCCW) {
        glFrontFace(GL_CCW);
    }else{
        glFrontFace(GL_CW);
    }
}

void MCGLContext_cullFace(MCGLFace face)
{
    glCullFace(face);
}

void MCGLContext_cullBackFace()
{
    MCGLContext_cullFace(MCGLBack);
}

//Texture
static unsigned texUnitNum = 1;
unsigned MCGLContext_getIdleTextureUnit()
{
    if (texUnitNum < MCGLContext_getMaxTextureUnits()) {
        texUnitNum++;
    } else {
        texUnitNum = 1;
    }
    return texUnitNum;
}

unsigned MCGLContext_getMaxTextureUnits()
{
    return (unsigned)GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
}

void MCGLContext_generateTextureId(GLuint* tid)
{
    glGenTextures(1, tid);
}

void MCGLContext_activeTextureUnit(GLuint index)
{
    glActiveTexture(GL_TEXTURE0 + index);
}

void MCGLContext_bindCubeTexture(GLint tid)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
}

void MCGLContext_bind2DTexture(GLuint tid)
{
    glBindTexture(GL_TEXTURE_2D, tid);
}

void MCGLContext_unbind2DTextures()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MCGLContext_rawdataToTexbuffer(struct MCTexture* tex, GLenum textype)
{
    if (tex->data && tex->data->raw) {
        if (tex->data->channels == 4) {
            glTexImage2D(textype, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data->raw);
            glGenerateMipmap(textype);
        }
        else {
            glTexImage2D(textype, 0, GL_RGB, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data->raw);
            glGenerateMipmap(textype);
        }
    }
}

//GL_TEXTURE_2D
void MCGLContext_setupTexParameter(struct MCTexture* tex, GLenum textype)
{
    if (tex->displayMode == MCTextureRepeat) {
        glTexParameteri(textype, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(textype, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else {
        glTexParameteri(textype, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(textype, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glTexParameteri(textype, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textype, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void MCGLContext_enableTransparency(bool enable)
{
    if (enable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        glDisable(GL_BLEND);
    }
}

void MCGLContext_enablePolygonOffset(bool enable)
{
    if (enable) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0, -1.0);
    }
    else {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
}

//Frame Rate (FPS)
int MCGLContext_tickFPS(struct MCClock* clock)
{
    static unsigned fcount = 0;
    static clock_t elapse = 0;
    static clock_t time, lastime;
    
    clock->getCPUClocksSinceStart(clock, &time);
    if (elapse >= CLOCKS_PER_SEC ) {
        unsigned result = fcount;
        //reset
        elapse = 0;
        fcount = 0;
        lastime = time;
        
        return result;
    }else{
        elapse += (time - lastime);
        fcount++;
        return -1;
    }
}

//Shader
bool MCGLContext_compileShader(GLuint* shader, GLenum type, const GLchar *source, const GLchar *version)
{
    if (!source) {
        return false;
    }
    GLint status = 0;
    
    const char* sources[] = {version, source};
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 2, sources, NULL);
    glCompileShader(*shader);
    
    GLint logLength = 0;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        printf("Shader compile log:\n%s", log);
        free(log);
        
        //dump source
        //error_log(source);
    }
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}

int MCGLContext_linkProgram(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);
    
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program link log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return 0;
    }
    
    return 1;
}

int MCGLContext_validateProgram(GLuint prog)
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return 0;
    }
    
    return 1;
}

void MCGLContext_setViewport(int x, int y, int width, int height)
{
    glEnable(GL_DEPTH_TEST);//this is for Google cardboard
    glViewport(x, y, width, height);
}

void MCGLContext_setScissor(int x, int y, int width, int height)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, width, height);
}

constructor(MCGLContext)) {
    MCObject(any);
    as(MCGLContext)
        it->shader = new(MCGLShader);
    }
    dynamic(MCGLContext)
        funbind(release);
        funbind(loadTexture);
        funbind(loadMaterial);
    }
    return any;
}



