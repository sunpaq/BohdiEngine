//
//  MCMatrial.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMaterial.h"
#include "MC3DBase.h"

constructor(MCMaterial)) {
    MCObject(any);
    as(MCMaterial)
        it->ambientLightColor  = MCVector3Make(1.0, 1.0, 1.0);
        it->diffuseLightColor  = MCVector3Make(1.0, 1.0, 1.0);
        it->specularLightColor = MCVector3Make(1.0, 1.0, 1.0);
        it->specularLightPower = 32.0;
        it->dissolve = 1.0;
        it->hidden   = 0;
        it->illum    = 2;
        it->dataChanged = true;
        it->tag[0] = NUL;
    }
    return any;
}
