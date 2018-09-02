//
//  MCWorld.c
//  monkcGame
//
//  Created by SunYuLi on 15/12/25.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCWorld.h"
#include "MC3DBase.h"

oninit(MCWorld)
{
    if (init(MCObject)) {
        return obj;
    }else{
        return null;
    }
}

fun(MCWorld, void, bye, voida)
{
    //clean up
}

fun(MCWorld, MCWorld*, initWith, voida)
{
    return obj;
}

onload(MCWorld)
{
    if (load(MCObject)) {
        bid(MCWorld, void, bye, voida);
        bid(MCWorld, MCWorld*, initWith, voida);
        return cla;
    }else{
        return null;
    }
}

