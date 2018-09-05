//
//  MCTexture.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCTexture.h"
#include "BEAssetsManager.h"

oninit(MCTexture)
{
    if (init(MCObject)) {
        var(Id) = 0;
        var(width) = 512;
        var(height)= 512;
        var(data) = null;
        var(displayMode) = MCTextureRepeat;
        var(loadedToGL) = false;
        return obj;
    }else{
        return null;
    }
}

ifun(unsigned char*, loadImageRawdata, const char* path)
{
    as(MCTexture);
    var(data) = BE2DTextureData_newWithPathname(path);
    if (var(data)) {
        obj->width  = obj->data->width;
        obj->height = obj->data->height;
        return obj->data->raw;
    }
    else {
        error_log("MCTexture - can not load image: %s\n", path);
        return null;
    }
}

ifun(void, freeRawdata, voida)
{
    as(MCTexture);
    if (obj->data) {
        release(obj->data);
        obj->data = null;
    }
}

fun(MCTexture, void, bye, voida)
{
    superbye(MCObject);
    freeRawdata(obj, 0);
}

fun(MCTexture, MCTexture*, initWithFileNameMode, const char* name, MCTextureDisplayMode mode)
{
    char pathbuff[PATH_MAX] = {0};
    if (MCFileGetPath(name, pathbuff)) {
        return null;
    }
    obj->displayMode = mode;
    loadImageRawdata(obj, pathbuff);
    return obj;
}

fun(MCTexture, MCTexture*, initWithFileName, const char* name)
{
    return MCTexture_initWithFileNameMode(obj, name, MCTextureRepeat);
}

fun(MCTexture, MCTexture*, initWith2DTexture, BE2DTextureData* tex)
{
    var(data) = tex;
    if (var(data)) {
        obj->width  = obj->data->width;
        obj->height = obj->data->height;
        return obj;
    }
    return null;
}

onload(MCTexture)
{
    if (load(MCObject)) {
        mix(unsigned char*, loadImageRawdata, const char* name);
        //mix(void, rawdataToTexbuffer, voida);
        //mix(void, setupTexParameter, GLenum textype);
        mix(void, freeRawdata, voida);
        
        bid(MCTexture, void, bye, voida);
        bid(MCTexture, MCTexture*, initWithFileNameMode, const char* name, MCTextureDisplayMode mode);
        bid(MCTexture, MCTexture*, initWithFileName, const char* name);
        bid(MCTexture, MCTexture*, initWith2DTexture, BE2DTextureData* tex);
        //bid(MCTexture, void, loadToGLBuffer, voida);
        //bid(MCTexture, void, active, GLuint pid, const char* uniformName);

        return cla;
    }else{
        return null;
    }
}
