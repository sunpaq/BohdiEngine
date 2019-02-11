//
//  BE2DTextureData.c
//  Sapindus
//
//  Created by YuliSun on 24/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#include "BE2DTextureData.h"
#include "BEAssetsManager.h"
#include "MCString.h"
#include "SOIL.h"
#include "MCLog.h"

struct BE2DTextureData* BE2DTextureData_newWithPathnameType(const char* path, unsigned type)
{
    struct BE2DTextureData* data = new(BE2DTextureData);
    
    char decodepath[PATH_MAX] = {0};
    MCString_percentDecode(path, decodepath);
    
    size_t psize = strlen(decodepath) * sizeof(char);
    data->path = strcpy(malloc(psize), decodepath);

    off_t buffsize;
    const char* buff = MCFileCopyContentWithPathGetBufferSize(path, &buffsize);
    data->raw = SOIL_load_image_from_memory((const unsigned char*)buff, (int)buffsize,
                                            &data->width, &data->height, &data->channels,
                                            SOIL_LOAD_AUTO);
    MCFileReleaseContent((void*)buff);

    if (!data->raw) {
        error_log("BE2DTextureData - load texture failed: %s (%s)\n", SOIL_last_result(), path);
        Release(data);
        return null;
    }
    return data;
}

struct BE2DTextureData* BE2DTextureData_newWithPathname(const char* path)
{
    return BE2DTextureData_newWithPathnameType(path, 3);//default RGB
}

struct BE2DTextureData* BE2DTextureData_newWithFilename(const char* file)
{
    char path[PATH_MAX] = {0};
    if(MCFileGetPath(file, path)) {
        return null;
    }
    if (path[0]) {
        return BE2DTextureData_newWithPathname(path);
    }
    return null;
}

fun(release, void)) as(BE2DTextureData)
    if (it->path) {
        free(it->path);
        it->path = null;
    }
    if (it->raw) {
        SOIL_free_image_data(it->raw);
        it->raw = null;
    }
    cast(it, MCObject)->release(it);
}

constructor(BE2DTextureData)) {
    MCObject(any);
    as(BE2DTextureData)
        //const types
        it->AUTO = 0;
        it->L    = 1;
        it->LA   = 2;
        it->RGB  = 3;
        it->RGBA = 4;
    
        //input
        //it->type = it->RGB;
        it->path = "";
    
        //output
        it->raw = null;
        it->width = 512;
        it->height = 512;
        it->channels = 3;
    }
    dynamic(BE2DTextureData)
        funbind(release);
    }
    return any;
}
