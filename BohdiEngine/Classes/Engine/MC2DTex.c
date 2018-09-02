//
//  MC2DTex.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC2DTex.h"

oninit(MC2DTex)
{
    if (init(MCTexture)) {
        return obj;
    }else{
        return null;
    }
}

fun(MC2DTex, MC2DTex*, initWithFileName, const char* name)
{
    return obj;
}

onload(MC2DTex)
{
    if (load(MCTexture)) {
        
        bid(MC2DTex, MC2DTex*, initWithFileName, const char* name);
        
        return cla;
    }else{
        return null;
    }
}
