//
//  MC3DAndroidDriver.c
//  Sapindus
//
//  Created by Sun YuLi on 16/7/9.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifdef __ANDROID__

#include "monkc.h"
#include <stdio.h>
#include "MC3DiOS.h"

//MCGL Errors
void MCGLError(const char* errmsg)
{
	debug_log("MCGLError: %s\n", errmsg);
}

void MCGLStartLoading()
{
	debug_log("start loading\n");
}

void MCGLStopLoading()
{
	debug_log("stop loading\n");
}

int getSkyboxOn()
{
    return 0;
}

#endif

