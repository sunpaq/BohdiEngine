//
//  MCGLContext.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCGLContext.h"
#include "MC3DBase.h"
#include "MCGLContext.h"
#include "BEAssetsManager.h"
#include "MCGLRenderer.h"

oninit(MCGLContext)
{
    if (init(MCObject)) {
        var(shader) = new(MCGLShader);
        var(drawMode) = MCDrawNone;
        return obj;
    }else{
        return null;
    }
}

method(MCGLContext, void, bye, voida)
{
    release(obj->shader);
    obj->shader = null;
}

method(MCGLContext, void, loadTexture, MCTexture* tex, const char* samplerName)
{
    if (tex) {
        if (tex->loadedToGL == false) {
            tex->loadedToGL = true;
            MCGLContext_generateTextureId(&tex->Id);
            MCGLContext_activeTextureUnit(tex->textureUnit);
            MCGLContext_bind2DTexture(tex->Id);
            MCGLContext_rawdataToTexbuffer(tex, GL_TEXTURE_2D);
            MCGLContext_setupTexParameter(tex, GL_TEXTURE_2D);
        }
        MCGLShader_shaderSetUInt(obj->shader, samplerName, tex->textureUnit);
        MCGLContext_activeTextureUnit(tex->textureUnit);
        MCGLContext_bind2DTexture(tex->Id);
    }
}

method(MCGLContext, void, loadMaterial, MCMaterial* mtl)
{
    //set up once part
    if (mtl->dataChanged == true) {
        MCGLShader_activateShaderProgram(obj->shader, 0);
        
        MCGLUniform f;
        f.type = MCGLUniformVec3;
        f.data.vec3 = mtl->ambientLightColor;
        MCGLShader_updateUniform(obj->shader, material_ambient, f.data);
        
        f.data.vec3 = mtl->diffuseLightColor;
        f.type = MCGLUniformVec3;
        MCGLShader_updateUniform(obj->shader, material_diffuse, f.data);
        
        f.type = MCGLUniformVec3;
        f.data.vec3 = mtl->specularLightColor;
        MCGLShader_updateUniform(obj->shader, material_specular, f.data);
        
        f.data.vec1 = mtl->specularLightPower;
        f.type = MCGLUniformVec1;
        MCGLShader_updateUniform(obj->shader, material_shininess, f.data);
        
        f.data.vec1 = mtl->dissolve;
        f.type = MCGLUniformVec1;
        MCGLShader_updateUniform(obj->shader, material_dissolve, f.data);
        
        mtl->dataChanged = false;
    }
    //set each time
    MCGLShader_shaderSetUInt(obj->shader, "illum", mtl->illum);
}

method(MCGLContext, void, loadMesh, MCMesh* meth)
{
    if (meth->isDataLoaded == false) {
        glGenVertexArrays(1, &meth->VAO);
        glGenBuffers(1, &meth->VBO);
        //VAO
        glBindVertexArray(meth->VAO);
        //VBO
        glBindBuffer(GL_ARRAY_BUFFER, meth->VBO);
        glBufferData(GL_ARRAY_BUFFER, meth->vertexDataSize, meth->vertexDataPtr, meth->useage);
        //EBO
        if (meth->vertexIndexes != null) {
            glGenBuffers(1, &meth->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meth->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*meth->vertexCount, meth->vertexIndexes, meth->useage);
        }
        //VAttributes
        int i;
        for (i=0; i<MCVertexAttribIndexMax-1; i++) {
            MCVertexAttribute attr = meth->vertexAttribArray[i];
            if (attr.vectorsize != (GLint)null) {
                MCVertexAttributeLoad(&meth->vertexAttribArray[i]);
            }
        }
        //Unbind
        glBindVertexArray(0);
        meth->isDataLoaded = true;
    }
}

method(MCGLContext, void, drawMesh, MCMesh* meth)
{
    glBindVertexArray(meth->VAO);
    //override draw mode
    GLenum mode = meth->mode;
    if (obj->drawMode != MCDrawNone) {
        mode = obj->drawMode;
    }
    //draw
    if (mode != MCDrawNone) {
        if (meth->vertexIndexes != null) {
            glDrawElements(mode, 100, GL_UNSIGNED_INT, (GLvoid*)0);
        }else{
            glDrawArrays(mode, 0, meth->vertexCount);
        }
    }
    //Unbind
    glBindVertexArray(0);
    MCGLContext_unbind2DTextures(0);
}

onload(MCGLContext)
{
    if (load(MCObject)) {
        binding(MCGLContext, void, bye, voida);
        binding(MCGLContext, void, loadTexture, MCTexture* tex, const char* samplerName);
        binding(MCGLContext, void, loadMaterial, MCMaterial* mtl);
        binding(MCGLContext, void, loadMesh, MCMesh* meth);
        binding(MCGLContext, void, drawMesh, MCMesh* ctx);
        return cla;
    }else{
        return null;
    }
}

//Global
utility(MCGLContext, MCBool, isFeatureOn, MCGLFeature feature)
{
    return (MCBool)glIsEnabled(feature);
}

utility(MCGLContext, void, featureSwith, MCGLFeature feature, MCBool onOrOff)
{
    MCBool isOn = (MCBool)glIsEnabled(feature);
    if (onOrOff) {
        if (!isOn) glEnable(feature);
    }else{
        if (isOn) glDisable(feature);
    }
}

utility(MCGLContext, void, flushCommandAsync, voida)
{
    glFlush();
}

utility(MCGLContext, void, flushCommandBlock, voida)
{
    glFinish();
}

utility(MCGLContext, void, clearScreen, voida)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

utility(MCGLContext, void, clearScreenWithColor, MCColorf color)
{
    glClearColor(color.R.f, color.G.f, color.B.f, color.A.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

utility(MCGLContext, void, clearDepthBuffer, voida)
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

utility(MCGLContext, void, clearStencilBuffer, voida)
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

utility(MCGLContext, void, setClearScreenColor, MCColorf color)
{
    glClearColor(color.R.f, color.G.f, color.B.f, color.A.f);
}

utility(MCGLContext, void, setPointSize, double pointsize)
{
    //glPointSize is replaced by the gl_PointSize variable in the vertex shader.
    //glPointSize((GLfloat)pointsize);
}

utility(MCGLContext, void, setLineWidth, double linewidth)
{
    glLineWidth((GLfloat)linewidth);
}

utility(MCGLContext, void, setFrontCounterClockWise, MCBool isCCW)
{
    if (isCCW) {
        glFrontFace(GL_CCW);
    }else{
        glFrontFace(GL_CW);
    }
}

utility(MCGLContext, void, cullFace, MCGLFace face)
{
    glCullFace(face);
}

utility(MCGLContext, void, cullBackFace, voida)
{
    MCGLContext_cullFace(MCGLBack);
}

//Texture
static MCUInt texUnitNum = 1;
utility(MCGLContext, MCUInt, getIdleTextureUnit, voida)
{
    if (texUnitNum < MCGLContext_getMaxTextureUnits(0)) {
        texUnitNum++;
    } else {
        texUnitNum = 1;
    }
    return texUnitNum;
}

utility(MCGLContext, MCUInt, getMaxTextureUnits, voida)
{
    return (MCUInt)GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
}

utility(MCGLContext, void, generateTextureId, MCUInt* tid)
{
    glGenTextures(1, tid);
}

utility(MCGLContext, void, activeTextureUnit, MCUInt index)
{
    glActiveTexture(GL_TEXTURE0 + index);
}

utility(MCGLContext, void, bindCubeTexture, MCUInt tid)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
}

utility(MCGLContext, void, bind2DTexture, MCUInt tid)
{
    glBindTexture(GL_TEXTURE_2D, tid);
}

utility(MCGLContext, void, unbind2DTextures, voida)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

utility(MCGLContext, void, rawdataToTexbuffer, MCTexture* tex, GLenum textype)
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
utility(MCGLContext, void, setupTexParameter, MCTexture* tex, GLenum textype)
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

utility(MCGLContext, void, enableTransparency, MCBool enable)
{
    if (enable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        glDisable(GL_BLEND);
    }
}

utility(MCGLContext, void, enablePolygonOffset, MCBool enable)
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
utility(MCGLContext, int, tickFPS, MCClock* clock)
{
    static unsigned fcount = 0;
    static clock_t elapse = 0;
    static clock_t time, lastime;
    
    MCClock_getCPUClocksSinceStart(clock, &time);
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
utility(MCGLContext, MCBool, compileShader, GLuint* shader, GLenum type, const GLchar *source, const GLchar *version)
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

utility(MCGLContext, int, linkProgram, GLuint prog)
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

utility(MCGLContext, int, validateProgram, GLuint prog)
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

utility(MCGLContext, void, setViewport, int x, int y, int width, int height)
{
    glEnable(GL_DEPTH_TEST);//this is for Google cardboard
    glViewport(x, y, width, height);
}

utility(MCGLContext, void, setScissor, int x, int y, int width, int height)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, width, height);
}
