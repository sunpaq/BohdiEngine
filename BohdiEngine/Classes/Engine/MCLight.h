//
//  MCLight.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCLight_h
#define MCLight_h

//#include "monkc_export.h"
#include "MC3DNode.h"

structure(MCLight, MC3DNode)
    struct MCLight* next;
    bool dataChanged;
    MCVector3 ambientLightStrength;
    MCVector3 diffuseLightStrength;
    MCVector3 specularLightStrength;
    MCVector3 lightColor;
    MCVector3 lightPosition;

    fundef(printDebugInfo, void));
};

constructor(MCLight));

alias(MCLight);

#endif /* MCLight_h */
