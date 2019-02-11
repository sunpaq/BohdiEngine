//
//  MCTexture.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCTexture.h"
#include "BEAssetsManager.h"
#include "BE2DTextureData.h"
#include "MCLog.h"

fun(loadImageRawdata, unsigned char*), const char* path) as(MCTexture)
    it->data = BE2DTextureData_newWithPathname(path);
    if (it->data) {
        it->width  = it->data->width;
        it->height = it->data->height;
        return it->data->raw;
    }
    else {
        error_log("MCTexture - can not load image: %s\n", path);
        return null;
    }
}

fun(freeRawdata, void)) as(MCTexture)
    if (it->data) {
        Release(it->data);
        it->data = null;
    }
}

fun(release, void)) as(MCObject)
    freeRawdata(it);
    it->release(it);
}

fun(initWithFileNameMode, struct MCTexture*), const char* name, MCTextureDisplayMode mode) as(MCTexture)
    char pathbuff[PATH_MAX] = {0};
    if (MCFileGetPath(name, pathbuff)) {
        return null;
    }
    it->displayMode = mode;
    loadImageRawdata(it, pathbuff);
    return it;
}

fun(initWithFileName, struct MCTexture*), const char* name) as(MCTexture)
    struct MCTexture* tex = MCTexture(alloc(MCTexture));
    return tex->initWithFileNameMode(tex, name, MCTextureRepeat);
}

fun(initWith2DTexture, struct MCTexture*), struct BE2DTextureData* tex) as(MCTexture)
    it->data = tex;
    if (it->data) {
        it->width  = it->data->width;
        it->height = it->data->height;
        return it;
    }
    return null;
}

constructor(MCTexture)) {
    MCObject(any);
    as(MCTexture)
        it->Id = 0;
        it->width = 512;
        it->height = 512;
        it->data = null;
        it->displayMode = MCTextureRepeat;
        it->loadedToGL = false;
    
        funadd(loadImageRawdata);
    };
    dynamic(MCTexture)
        funbind(release);
        funbind(initWithFileNameMode);
        funbind(initWithFileName);
        funbind(initWith2DTexture);
    };
    return any;
}

