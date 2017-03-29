//
//  MCSkysphere.h
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#ifndef MCSkysphere_h
#define MCSkysphere_h

#include "MC3DNode.h"
#include "BE2DTextureData.h"

class(MCSkysphere, MC3DNode,
      BE2DTextureData* tex;
      MCGLContext* ctx;
      
      MCMatrix4 sphViewMatrix;
      MCMatrix4 sphProjectionMatrix;
      double sphCameraRatio;
      double sphCameraAngle;
            
      size_t   vertices_size;
      size_t   indices_size;
      GLfloat* vertices;
      GLuint*  indices;
      GLuint   ic;
      
      MCUInt vaoid;
      MCUInt vboid;
      MCUInt eboid;
      MCUInt texid);

method(MCSkysphere, void, bye, voida);
method(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex);
method(MCSkysphere, MCSkysphere*, initWithFileName, const char* name);
method(MCSkysphere, MCSkysphere*, initWithDefaultFile, voida);

method(MCSkysphere, void, setRotationMat3, float mat3[9]);
//override
method(MCSkysphere, void, update, MCGLContext* ctx);
method(MCSkysphere, void, draw, MCGLContext* ctx);

#endif /* MCSkysphere_h */
