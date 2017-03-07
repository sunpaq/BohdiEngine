//
//  MCSkysphereCamera.h
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#ifndef MCSkysphereCamera_h
#define MCSkysphereCamera_h

#include "MCCamera.h"

class(MCSkysphereCamera, MCCamera,
      //matrix
      computing(MCMatrix4, viewMatrix);
      computing(MCMatrix4, projectionMatrix);
);

//override
method(MCSkysphereCamera, MCSkysphereCamera*, initWithWidthHeightRatio, MCFloat ratio);
method(MCSkysphereCamera, void, move, MCFloat deltaFai, MCFloat deltaTht);
method(MCSkysphereCamera, void, update, MCGLContext* ctx);
method(MCSkysphereCamera, void, setRotationMat3, float mat3[9]);

#endif /* MCSkysphereCamera_h */
