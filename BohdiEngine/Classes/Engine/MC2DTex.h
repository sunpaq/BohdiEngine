//
//  MC2DTex.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC2DTex_h
#define MC2DTex_h

#include "MCObject.h"
#include "MCTexture.h"

structure(MC2DTex, MCTexture)
    fundef(initWithFileName, struct MC2DTex*), const char* name);
};

constructor(MC2DTex));

alias(MC2DTex);

#endif /* MC2DTex_h */
