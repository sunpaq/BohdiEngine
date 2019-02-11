//
//  MC3DAxis.h
//  Sapindus
//
//  Created by YuliSun on 27/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#ifndef MC3DAxis_h
#define MC3DAxis_h

//#include "monkc_export.h"
#include "MC3DNode.h"
#include "MC3DShapeBase.h"

structure(MC3DAxis, MC3DNode)
    unsigned buffId;

    //override MC3DNode
    fundef(release, void));
    fundef(draw, void));
};

constructor(MC3DAxis));

alias(MC3DAxis);

#endif /* MC3DAxis_h */
