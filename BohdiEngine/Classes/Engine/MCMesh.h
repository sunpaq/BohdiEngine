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

typedef union {
    struct {
        float x;
        float y;
        float z;
        float nx;
        float ny;
        float nz;
        float r;
        float g;
        float b;
        float u;
        float v;
    };
    float data[11];
} MCVertexData;

class(MCMesh, MCItem,
      MCBool     isDataLoaded;
      MCBool     calculatedNormal;
      MCBool     vertexDataNeedRelease;
      float*   vertexDataPtr;   //gCubeVertexData
      intptr_t vertexDataSize;  //sizeof(gCubeVertexData)
      uint32_t*    vertexIndexes;
      int32_t    vertexCount;

      MC3DFrame  Frame;

      //MCVertexAttribute vertexAttribArray[MCVertexAttribIndexMax];
      //MCDrawMode mode;
);

method(MCMesh, void, bye, voida);
method(MCMesh, MCMesh*, initWithVertexCount, int32_t vertexCount);
method(MCMesh, void, setVertex, uint32_t offset, MCVertexData* data);
method(MCMesh, void, normalizeNormals, voida);

#endif /* MCMesh_h */
