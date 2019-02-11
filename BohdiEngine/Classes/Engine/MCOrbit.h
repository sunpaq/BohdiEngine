//
//  MCOrbit.h
//  monkcGame
//
//  Created by SunYuLi on 5/24/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MCOrbit__
#define __monkcGame__MCOrbit__

//#include "monkc_export.h"
#include "MC3DNode.h"

structure(MCOrbit, MC3DNode)
    double R;
    unsigned segments;
    MCVector3 center;
    MCVector3 up;
    //vertex array
    unsigned vertexArrayId;
    unsigned vertexBufferId;
};

constructor(MCOrbit));

alias(MCOrbit);

#endif /* defined(__monkcGame__MCOrbit__) */
