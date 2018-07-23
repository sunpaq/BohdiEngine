//
//  MCMesh.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCMesh_h
#define MCMesh_h

#include "monkc.h"
#include "MC3DBase.h"
#include "MCLinkedList.h"
#include "MCMath.h"
#include "BAObjParser.h"

class(MCMesh, MCItem,
      MCBool     isDataLoaded;
      MCBool     calculatedNormal;
      MCBool     vertexDataNeedRelease;
      
      float*     vertexDataPtr;   //gCubeVertexData
      size_t     vertexDataSize;  //sizeof(gCubeVertexData)
      uint32_t*  vertexIndexes;
      uint32_t   vertexCount;

      MC3DFrame  Frame;
);

method(MCMesh, void, bye, voida);
method(MCMesh, MCMesh*, initWithVertexCount, int32_t vertexCount);
method(MCMesh, void, setVertex, uint32_t index, MCVertexData* data);
method(MCMesh, void, normalizeNormals, voida);

#endif /* MCMesh_h */
