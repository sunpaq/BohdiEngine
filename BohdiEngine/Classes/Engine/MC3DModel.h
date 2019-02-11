//
//  MC3DModel.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DModel_h
#define MC3DModel_h

//#include "monkc_export.h"
#include "MC3DNode.h"
#include "MCTexture.h"

structure(MC3DModel, MC3DNode)
    int tag;
    char name[256];
    //char mtl[256];

    MCColorf defaultColor;
    const char* defaultExtension;
    bool textureOnOff;
    bool fitted;
    MC3DFrame lastSavedFrame;

    fundef(frame, MC3DFrame));
    fundef(center, MCVector3));
    fundef(maxlength, double));

    fundef(release, void));
    fundef(initWithFilePath, struct MC3DModel*), const char* path);
    fundef(initWithFileName, struct MC3DModel*), const char* name);
    fundef(initWithFilePathColor, struct MC3DModel*), const char* path, MCColorf color);
    fundef(initWithFileNameColor, struct MC3DModel*), const char* name, MCColorf color);
    fundef(translateToOrigin, void));
    //rotate self (counter clock wise is positive)
    fundef(rotateAroundSelfAxisX, void), double ccwRadian);
    fundef(rotateAroundSelfAxisY, void), double ccwRadian);
    fundef(rotateAroundSelfAxisZ, void), double ccwRadian);
    //resize
    fundef(resizeToFit, void), double maxsize);
};

constructor(MC3DModel));

alias(MC3DModel);

#endif /* MC3DModel_h */

