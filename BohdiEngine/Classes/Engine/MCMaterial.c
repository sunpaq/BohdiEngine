//
//  MCMatrial.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMaterial.h"
#include "MC3DBase.h"

oninit(MCMaterial)
{
    if (init(MCObject)) {
        obj->ambientLightColor  = MCVector3Make(1.0, 1.0, 1.0);
        obj->diffuseLightColor  = MCVector3Make(1.0, 1.0, 1.0);
        obj->specularLightColor = MCVector3Make(1.0, 1.0, 1.0);
        obj->specularLightPower = 32.0;
        obj->dissolve = 1.0;
        obj->hidden   = 0;
        obj->illum    = 2;
        obj->dataChanged = true;
        
        obj->tag[0] = NUL;
        return obj;
    }else{
        return null;
    }
}

onload(MCMaterial)
{
    if (load(MCObject)) {
        return cla;
    }else{
        return null;
    }
}
