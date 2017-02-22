//
//  MCTexture.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCTexture.h"
#include "MCGLEngine.h"

static unsigned unitNum = 1;

oninit(MCTexture)
{
    if (init(MCObject)) {
        if (unitNum < 32) {
            obj->textureUnit = unitNum++;
        } else {
            unitNum = 1;
        }
        return obj;
    }else{
        return null;
    }
}

function(unsigned char*, loadImageRawdata, const char* path)
{
    as(MCTexture);
    var(data) = BE2DTextureData_newWithPath(path);
    obj->width  = obj->data->width;
    obj->height = obj->data->height;
    return obj->data->raw;
}

function(void, rawdataToTexbuffer, GLenum textype)
{
    as(MCTexture);
    if (obj->data->raw) {
        if (obj->data->channels == 4) {
            glTexImage2D(textype, 0, GL_RGBA, obj->width, obj->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, obj->data->raw);
            glGenerateMipmap(textype);
        }
        else if (obj->data->channels == 3) {
            glTexImage2D(textype, 0, GL_RGB, obj->width, obj->height, 0, GL_RGB, GL_UNSIGNED_BYTE, obj->data->raw);
            glGenerateMipmap(textype);
        }
    }
}

//GL_TEXTURE_2D
function(void, setupTexParameter, GLenum textype)
{
    //glTexParameteri(textype, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(textype, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(textype, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(textype, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(textype, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(textype, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

function(void, freeRawdata, voida)
{
    as(MCTexture);
    release(obj->data);
}

method(MCTexture, MCTexture*, initWithFileName, const char* name)
{
    char pathbuff[PATH_MAX] = {};
    if (MCFileGetPath(name, pathbuff)) {
        return null;
    }
    
    loadImageRawdata(0, obj, pathbuff);
    return obj;
}

method(MCTexture, void, loadToGLBuffer, voida)
{
    glGenTextures(1, &obj->Id);
    MCGLEngine_activeTextureUnit(obj->textureUnit);
    MCGLEngine_bind2DTexture(obj->Id);
    
    rawdataToTexbuffer(0, obj, GL_TEXTURE_2D);
    setupTexParameter(0, obj, GL_TEXTURE_2D);
    //freeRawdata(0, obj, 0);
}

method(MCTexture, void, active, GLuint pid, const char* uniformName)
{
    if (obj) {
        glUniform1i(glGetUniformLocation(pid, uniformName), obj->textureUnit);
        MCGLEngine_activeTextureUnit(obj->textureUnit);
        MCGLEngine_bind2DTexture(obj->Id);
    }
}

onload(MCTexture)
{
    if (load(MCObject)) {
        mixing(unsigned char*, loadImageRawdata, const char* name);
        mixing(void, rawdataToTexbuffer, voida);
        mixing(void, setupTexParameter, GLenum textype);
        mixing(void, freeRawdata, voida);
        
        binding(MCTexture, MCTexture*, initWithFileName, const char* name);
        binding(MCTexture, void, loadToGLBuffer, voida);
        binding(MCTexture, void, active, GLuint pid, const char* uniformName);

        return cla;
    }else{
        return null;
    }
}
