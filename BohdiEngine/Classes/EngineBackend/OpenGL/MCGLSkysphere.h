//
//  MCGLSkysphere.h
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#ifndef MCGLSkysphere_h
#define MCGLSkysphere_h

#include "MC3DNode.h"
#include "BE2DTextureData.h"

class(MCGLSkysphere, MC3DNode,
      BE2DTextureData* tex;
      MCGLContext* ctx;
      
      MCMatrix4 sphViewMatrix;
      MCMatrix4 sphProjectionMatrix;
      double sphCameraRatio;
      double sphCameraAngle;
      
      //local spherical coordinate
      //R[0,unlimited) tht[0, 180.0), fai[0, 360.0)
      double R_value;
      double R_percent;
      double tht;
      double fai;
      
      size_t   vertices_size;
      size_t   indices_size;
      GLfloat* vertices;
      GLuint*  indices;
      GLuint   ic;
      
      MCUInt vaoid;
      MCUInt vboid;
      MCUInt eboid;
      MCUInt texid);

method(MCGLSkysphere, void, bye, voida);
method(MCGLSkysphere, MCGLSkysphere*, initWithBE2DTexture, BE2DTextureData* tex);
method(MCGLSkysphere, MCGLSkysphere*, initWithFileName, const char* name);
method(MCGLSkysphere, MCGLSkysphere*, initWithDefaultFile, voida);

method(MCGLSkysphere, void, setRotationMat3, float mat3[9]);
method(MCGLSkysphere, void, setRotationMat4, float mat4[16]);
//override
method(MCGLSkysphere, void, update, MCGLContext* ctx);
method(MCGLSkysphere, void, draw, MCGLContext* ctx);

#endif /* MCGLSkysphere_h */
