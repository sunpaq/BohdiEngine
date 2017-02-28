//
//  BE2DTextureData.c
//  Sapindus
//
//  Created by YuliSun on 24/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#include "BEAssetsManager.h"
#include "BE2DTextureData.h"
#include "MCString.h"
#include "SOIL.h"

oninit(BE2DTextureData)
{
    if (init(MCObject)) {
        //const types
        obj->AUTO = 0;
        obj->L    = 1;
        obj->LA   = 2;
        obj->RGB  = 3;
        obj->RGBA = 4;
        
        //input
        //obj->type = obj->RGB;
        obj->path = "";
        
        //output
        obj->raw = null;
        return obj;
    }else{
        return null;
    }
}

utility(BE2DTextureData, BE2DTextureData*, newWithPathType, const char* path, unsigned type)
{
    BE2DTextureData* data = new(BE2DTextureData);
    
    char decodepath[PATH_MAX] = {0};
    MCString_percentDecode(path, decodepath);
    
    size_t psize = strlen(decodepath) * sizeof(char);
    data->path = strcpy(malloc(psize), decodepath);
    
    //    if (type >= data->AUTO && type<= data->RGBA ) {
    //        data->type = type;
    //    }else{
    //        data->type = data->RGB;
    //    }
    
    data->raw = SOIL_load_image(data->path, &data->width, &data->height, &data->channels, SOIL_LOAD_AUTO);
    return data;
}

utility(BE2DTextureData, BE2DTextureData*, newWithPath, const char* path)
{
    return BE2DTextureData_newWithPathType(path, 3);//default RGB
}

method(BE2DTextureData, void, bye, voida)
{
    if (obj->raw != null) {
        SOIL_free_image_data(obj->raw);
    }
    superbye(MCObject);
}

onload(BE2DTextureData)
{
    if (load(MCObject)) {
        binding(BE2DTextureData, void, bye, voida);
        return cla;
    }else{
        return null;
    }
}
