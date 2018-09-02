//
//  MC3DAxis.h
//  Sapindus
//
//  Created by YuliSun on 27/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#ifndef MC3DAxis_h
#define MC3DAxis_h

#include "monkc_export.h"
#include "MC3DNode.h"
#include "MC3DShapeBase.h"

class(MC3DAxis, MC3DNode,
      MCUInt buffId;
);

//override MC3DNode
fun(MC3DAxis, void, draw, voida);

#endif /* MC3DAxis_h */
