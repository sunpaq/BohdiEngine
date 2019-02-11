//
//  MCOrbit.c
//  monkcGame
//
//  Created by SunYuLi on 5/24/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCOrbit.h"
#include "MC3DShapeBase.h"

static double gCircleVertexData[360*3];

constructor(MCOrbit)) {
    MC3DNode(any);
    as(MCOrbit)
        it->R = 10.0;
        it->segments = 360.0;
        it->center = MCVector3Make(0, 0, 0);
        it->up = MCVector3Make(0, 0, 1.0);
        MakeCircleData(0, 0, 0, it->R, it->segments, gCircleVertexData);
    }
    return any;
}
