//
//  MC3DModel.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DModel_h
#define MC3DModel_h

#include "monkc_export.h"
#include "MC3DNode.h"
#include "MCTexture.h"

class(MC3DModel, MC3DNode,
      int tag;
      char name[256];
      //char mtl[256];

      MCColorf defaultColor;
      const char* defaultExtension;
      MCBool textureOnOff;
      MCBool fitted;
      
      MC3DFrame lastSavedFrame;
      computing(MC3DFrame, frame);
      computing(MCVector3, center);
      computing(double, maxlength);
);

fun(MC3DModel, void, bye, voida);
fun(MC3DModel, MC3DModel*, initWithFilePath, const char* path);
fun(MC3DModel, MC3DModel*, initWithFileName, const char* name);
fun(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorf color);
fun(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorf color);
fun(MC3DModel, void, translateToOrigin, voida);
//rotate self (counter clock wise is positive)
fun(MC3DModel, void, rotateAroundSelfAxisX, double ccwRadian);
fun(MC3DModel, void, rotateAroundSelfAxisY, double ccwRadian);
fun(MC3DModel, void, rotateAroundSelfAxisZ, double ccwRadian);
//resize
fun(MC3DModel, void, resizeToFit, double maxsize);

#endif /* MC3DModel_h */
