//
//  MCMesh.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMesh.h"
#include "MCLog.h"

fun(release, void)) as(MCMesh)
    if (it->vertexDataNeedRelease && it->vertexDataPtr) {
        free(it->vertexDataPtr);
    }
}

fun(allocVertexBuffer, void), int32_t vertexCount) as(MCMesh)
    it->vertexCount = vertexCount ;
    it->vertexDataSize = it->vertexCount * sizeof(MCVertexData);
    if (it->vertexDataSize != 0) {
        it->vertexDataPtr = (float*)malloc(it->vertexDataSize);
        memset(it->vertexDataPtr, 0, it->vertexDataSize);
    }else{
        it->vertexDataPtr = null;
    }
}

fun(setVertex, void), uint32_t index, MCVertexData* data) as(MCMesh)
    MCVertexData* array = (MCVertexData*)it->vertexDataPtr;
    array[index].x = data->x;
    array[index].y = data->y;
    array[index].z = data->z;

    if (it->calculatedNormal) {
        array[index].nx += data->nx;
        array[index].ny += data->ny;
        array[index].nz += data->nz;
    } else {
        array[index].nx = data->nx;
        array[index].ny = data->ny;
        array[index].nz = data->nz;
    }
    
    array[index].r = data->r;
    array[index].g = data->g;
    array[index].b = data->b;
    
    array[index].u = data->u;
    array[index].v = data->v;
}

fun(normalizeNormals, void)) as(MCMesh)
    if (!it->calculatedNormal) {
        return;
    }
    MCVertexData* array = (MCVertexData*)it->vertexDataPtr;
    for (int i=0; i<it->vertexCount; i++) {
        MCVector3 n = MCVector3Normalize(MCVector3Make(array[i].nx, array[i].ny, array[i].nz));
        array[i].nx = n.x;
        array[i].ny = n.y;
        array[i].nz = n.z;
    }
}

constructor(MCMesh), int32_t vertexCount) {
    MCItem(any, null);
    as(MCMesh)
        it->isDataLoaded = false;
        it->calculatedNormal = false;
        it->Frame = (MC3DFrame){0,0,0,0,0,0};
        it->vertexDataNeedRelease = true;
        it->vertexDataPtr = null;
        it->vertexDataSize = 0;
        it->vertexIndexes = null;
        it->vertexCount = vertexCount;
    
        allocVertexBuffer(it, vertexCount);
        debug_log("MCMesh - init finished\n");
    }
    dynamic(MCMesh)
        funbind(release);
        funbind(setVertex);
        funbind(normalizeNormals);
    }
    return any;
}

