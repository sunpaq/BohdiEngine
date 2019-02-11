//
//  MCMesh.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCMesh_h
#define MCMesh_h

//#include "monkc_export.h"
#include "MC3DBase.h"
#include "MCLinkedList.h"

structure(MCMesh, MCItem)
    bool isDataLoaded;
    bool calculatedNormal;
    bool vertexDataNeedRelease;

    float* vertexDataPtr;
    size_t vertexDataSize;
    uint32_t* vertexIndexes;
    uint32_t vertexCount;

    MC3DFrame  Frame;

    fundef(release, void));
    fundef(setVertex, void), uint32_t index, MCVertexData* data);
    fundef(normalizeNormals, void));
};

constructor(MCMesh), int32_t vertexCount);

alias(MCMesh);

#endif /* MCMesh_h */
