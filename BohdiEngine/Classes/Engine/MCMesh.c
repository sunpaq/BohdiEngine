//
//  MCMesh.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMesh.h"

oninit(MCMesh)
{
    if (init(MCItem)) {
        var(isDataLoaded) = false;
        var(calculatedNormal) = false;
        
        var(Frame) = (MC3DFrame){0,0,0,0,0,0};

        var(vertexDataNeedRelease) = true;
        var(vertexDataPtr) = null;
        var(vertexDataSize)= 0;
        var(vertexIndexes) = null;
        var(vertexCount)   = 0;
        
        debug_log("MCMesh - init finished\n");
        return obj;
    }else{
        return null;
    }
}

fun(MCMesh, void, bye, voida)
{
    if (obj->vertexDataNeedRelease && obj->vertexDataPtr) {
        free(obj->vertexDataPtr);
    }
}

fun(MCMesh, void, allocVertexBuffer, int32_t vertexCount)
{
    obj->vertexCount = vertexCount ;
    obj->vertexDataSize = obj->vertexCount * sizeof(MCVertexData);
    if (obj->vertexDataSize != 0) {
        obj->vertexDataPtr = (float*)malloc(obj->vertexDataSize);
        memset(obj->vertexDataPtr, 0, obj->vertexDataSize);
    }else{
        obj->vertexDataPtr = null;
    }
}

fun(MCMesh, MCMesh*, initWithVertexCount, int32_t vertexCount)
{
    //alloc vertex buffer
    MCMesh_allocVertexBuffer(obj, vertexCount);
    //obj->vertexIndexes = (GLuint*)malloc(sizeof(GLuint)*obj->vforertexCount);
    return obj;
}

fun(MCMesh, void, setVertex, uint32_t index, MCVertexData* data)
{
    MCVertexData* array = (MCVertexData*)obj->vertexDataPtr;
    array[index].x = data->x;
    array[index].y = data->y;
    array[index].z = data->z;

    if (obj->calculatedNormal) {
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

fun(MCMesh, void, normalizeNormals, voida)
{
    if (!obj->calculatedNormal) {
        return;
    }
    MCVertexData* array = (MCVertexData*)obj->vertexDataPtr;
    for (int i=0; i<obj->vertexCount; i++) {
        MCVector3 n = MCVector3Normalize(MCVector3Make(array[i].nx, array[i].ny, array[i].nz));
        array[i].nx = n.x;
        array[i].ny = n.y;
        array[i].nz = n.z;
    }
}

onload(MCMesh)
{
    if (load(MCItem)) {
        bid(MCMesh, void, bye, voida);
        bid(MCMesh, MCMesh*, initWithVertexCount, int32_t vertexCount);
        bid(MCMesh, void, setVertex, uint32_t offset, MCVertexData* data);
        bid(MCMesh, void, normalizeNormals, voida);
        return cla;
    }else{
        return null;
    }
}


