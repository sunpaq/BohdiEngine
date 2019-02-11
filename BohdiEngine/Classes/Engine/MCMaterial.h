//
//  MCMatrial.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCMatrial_h
#define MCMatrial_h

//#include "monkc_export.h"
#include "MCObject.h"
#include "MCMath.h"

structure(MCMaterial, MCObject)
    bool        dataChanged;
    MCVector3   ambientLightColor;
    MCVector3   diffuseLightColor;
    MCVector3   specularLightColor;
    double      specularLightPower;
    double      dissolve;
    int         hidden;
    int         illum;
    char        tag[256];
};

constructor(MCMaterial));

alias(MCMaterial);

#endif /* MCMatrial_h */
