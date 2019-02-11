//
//  MCLight.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCLight.h"
#include "MCLog.h"

fun(printDebugInfo, void)) as(MCLight)
    debug_log("MCLight: lightColor=%.2f/%.2f/%.2f lightPosition=%.2f/%.2f/%.2f\n",
              it->lightColor.x, it->lightColor.y, it->lightColor.z,
              it->lightPosition.x, it->lightPosition.y, it->lightPosition.z);
}

constructor(MCLight)) {
    MC3DNode(any);
    as(MCLight)
        it->next = null;
        it->ambientLightStrength  = MCVector3Make(0.2, 0.2, 0.2);
        it->diffuseLightStrength  = MCVector3Make(1.0, 1.0, 1.0);
        it->specularLightStrength = MCVector3Make(0.1, 0.1, 0.1);
    
        it->lightColor    = MCVector3Make(1.0, 1.0, 1.0);
        it->lightPosition = MCVector3Make(0, 0, -1000);
    
        it->dataChanged = true;
    }
    dynamic(MCLight)
        funbind(printDebugInfo);
    }
    return any;
}


